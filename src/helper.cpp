#include "helper.h"
#include "fmt/core.h"
#include "fmt/printf.h"
#include <cctype>
#include <cstddef>

static void asciiOutput(const char *data, size_t len) {
    std::string buf(len + 1, '\0');
    for (int i = 0; i < len; ++i) {
        if (isprint(data[i])) {
            buf[i] = data[i];
        } else {
            buf[i] = '?';
        }
    }
    fmt::print("{}", buf);
}

void HexOutput(const char *data, long len) {
    fmt::print("{:^10} {:02X} {:02X} {:02X} {:02X} {:02X} {:02X} {:02X} {:02X} "
               "{:02X} {:02X} {:02X} {:02X} {:02X} {:02X} {:02X} {:02X} \n",
               "Offset", 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
    int idx = 0;
    while (len > 0) {
        auto nbyte = std::min(16l, len);
        auto start = data + idx * 16;
        fmt::print("{:010X} ", idx);
        for (int i = 0; i < nbyte; ++i) {
            fmt::print("{:02X} ", (unsigned char)start[i]);
        }
        int padding = (16 - nbyte) * 3;
        fmt::print("{:>{p}}", "", fmt::arg("p", padding));
        asciiOutput(start, nbyte);
        fmt::print("\n");
        idx += 1;
        len -= 16;
    }
}