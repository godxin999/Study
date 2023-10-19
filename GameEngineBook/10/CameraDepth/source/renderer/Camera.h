#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <vector>
#include <functional>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "../component/Component.h"

class Camera :public Component {
public:
	Camera();
	~Camera();
public:
	void setView(const glm::vec3& cameraForward, const glm::vec3& cameraUp);
	void setProjection(float fovDegrees, float aspectRatio, float nearClip, float farClip);

	glm::mat4& view_mat4() {
		return view_mat4_;
	}
	glm::mat4& projection_mat4() {
		return projection_mat4_;
	}
	unsigned char depth() {
		return depth_;
	}
	void set_clear_color(float r, float g, float b, float a) {
		clear_color_ = glm::vec4(r, g, b, a);
	}
	void set_clear_flag(unsigned clear_flag) {
		clear_flag_ = clear_flag;
	}
	void set_depth(unsigned char depth) {
		if (depth_ == depth)return;
		depth_ = depth;
		Sort();
	}
	void clear();

	static void Foreach(std::function<void()> func);
	static Camera* current_camera() {
		return current_camera_;
	}
	static void Sort();
private:
	glm::mat4 view_mat4_{ 1.f };
	glm::mat4 projection_mat4_{ 1.f };
	glm::vec4 clear_color_{ 49.f / 255,77.f / 255,121.f / 255,1.f };
	unsigned clear_flag_{ GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT };

	//深度值越大的相机越后渲染，UI相机应该覆盖在游戏世界上，所以UI相机的深度值应该大于游戏世界相机的深度值，即位于相机队列的末尾
	unsigned char depth_{ 0 };

	inline static std::vector<Camera*> all_camera_;
	inline static Camera* current_camera_ = nullptr;
};

#endif