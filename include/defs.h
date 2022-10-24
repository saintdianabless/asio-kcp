#ifndef AKCP_DEFS_H
#define AKCP_DEFS_H

#include <cstddef>

using kcp_conv_t = unsigned int;
using update_clock_t = unsigned int;
constexpr size_t KCP_HEADER_SIZE = 20;
constexpr size_t BUFFER_SIZE = 1500;

#endif