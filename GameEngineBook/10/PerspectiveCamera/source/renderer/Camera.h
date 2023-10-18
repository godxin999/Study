#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "../component/Component.h"

class Camera :public Component {
public:
	void setView(const glm::vec3& cameraForward, const glm::vec3& cameraUp);
	void setProjection(float fovDegrees, float aspectRatio, float nearClip, float farClip);

	glm::mat4& view_mat4() {
		return view_mat4_;
	}
	glm::mat4& projection_mat4() {
		return projection_mat4_;
	}
	void set_clear_color(float r, float g, float b, float a) {
		clear_color_=glm::vec4(r,g,b,a);
	}
	void set_clear_flag(unsigned clear_flag) {
		clear_flag_=clear_flag;
	}
	void clear();
private:
	glm::mat4 view_mat4_;
	glm::mat4 projection_mat4_;
	glm::vec4 clear_color_{ 49.f / 255,77.f / 255,121.f / 255,1.f };
	unsigned clear_flag_{ GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT };
};

#endif