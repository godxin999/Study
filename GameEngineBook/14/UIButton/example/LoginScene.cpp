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
#include "renderer/texture2d.h"
#include "utils/Application.h"

#include "component/Component.h"
#include "component/GameObject.h"
#include "component/Transform.h"

#include "control/Input.h"
#include "control/key_code.h"

#include "utils/Screen.h"
#include "ui/UIImage.h"
#include "ui/UIMask.h"
#include "ui/UIText.h"
#include "ui/UIButton.h"

#include "LoginScene.h"

#include <rttr/registration>

#include <string>

RTTR_REGISTRATION
{
	rttr::registration::class_<LoginScene>("LoginScene")
		.constructor<>()(rttr::policy::ctor::as_raw_ptr);
}

void LoginScene::Awake() {
	//创建主相机
	auto go_camera_1 = new GameObject("main_camera");
	//挂载Transform组件
	transform_camera_1 = dynamic_cast<Transform*>(go_camera_1->AddComponent("Transform"));
	transform_camera_1->set_position(glm::vec3(0, 0, 10));
	//挂载Camera组件
	camera_1 = dynamic_cast<Camera*>(go_camera_1->AddComponent("Camera"));
	camera_1->set_depth(0);

	last_frame_mouse_position = Input::mouse_position();
	CreateFishSoupPot();
	//CreateFont();
	CreateUI();
}

void LoginScene::Update() {
	camera_1->SetView(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	camera_1->SetPerspective(60.f, Screen::aspect_ratio(), 1.f, 1000.f);

	//按A键显示/隐藏UIMask
	if (Input::GetKeyUp(KEY_CODE_A)) {
		auto go_ui_mask = GameObject::Find("mask_mod_bag");
		go_ui_mask->SetActive(!go_ui_mask->active());
	}

	//旋转物体
	if (Input::GetKeyDown(KEY_CODE_R) || Input::GetKeyDown(KEY_CODE_RIGHT_ALT)) {
		static float rotate_eulerAngle = 0.f;
		rotate_eulerAngle += 1;
		glm::vec3 rotation = transform_fishsoup_pot->rotation();
		rotation.y = rotate_eulerAngle;
		transform_fishsoup_pot->set_rotation(rotation);
	}
	//旋转相机
	if (Input::GetKeyDown(KEY_CODE_LEFT_ALT) && Input::GetMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		float degrees = Input::mouse_position().x - last_frame_mouse_position.x;

		glm::mat4 old_mat4 = glm::mat4(1.f);

		glm::mat4 rotate_mat4 = glm::rotate(old_mat4, glm::radians(degrees), glm::vec3(0.f, 1.f, 0.f));
		glm::vec4 old_pos = glm::vec4(transform_camera_1->position(), 1.0f);
		glm::vec4 new_pos = rotate_mat4 * old_pos;

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
	go->SetLayer(0x10);
	//go->SetActive(!go->active());
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
				{{-1.0f + offset_x, 2.0f, 1.0f}, {1.0f,0.0f,0.0f,1.0f},   {character->left_top_x_, character->right_bottom_y_}},
				{{ 1.0f + offset_x, 2.0f, 1.0f}, {1.0f,0.0f,0.0f,1.0f},   {character->right_bottom_x_,character->right_bottom_y_}},
				{{ 1.0f + offset_x,  4.0f, 1.0f}, {0.0f,1.0f,0.0f,1.0f},   {character->right_bottom_x_,character->left_top_y_}},
				{{-1.0f + offset_x,  4.0f, 1.0f}, {0.0f,1.0f,0.0f,1.0f},   {character->left_top_x_,character->left_top_y_}}
		};
		std::vector<unsigned short> index_vector = {
				0,1,2,
				0,2,3
		};
		//创建GameObject
		GameObject* go = new GameObject("quad_draw_font_color");
		go->SetLayer(0x01);
		//挂载Transform组件
		auto transform = dynamic_cast<Transform*>(go->AddComponent("Transform"));
		transform->set_position({ -8.f,0.f,0.f });
		//挂载MeshFilter组件
		auto mesh_filter = dynamic_cast<MeshFilter*>(go->AddComponent("MeshFilter"));
		mesh_filter->CreateMesh(vertex_vector, index_vector);
		//创建Material
		auto material = new Material();
		material->Parse("material/quad_draw_font_color.mat");
		//挂载MeshRenderer组件
		auto mesh_renderer = dynamic_cast<MeshRenderer*>(go->AddComponent("MeshRenderer"));
		mesh_renderer->SetMaterial(material);
		//使用文字贴图
		material->SetTexture("u_diffuse_texture", font->font_texture());
	}

}

void LoginScene::CreateUI() {
	//创建UI相机
	auto go_camera_ui = new GameObject("ui_camera");
	//挂载Transform组件
	auto transform_camera_ui = dynamic_cast<Transform*>(go_camera_ui->AddComponent("Transform"));
	transform_camera_ui->set_position(glm::vec3(0, 0, 10));
	//挂载Camera组件
	auto camera_ui = dynamic_cast<Camera*>(go_camera_ui->AddComponent("Camera"));
	camera_ui->set_depth(1);
	camera_ui->set_culling_mask(0x02);
	camera_ui->set_clear_flag(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	camera_ui->SetView(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	camera_ui->SetOrthographic(-Screen::width() / 2.0, Screen::width() / 2.0, -Screen::height() / 2.0, Screen::height() / 2.0, -100, 100);

	//创建GameObject
	auto go_ui_image = new GameObject("image_mod_bag");
	go_ui_image->SetLayer(0x02);
	//挂载Transform组件
	go_ui_image->AddComponent("Transform");
	//挂载UIImage组件
	auto ui_image_mod_bag = dynamic_cast<UIImage*>(go_ui_image->AddComponent("UIImage"));
	ui_image_mod_bag->SetTexture(Texture2D::LoadFromCptFile("image/mod_bag.cpt"));

	//创建GameObject
	auto go_ui_mask = new GameObject("mask_mod_bag");
	go_ui_mask->SetLayer(0x02);
	go_ui_mask->SetParent(go_ui_image);
	//挂载Transform组件
	go_ui_mask->AddComponent("Transform");
	//挂载UIMask组件
	auto ui_mask_mod_bag = dynamic_cast<UIMask*>(go_ui_mask->AddComponent("UIMask"));
	ui_mask_mod_bag->SetTexture(Texture2D::LoadFromCptFile("image/mod_bag_mask.cpt"));

	//生成文字贴图
	/*
	Font* font = Font::LoadFromFile("font/hkyuan.ttf", 125);
	//创建GameObject
	auto go_ui_text = new GameObject("text");
	go_ui_text->SetLayer(0x02);
	//挂载Transform组件
	auto transform_ui_text= dynamic_cast<Transform*>(go_ui_text->AddComponent("Transform"));
	transform_ui_text->set_position({ 0.f,-200.f,0 });
	//挂载UIText组件
	auto ui_text=dynamic_cast<UIText*>(go_ui_text->AddComponent("UIText"));
	ui_text->SetFont(font);
	ui_text->SetText("hello world!");
	ui_text->SetColor({ 1,0,0,1 });
	*/

	//创建按钮普通状态图片
	auto go_button_image_normal = new GameObject("btn_power");
	go_button_image_normal->SetLayer(0x02);
	//挂载Transform组件
	go_button_image_normal->AddComponent("Transform");
	//挂载UIImage组件
	auto ui_image_button_image_normal = dynamic_cast<UIImage*>(go_button_image_normal->AddComponent("UIImage"));
	ui_image_button_image_normal->SetTexture(Texture2D::LoadFromCptFile("image/btn_power.cpt"));
	//创建按钮按下状态图片
	auto go_button_image_pressed = new GameObject("btn_power_pressed");
	go_button_image_pressed->SetLayer(0x02);
	//挂载Transform组件
	go_button_image_pressed->AddComponent("Transform");
	//挂载UIImage组件
	auto ui_image_button_image_pressed = dynamic_cast<UIImage*>(go_button_image_pressed->AddComponent("UIImage"));
	ui_image_button_image_pressed->SetTexture(Texture2D::LoadFromCptFile("image/btn_power_pressed.cpt"));
	//创建按钮
	auto go_ui_button = new GameObject("button");
	go_ui_button->SetLayer(0x02);
	auto transform_ui_button = dynamic_cast<Transform*>(go_ui_button->AddComponent("Transform"));
	transform_ui_button->set_position({ 100.f,-200.f,0 });
	//挂载UIButton组件
	auto ui_button = dynamic_cast<UIButton*>(go_ui_button->AddComponent("UIButton"));
	ui_button->SetImageNormal(ui_image_button_image_normal);
	ui_button->SetImagePressed(ui_image_button_image_pressed);
	ui_button->SetClickCallback([=]() {
		go_ui_mask->SetActive(!go_ui_mask->active());
		});
}