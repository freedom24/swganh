// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <list>
#include <memory>

#include <boost/asio/ip/udp.hpp>

#include "swganh/byte_buffer.h"

namespace swganh {
namespace network {
namespace soe {

/**
 * Builds a data channel message header with the provided sequence.
 *
 * @param sequence The sequence of the data channel message header being created.
 * @return A byte buffer containing the fully constructed data channel message header.
 */
swganh::ByteBuffer BuildDataChannelHeader(uint16_t sequence);

/**
 * Builds a fragmented data channel message header with the provided sequence.
 *
 * @param sequence The sequence of the data channel message header being created.
 * @return A byte buffer containing the fully constructed fragmented data channel message header.
 */
swganh::ByteBuffer BuildFragmentedDataChannelHeader(uint16_t sequence);

/**
 * Splits a large data channel message into fragments.
 *
 * @param message The message to split.
 * @param max_size The maximum size of of each fragment.
 * @return A list containing the in-order message fragments.
 */
std::list<swganh::ByteBuffer> SplitDataChannelMessage(swganh::ByteBuffer message, uint32_t max_size);

/**
 * Creates a uint32_t hash from an endpoint.
 *
 * @param endpoint The endpoint to hash.
 * @return A hash of the provided endpoint.
 */
uint32_t CreateEndpointHash(const boost::asio::ip::udp::endpoint& endpoint);

class EndpointHash {
  public:
    size_t operator ()(const boost::asio::ip::udp::endpoint& endpoint) const { 
        return swganh::network::soe::CreateEndpointHash(endpoint); 
    }
};

class EndpointEqual {
  public:
    bool operator ()(
        const boost::asio::ip::udp::endpoint& endpoint1, 
        const boost::asio::ip::udp::endpoint& endpoint2) const
    {
        return endpoint1 == endpoint2;
    }
};

}}}  // namespace swganh::network::soe
