#ifndef AKCP_AKCP_H
#define AKCP_AKCP_H

#include <cstddef>
#include <memory>

#include "asio.hpp"
#include "defs.h"
#include "ikcp.h"
#include "server.h"

namespace akcp {
using namespace asio::ip;

class AKcp : public std::enable_shared_from_this<AKcp> {
  private:
    struct ____S {
        explicit ____S() = default;
    };

  public:
    AKcp(____S, udp::endpoint ep, kcp_conv_t conv, Server *server);

    static std::shared_ptr<AKcp> create(udp::endpoint ep, kcp_conv_t con,
                                        Server *server);

    void update(update_clock_t now) { ikcp_update(kcp_, now); }

    // 用于kcp_output
    void send_packet(const char *buf, size_t len);

    // 用户使用
    void send_kcp_message(const char *data, size_t len);

    void recv(std::unique_ptr<char[]> data, size_t nrecv);

    void set_remote_endpoint(udp::endpoint ep) { remote_ep_ = ep; }

    void close();

  private:
    ikcpcb *kcp_;
    udp::endpoint remote_ep_;
    kcp_conv_t conv_;
    Server *server_;
};

using AKcpPtr = std::shared_ptr<AKcp>;
} // namespace akcp

#endif