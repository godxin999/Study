#pragma once

#ifndef LOGINSCENE_H
#define LOGINSCENE_H

#include "component/Component.h"
#include "control/Input.h"

class Transform;
class Camera;
class LoginScene :public Component {
public:
	void Awake()override;
	void Update()override;
private:
	Transform* transform_fishsoup_pot{ nullptr };
	Transform* transform_camera_1{ nullptr };
	Transform* transform_camera_2{ nullptr };
	Camera* camera_1{ nullptr };
	Camera* camera_2{ nullptr };
	vec2_ushort last_frame_mouse_position;
};


#endif