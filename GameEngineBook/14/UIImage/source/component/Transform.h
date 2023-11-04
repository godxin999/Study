#pragma once

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include "Component.h"

class Transform : public Component {
public:
	glm::vec3 position()const {
		return position_;
	}
	glm::vec3 rotation()const {
		return rotation_;
	}
	glm::vec3 scale()const {
		return scale_;
	}
	void set_position(const glm::vec3 position) {
		position_ = position;
	}
	void set_rotation(const glm::vec3 rotation) {
		rotation_ = rotation;
	}
	void set_scale(const glm::vec3 scale) {
		scale_ = scale;
	}

private:
	glm::vec3 position_{ 0.f };
	glm::vec3 rotation_{ 0.f };
	glm::vec3 scale_{ 1.f };
};

#endif