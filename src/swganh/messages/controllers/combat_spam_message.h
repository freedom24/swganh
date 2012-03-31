// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_CONTROLLERS_COMBAT_SPAM_MESSAGE_H_
#define SWGANH_MESSAGES_CONTROLLERS_COMBAT_SPAM_MESSAGE_H_

#include <cstdint>
#include "anh/byte_buffer.h"

#include "swganh/messages/obj_controller_message.h"

namespace swganh {
namespace messages {
namespace controllers {

    class CombatSpamMessage : ObjControllerMessage
    {
    public:
        explicit CombatSpamMessage(uint32_t controller_type = 0x0000000B)
            : ObjControllerMessage(controller_type, message_type())
        {}

        explicit CombatSpamMessage(ObjControllerMessage controller_message)
            : ObjControllerMessage(std::move(controller_message))
        {
            Deserialize(std::move(data));
        }

    	static uint32_t message_type() { return 0x00000134; }
    
    	CombatSpamMessage()
            : attacker_id(0)
            , defender_id(0)
            , weapon_id(0)
            , damage(0)
            , file("")
            , text("cbt_spam")
            , color_flag(1)
            , unicode_string(L"")
    	{}
        
    	uint64_t attacker_id;
        uint64_t defender_id;
    	uint64_t weapon_id;
        int32_t damage;
    	std::string file;
    	std::string text;
    	uint8_t color_flag;
        std::wstring unicode_string;
    
    	void Serialize(anh::ByteBuffer& buffer) const {
    		buffer.write(attacker_id);
    		buffer.write(defender_id);
    		buffer.write(weapon_id);
    		buffer.write(damage);
    		buffer.write(file);
    		buffer.write(0);
            buffer.write(text);
            buffer.write(color_flag);
            buffer.write(unicode_string);
    
    	}
    
    	void Deserialize(anh::ByteBuffer buffer) {
    		attacker_id = buffer.read<uint64_t>();
    		defender_id = buffer.read<uint64_t>();
    		weapon_id = buffer.read<uint64_t>();
    		damage = buffer.read<int>();
    		file = buffer.read<std::string>();
            buffer.read<uint32_t>();
    		text = buffer.read<std::string>();
            color_flag = buffer.read<uint8_t>();
            unicode_string = buffer.read<std::wstring>();
    	}
    };

}}}  // namespace swganh::messages::controllers

#endif  // SWGANH_MESSAGES_CONTROLLERS_COMBAT_SPAM_MESSAGE_H_
