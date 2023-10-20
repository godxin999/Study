#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/scalar_multiplication.hpp>

#include "renderer/MeshFilter.h"
#include "renderer/Shader.h"
#include "renderer/Material.h"
#include "renderer/MeshRenderer.h"
#include "renderer/Camera.h"
#include "utils/Application.h"

#include "component/Component.h"
#include "component/GameObject.h"
#include "component/Transform.h"

#include "control/Input.h"
#include "control/key_code.h"

#include "utils/Screen.h"

#include "LoginScene.h"

#include <rttr/registration>

RTTR_REGISTRATION
{
	rttr::registration::class_<LoginScene>("LoginScene")
		.constructor<>()(rttr::policy::ctor::as_raw_ptr);
}

void LoginScene::Awake() {
	//创建GameObject
	GameObject* go = new GameObject("fishsoup_pot");
	//挂载Transform组件
	transform_fishsoup_pot = dynamic_cast<Transform*>(go->AddComponent("Transform"));
	//挂载MeshFilter组件
	auto mesh_filter = dynamic_cast<MeshFilter*>(go->AddComponent("MeshFilter"));
	mesh_filter->LoadMesh("model/fishsoup_pot.mesh");
	//挂载MeshRenderer组件
	auto mesh_renderer = dynamic_cast<MeshRenderer*>(go->AddComponent("MeshRenderer"));
	Material* material = new Material();
	material->Prase("material/fishsoup_pot.mat");
	mesh_renderer->SetMaterial(material);

	//创建相机1
	auto go_camera_1 = new GameObject("main_camera");
	//挂载Transform组件
	transform_camera_1 = dynamic_cast<Transform*>(go_camera_1->AddComponent("Transform"));
	transform_camera_1->set_position(glm::vec3(0, 0, 10));
	//挂载Camera组件
	camera_1 = dynamic_cast<Camera*>(go_camera_1->AddComponent("Camera"));
	camera_1->set_depth(0);

	//创建相机2
	auto go_camera_2 = new GameObject("main_camera");
	//挂载Transform组件
	transform_camera_2 = dynamic_cast<Transform*>(go_camera_2->AddComponent("Transform"));
	transform_camera_2->set_position(glm::vec3(1, 0, 10));
	//挂载Camera组件
	camera_2 = dynamic_cast<Camera*>(go_camera_2->AddComponent("Camera"));
	camera_2->set_clear_flag(GL_DEPTH_BUFFER_BIT);//第二个相机不能清除之前的颜色，不然第一个相机矩阵渲染的物体就被清除了
	camera_2->set_depth(1);
	camera_2->set_culling_mask(0x02);

	last_frame_mouse_position = Input::mouse_position();
}

void LoginScene::Update() {
	camera_1->setView(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	camera_1->setProjection(60.f, Screen::aspect_ratio(), 1.f, 1000.f);

	camera_2->setView(glm::vec3(transform_camera_2->postion().x, 0, 0), glm::vec3(0, 1, 0));
	camera_2->setProjection(60.f, Screen::aspect_ratio(), 1.f, 1000.f);

	//旋转物体
	if (Input::GetKeyDown(KEY_CODE_R) || Input::GetKeyDown(KEY_CODE_RIGHT_ALT)) {
		static float rotate_eulerAngle = 0.f;
		rotate_eulerAngle += 1;
		glm::vec3 rotation = transform_fishsoup_pot->rotation();
		rotation.y = rotate_eulerAngle;
		transform_fishsoup_pot->set_rotation(rotation);
	}
	//std::cout << glm::to_string(transform_camera_1->postion()) << '\n';
	//旋转相机
	if (Input::GetKeyDown(KEY_CODE_LEFT_ALT) && Input::GetMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		float degrees = Input::mouse_position().x_ - last_frame_mouse_position.x_;

		glm::mat4 old_mat4 = glm::mat4(1.f);
		//std::cout << glm::to_string(old_mat4) << '\n';

		glm::mat4 rotate_mat4 = glm::rotate(old_mat4, glm::radians(degrees), glm::vec3(0.f, 1.f, 0.f));
		glm::vec4 old_pos = glm::vec4(transform_camera_1->postion(), 1.0f);
		glm::vec4 new_pos = rotate_mat4 * old_pos;
		//std::cout << glm::to_string(new_pos) << '\n';

		transform_camera_1->set_position(glm::vec3(new_pos));
	}
	last_frame_mouse_position = Input::mouse_position();
	//鼠标滚轮控制相机远近
	transform_camera_1->set_position(transform_camera_1->postion() * (10 - Input::mouse_scroll()) / 10.f);
}