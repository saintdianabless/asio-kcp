#include "akcp.h"
#include "fmt/core.h"
#include "helper.h"
#include "ikcp.h"
#include <memory>

static int udp_output(const char *buf, int len, ikcpcb *kcp, void *user) {
    auto conn = static_cast<akcp::AKcp *>(user);
    conn->send_packet(buf, len);
    return 0;
}

namespace akcp {

AKcp::AKcp(____S, udp::endpoint ep, kcp_conv_t conv, Server *server)
    : remote_ep_(ep), conv_(conv), server_(server) {
    kcp_ = ikcp_create(conv, this);
    kcp_->output = udp_output;

    ikcp_nodelay(kcp_, 1, 5, 1, 1);
}

std::shared_ptr<AKcp> AKcp::create(udp::endpoint ep, kcp_conv_t conv,
                                   Server *server) {
    return std::make_shared<AKcp>(____S{}, ep, conv, server);
}

void AKcp::recv(std::unique_ptr<char[]> data, size_t nrecv) {
    ikcp_input(kcp_, data.get(), nrecv);
    auto buf = std::make_unique<char[]>(1024*1000);
    int n = ikcp_recv(kcp_, buf.get(), 1024 * 1000);
    if (n < 0) {
        return;
    }
    // HexOutput(buf.get(), n);
    send_kcp_message(buf.get(), n);
}

void AKcp::send_packet(const char *buf, size_t len) {
    server_->send_packet(buf, len, remote_ep_);
}

void AKcp::send_kcp_message(const char *data, size_t len) {
    int n = ikcp_send(kcp_, data, len);
    if (n < 0) {
        fmt::print("error when send kcp packet: {}\n", n);
    }
}

void AKcp::close() { ikcp_release(kcp_); }

} // namespace akcp