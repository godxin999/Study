#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <vector>
#include <functional>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "component/Component.h"

class Camera :public Component {
public:
	Camera();
	~Camera();
public:
	enum class Projection {
		Perspective,
		Orthographic
	};
public:
	static void Foreach(std::function<void()> func);
	static Camera* current_camera() {
		return current_camera_;
	}
	static void Sort();
public:
	void SetView(const glm::vec3& cameraForward, const glm::vec3& cameraUp);
	void SetPerspective(float fovDegrees, float aspectRatio, float nearClip, float farClip);
	void SetOrthographic(float left, float right, float bottom, float top, float z_near, float z_far);
	glm::mat4& view_mat4() {
		return view_mat4_;
	}
	glm::mat4& projection_mat4() {
		return projection_mat4_;
	}
	unsigned char depth() {
		return depth_;
	}
	unsigned char culling_mask() {
		return culling_mask_;
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
	void set_culling_mask(unsigned char culling_mask) {
		culling_mask_ = culling_mask;
	}
	void Clear();
private:
	glm::mat4 view_mat4_;
	glm::mat4 projection_mat4_;
	glm::vec4 clear_color_{ 49.f / 255,77.f / 255,121.f / 255,1.f };
	unsigned clear_flag_{ GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT };

	//深度值越大的相机越后渲染，UI相机应该覆盖在游戏世界上，所以UI相机的深度值应该大于游戏世界相机的深度值，即位于相机队列的末尾
	unsigned char depth_{ 0 };
	unsigned char culling_mask_{ 0x01 };//控制渲染哪些layer的物体
private:
	inline static std::vector<Camera*> all_camera_;
	inline static Camera* current_camera_ = nullptr;
};

#endif