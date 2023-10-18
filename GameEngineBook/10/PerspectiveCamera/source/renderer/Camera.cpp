#include "Camera.h"
#include <rttr/registration>
#include <glm/gtc/matrix_transform.hpp>
#include "../component/GameObject.h"
#include "../component/Transform.h"

RTTR_REGISTRATION
{
	rttr::registration::class_<Camera>("Camera")
		.constructor<>()(rttr::policy::ctor::as_raw_ptr);
}

void Camera::setView(const glm::vec3& cameraForward, const glm::vec3& cameraUp) {
	auto transform = dynamic_cast<Transform*>(game_object()->GetComponent("Transform"));
	view_mat4_ = glm::lookAt(transform->postion(), cameraForward, cameraUp);
}

void Camera::setProjection(float fovDegrees, float aspectRatio, float nearClip, float farClip) {
	projection_mat4_ = glm::perspective(glm::radians(fovDegrees), aspectRatio, nearClip, farClip);
}

void Camera::clear() {
	glClear(clear_flag_);
	glClearColor(clear_color_.r, clear_color_.g, clear_color_.b, clear_color_.a);
}