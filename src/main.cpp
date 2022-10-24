#include "asio/io_context.hpp"
#include "asio/signal_set.hpp"
#include "server.h"
#include <system_error>

int main() {
    asio::io_context io;

    akcp::Server s(io, 9900);
    s.start();

    asio::signal_set signals(io, SIGINT, SIGTERM);
    signals.async_wait([&](const std::error_code&, int){
        s.stop();
        io.stop();
    });

    io.run();
}