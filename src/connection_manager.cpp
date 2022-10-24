#include "connection_manager.h"
#include "akcp.h"
#include "ikcp.h"

namespace akcp {

AKcpPtr ConnectionManager::get(const char *rawdata) {
    int conv = ikcp_getconv(rawdata);
    if (connections_.count(conv) == 0) {
        return nullptr;
    }
    return connections_.find(conv)->second;
}

void ConnectionManager::add(udp::endpoint remote, const char *raw_data,
                            Server *server) {
    int conv = ikcp_getconv(raw_data);
    auto akcp = AKcp::create(remote, conv, server);
    connections_[conv] = akcp;
}

void ConnectionManager::update(update_clock_t now) {
    for (const auto &p : connections_) {
        p.second->update(now);
    }
}

} // namespace akcp