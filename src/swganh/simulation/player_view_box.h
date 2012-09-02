// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <swganh/object/object.h>

namespace swganh {
namespace simulation {

	class PlayerViewBox : public swganh::object::Object
	{
	public:
		PlayerViewBox(std::shared_ptr<swganh::object::Object> player);
		~PlayerViewBox();

		void OnCollisionEnter(std::shared_ptr<swganh::object::Object> collider);
		void OnCollisionLeave(std::shared_ptr<swganh::object::Object> collider);
		void __BuildCollisionBox();
		void __BuildBoundingVolume();

	private:
		std::shared_ptr<swganh::object::Object> player_;
	};

}} // namespace swganh::simulation