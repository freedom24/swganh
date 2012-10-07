// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "food_attribute_template.h"

#include <sstream>
#include "attributes_helper.h"
#include "swganh/event_dispatcher.h"
#include "swganh_core/object/tangible/tangible.h"

using namespace std;
using namespace swganh;
using namespace swganh::object;
using namespace swganh::messages;
using namespace swganh::attributes;

FoodAttributeTemplate::FoodAttributeTemplate(swganh::EventDispatcher* dispatcher)
	: BaseAttributeTemplate(dispatcher)
{
	RegisterEventHandlers();
}

swganh::messages::AttributeListMessage FoodAttributeTemplate::BuildAttributeTemplate(shared_ptr<Object> object)
{
	uint64_t object_id = object->GetObjectId();
	if (HasAttributeListMessage(object_id))
		return object_attribute_list_messages_[object_id];

	swganh::messages::AttributeListMessage attribute_list_message;
	
	std::vector<Attribute> attribute_list;
	attribute_list_message.object_id = object->GetObjectId();

	attribute_list.push_back(Attribute("@obj_attr_n:original_name", object->GetAttributeAsString("original_name")));
	attribute_list.push_back(Attribute("@obj_attr_n:volume", AttributesHelper::GetVolume(object)));
    attribute_list.push_back(Attribute("@obj_attr_n:counter_uses_remaining", object->GetAttributeAsString("counter_uses_remaining")));
	AttributesHelper::SetOptionalAttribute(attribute_list, "@obj_attr_n:crafter", "crafter", object, false);
	AttributesHelper::SetOptionalAttribute(attribute_list, "@obj_attr_n:serial_number", "serial_number", object, false);
	attribute_list_message.attributes = move(attribute_list);

	object_attribute_list_messages_[object_id] = move(attribute_list_message);
	return attribute_list_message;
}

