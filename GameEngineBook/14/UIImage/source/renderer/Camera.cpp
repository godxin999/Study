#include "Camera.h"
#include <rttr/registration>
#include <algorithm>
#include <functional>
#include <glm/gtc/matrix_transform.hpp>
#include "../component/GameObject.h"
#include "../component/Transform.h"

RTTR_REGISTRATION
{
	rttr::registration::class_<Camera>("Camera")
		.constructor<>()(rttr::policy::ctor::as_raw_ptr);
}

Camera::Camera() {
	//默认获取现有Camera的最大depth，并设置当前Camera的depth为最大depth+1
	if (all_camera_.size() > 0) {
		unsigned char max_depth = all_camera_.back()->depth();
		depth_ = max_depth + 1;
	}
	all_camera_.push_back(this);
}

Camera::~Camera() {
	if (auto it = std::ranges::find(all_camera_, this); it != all_camera_.end()) {
		all_camera_.erase(it);
	}
}

void Camera::SetView(const glm::vec3& cameraForward, const glm::vec3& cameraUp) {
	auto transform = dynamic_cast<Transform*>(game_object()->GetComponent("Transform"));
	view_mat4_ = glm::lookAt(transform->position(), cameraForward, cameraUp);
}

void Camera::SetPerspective(float fovDegrees, float aspectRatio, float nearClip, float farClip) {
	projection_mat4_ = glm::perspective(glm::radians(fovDegrees), aspectRatio, nearClip, farClip);
}
#include <glm/gtx/string_cast.hpp>
void Camera::SetOrthographic(float left, float right, float bottom, float top, float z_near, float z_far) {
	projection_mat4_ = glm::ortho(left, right, bottom, top, z_near, z_far);
}

void Camera::Clear() {
	glClear(clear_flag_);
	glClearColor(clear_color_.r, clear_color_.g, clear_color_.b, clear_color_.a);
}

void Camera::Foreach(std::function<void()> func) {
	for (auto& camera : all_camera_) {
		current_camera_ = camera;
		current_camera_->Clear();
		func();
	}
}

void Camera::Sort() {
	//按深度值排序
	std::ranges::sort(all_camera_, std::less<unsigned char>{}, & Camera::depth);
}