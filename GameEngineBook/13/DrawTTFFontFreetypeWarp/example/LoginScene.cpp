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
#include "renderer/Font.h"
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
	//创建相机1
	auto go_camera_1 = new GameObject("main_camera");
	//挂载Transform组件
	transform_camera_1 = dynamic_cast<Transform*>(go_camera_1->AddComponent("Transform"));
	transform_camera_1->set_position(glm::vec3(0, 0, 10));
	//挂载Camera组件
	camera_1 = dynamic_cast<Camera*>(go_camera_1->AddComponent("Camera"));
	camera_1->set_depth(0);

	last_frame_mouse_position = Input::mouse_position();
	CreateFishSoupPot();
	CreateFont();
}

void LoginScene::Update() {
	camera_1->setView(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	camera_1->setProjection(60.f, Screen::aspect_ratio(), 1.f, 1000.f);

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
		glm::vec4 old_pos = glm::vec4(transform_camera_1->position(), 1.0f);
		glm::vec4 new_pos = rotate_mat4 * old_pos;
		//std::cout << glm::to_string(new_pos) << '\n';

		transform_camera_1->set_position(glm::vec3(new_pos));
	}
	last_frame_mouse_position = Input::mouse_position();
	//鼠标滚轮控制相机远近
	transform_camera_1->set_position(transform_camera_1->position() * (10 - Input::mouse_scroll()) / 10.f);
}

void LoginScene::CreateFishSoupPot() {
	//创建GameObject
	GameObject* go = new GameObject("fishsoup_pot");
	//挂载Transform组件
	transform_fishsoup_pot = dynamic_cast<Transform*>(go->AddComponent("Transform"));
	//挂载MeshFilter组件
	auto mesh_filter = dynamic_cast<MeshFilter*>(go->AddComponent("MeshFilter"));
	mesh_filter->LoadMesh("model/fishsoup_pot.mesh");
	//挂载MeshRenderer组件
	auto mesh_renderer = dynamic_cast<MeshRenderer*>(go->AddComponent("MeshRenderer"));
	auto material = new Material();
	material->Parse("material/fishsoup_pot.mat");
	mesh_renderer->SetMaterial(material);
	//go->set_layer(0x02);
}

void LoginScene::CreateFont() {
	std::string str = "Captain";
	//生成文字贴图
	Font* font = Font::LoadFromFile("font/hkyuan.ttf", 100);
	auto character_vec = font->LoadStr(str);
	//遍历每个字符进行绘制
	int offset_x = 0;
	for (auto character : character_vec) {
		offset_x += 2;
		//因为freetype的bitmap是上下颠倒的，所以UV坐标也要对应翻转，将左上角作为零点
		std::vector<MeshFilter::Vertex> vertex_vector = {
				{{-1.0f + offset_x, 2.0f, 1.0f}, {1.0f,1.0f,1.0f,1.0f},   {character->left_top_x_, character->right_bottom_y_}},
				{{ 1.0f + offset_x, 2.0f, 1.0f}, {1.0f,1.0f,1.0f,1.0f},   {character->right_bottom_x_,character->right_bottom_y_}},
				{{ 1.0f + offset_x,  4.0f, 1.0f}, {1.0f,1.0f,1.0f,1.0f},   {character->right_bottom_x_,character->left_top_y_}},
				{{-1.0f + offset_x,  4.0f, 1.0f}, {1.0f,1.0f,1.0f,1.0f},   {character->left_top_x_,character->left_top_y_}}
		};
		std::vector<unsigned short> index_vector = {
				0,1,2,
				0,2,3
		};
		//创建GameObject
		GameObject* go = new GameObject("quad_draw_font");
		go->set_layer(0x01);
		//挂载Transform组件
		auto transform = dynamic_cast<Transform*>(go->AddComponent("Transform"));
		transform->set_position({ -8.f,0.f,0.f });
		//挂载MeshFilter组件
		auto mesh_filter = dynamic_cast<MeshFilter*>(go->AddComponent("MeshFilter"));
		mesh_filter->CreateMesh(vertex_vector, index_vector);
		//创建Material
		auto material = new Material();
		material->Parse("material/quad_draw_font.mat");
		//挂载MeshRenderer组件
		auto mesh_renderer = dynamic_cast<MeshRenderer*>(go->AddComponent("MeshRenderer"));
		mesh_renderer->SetMaterial(material);
		//使用文字贴图
		material->SetTexture("u_diffuse_texture", font->font_texture());
	}

}
