#ifndef AKCP_SERVER_H
#define AKCP_SERVER_H

#include "asio.hpp"
#include "asio/steady_timer.hpp"
#include "defs.h"

namespace akcp {
using namespace asio::ip;

class ConnectionManager;

class Server {
  public:
    Server(asio::io_context &io, const std::string &address, short port);
    Server(asio::io_context &io, short port);

    void start();
    void stop();

    void send_packet(const char* buf, size_t nbytes, udp::endpoint remote);

  private:
    void do_recv();

    void update();

    void kcp_update();

    asio::io_context &io_;
    udp::socket socket_;
    asio::steady_timer timer_;

    bool stop_ = false;

    ConnectionManager *manager_;
};
} // namespace akcp
#endif