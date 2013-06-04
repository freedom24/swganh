// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "packet_utilities.h"

#include <cassert>
#include <algorithm>
#include <stdexcept>

#include <boost/lexical_cast.hpp>

#include "swganh/crc.h"
#include "swganh/utilities.h"

using namespace swganh;
using namespace std;
using swganh::memcrc;
using boost::lexical_cast;

namespace swganh {
namespace network {
namespace soe {

ByteBuffer BuildDataChannelHeader(uint16_t sequence) {
    ByteBuffer data_channel_header;

    data_channel_header.write<uint16_t>(hostToBig<uint16_t>(0x09));
    data_channel_header.write<uint16_t>(hostToBig<uint16_t>(sequence));

    return data_channel_header;
}

ByteBuffer BuildFragmentedDataChannelHeader(uint16_t sequence) {
    ByteBuffer data_channel_header;

    data_channel_header.write<uint16_t>(hostToBig<uint16_t>(0x0D));
    data_channel_header.write<uint16_t>(hostToBig<uint16_t>(sequence));

    return data_channel_header;
}

list<ByteBuffer> SplitDataChannelMessage(ByteBuffer message, uint32_t max_size) {
    uint32_t message_size = message.size();  
    
    if (message_size < max_size) {
        throw invalid_argument("Message must be bigger than max size");
    }

    list<ByteBuffer> fragmented_messages;

    uint32_t remaining_bytes = message_size;
    uint32_t chunk_size = 0;

    while (remaining_bytes != 0) {        
        ByteBuffer fragment;
        chunk_size = (remaining_bytes > max_size) ? max_size : remaining_bytes;

        if (remaining_bytes == message_size) {
            chunk_size -= sizeof(uint32_t);
            fragment.write<uint32_t>(hostToBig<uint32_t>(message_size));
        }
        
        fragment.write(message.data() + (message_size - remaining_bytes), chunk_size);

        fragmented_messages.push_back(move(fragment));
        remaining_bytes -= chunk_size;
    }

    return fragmented_messages;
}

uint32_t CreateEndpointHash(const boost::asio::ip::udp::endpoint& endpoint) {
    string crc_string = endpoint.address().to_string() + ":" + lexical_cast<string>(endpoint.port());
    return memcrc(crc_string);
}

}}}  // namespace swganh::network::soe
