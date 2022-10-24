#include <chrono>
#include <cstddef>
#include <iostream>
#include <memory>
#include <string>
#include <system_error>
#include <unordered_map>
#include <vector>

#include "connection_manager.h"
#include "defs.h"
#include "fmt/printf.h"
#include "helper.h"
#include "server.h"

static update_clock_t current() {
    const auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(
               now.time_since_epoch())
        .count();
}

namespace akcp {

Server::Server(asio::io_context &io, const std::string &address, short port)
    : io_(io), socket_(io_, udp::endpoint(address::from_string(address), port)),
      timer_(io), manager_(new ConnectionManager()) {
}

Server::Server(asio::io_context &io, short port)
    : Server(io, "127.0.0.1", port) {}

void Server::start() {
    do_recv();
    update();
}

void Server::stop() {
    stop_ = true;
    manager_->release();
    delete manager_;
    socket_.cancel();
}

void Server::send_packet(const char *buf, size_t nbytes, udp::endpoint remote) {
    socket_.async_send_to(asio::buffer(buf, nbytes), remote,
                          [](const std::error_code &, size_t nsent) {});
}

void Server::do_recv() {
    if (stop_) {
        return;
    }
    auto buf = std::make_unique<char[]>(BUFFER_SIZE);
    auto raw_buf = buf.get(); // evaluation order

    socket_.async_receive_from(
        asio::buffer(raw_buf, BUFFER_SIZE), remote_ep_,
        [this, buf = std::move(buf)](const std::error_code &ec,
                                     size_t nread) mutable {
            if (ec) {
                std::cout << ec.message() << std::endl;
            } else {
                HexOutput(buf.get(), nread);
                if (nread > KCP_HEADER_SIZE) {
                    auto kcp = manager_->get(buf.get());
                    if (kcp == nullptr) {
                        manager_->add(remote_ep_, buf.get(), this);
                        fmt::print("new connection: {}:{}\n",
                                   remote_ep_.address().to_string(),
                                   remote_ep_.port());
                    }
                    kcp = manager_->get(buf.get());
                    kcp->set_remote_endpoint(remote_ep_);
                    kcp->recv(std::move(buf), nread);
                }
            }
            do_recv();
        });
}

void Server::update() {
    if (stop_) {
        return;
    }
    timer_.expires_from_now(std::chrono::milliseconds(1));
    timer_.async_wait([this](const std::error_code &) { kcp_update(); });
}

void Server::kcp_update() {
    update();
    manager_->update(current());
}

} // namespace akcp