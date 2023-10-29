#include "Transform.h"
#include <rttr/registration>

RTTR_REGISTRATION
{
	rttr::registration::class_<Transform>("Transform")
		.constructor<>()(rttr::policy::ctor::as_raw_ptr)
		.property("position", &Transform::position, &Transform::set_position)
		.property("rotation", &Transform::rotation, &Transform::set_rotation)
		.property("scale", &Transform::scale, &Transform::set_scale);
}