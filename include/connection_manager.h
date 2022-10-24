#ifndef AKCP_CONNECTION_MANAGER_H
#define AKCP_CONNECTION_MANAGER_H

#include <unordered_map>

#include "akcp.h"
#include "asio.hpp"
#include "defs.h"

namespace akcp {
using namespace asio::ip;

class ConnectionManager {

  public:
    AKcpPtr get(const char *rawdata);

    void add(udp::endpoint remote, const char *raw_data, Server *server);

    void update(update_clock_t now);

    void release() {
        for (const auto &p : connections_) {
            p.second->close();
        }
        connections_.clear();
    }

  private:
    std::unordered_map<kcp_conv_t, AKcpPtr> connections_;
};
} // namespace akcp

#endif