
#ifndef _TCP_PACKET_H_
#define _TCP_PACKET_H_

#include <vector>
#include <tuple>

namespace TCP {

/**
 * Packet definition
 */
using Packet = std::vector<uint8_t>;

/**
 * Raw packet definition
 */
using RawPacket = std::tuple<const uint8_t*, std::size_t>;

}

#endif // _TCP_PACKET_H_
