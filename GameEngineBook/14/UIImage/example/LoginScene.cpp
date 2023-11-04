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

#include "LoginScene.h"

#include <rttr/registration>

#include <string>

RTTR_REGISTRATION
{
	rttr::registration::class_<LoginScene>("LoginScene")
		.constructor<>()(rttr::policy::ctor::as_raw_ptr);
}

void LoginScene::Awake() {
	//���������
	auto go_camera_1 = new GameObject("main_camera");
	//����Transform���
	transform_camera_1 = dynamic_cast<Transform*>(go_camera_1->AddComponent("Transform"));
	transform_camera_1->set_position(glm::vec3(0, 0, 10));
	//����Camera���
	camera_1 = dynamic_cast<Camera*>(go_camera_1->AddComponent("Camera"));
	camera_1->set_depth(0);

	last_frame_mouse_position = Input::mouse_position();
	CreateFishSoupPot();
	CreateFont();
	CreateUI();
}

void LoginScene::Update() {
	camera_1->SetView(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	camera_1->SetPerspective(60.f, Screen::aspect_ratio(), 1.f, 1000.f);

	//��ת����
	if (Input::GetKeyDown(KEY_CODE_R) || Input::GetKeyDown(KEY_CODE_RIGHT_ALT)) {
		static float rotate_eulerAngle = 0.f;
		rotate_eulerAngle += 1;
		glm::vec3 rotation = transform_fishsoup_pot->rotation();
		rotation.y = rotate_eulerAngle;
		transform_fishsoup_pot->set_rotation(rotation);
	}
	//��ת���
	if (Input::GetKeyDown(KEY_CODE_LEFT_ALT) && Input::GetMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		float degrees = Input::mouse_position().x_ - last_frame_mouse_position.x_;

		glm::mat4 old_mat4 = glm::mat4(1.f);

		glm::mat4 rotate_mat4 = glm::rotate(old_mat4, glm::radians(degrees), glm::vec3(0.f, 1.f, 0.f));
		glm::vec4 old_pos = glm::vec4(transform_camera_1->position(), 1.0f);
		glm::vec4 new_pos = rotate_mat4 * old_pos;

		transform_camera_1->set_position(glm::vec3(new_pos));
	}
	last_frame_mouse_position = Input::mouse_position();
	//�����ֿ������Զ��
	transform_camera_1->set_position(transform_camera_1->position() * (10 - Input::mouse_scroll()) / 10.f);
}

void LoginScene::CreateFishSoupPot() {
	//����GameObject
	GameObject* go = new GameObject("fishsoup_pot");
	//����Transform���
	transform_fishsoup_pot = dynamic_cast<Transform*>(go->AddComponent("Transform"));
	//����MeshFilter���
	auto mesh_filter = dynamic_cast<MeshFilter*>(go->AddComponent("MeshFilter"));
	mesh_filter->LoadMesh("model/fishsoup_pot.mesh");
	//����MeshRenderer���
	auto mesh_renderer = dynamic_cast<MeshRenderer*>(go->AddComponent("MeshRenderer"));
	auto material = new Material();
	material->Parse("material/fishsoup_pot.mat");
	mesh_renderer->SetMaterial(material);
	//go->set_layer(0x02);
}

void LoginScene::CreateFont() {
	std::string str = "Captain";
	//����������ͼ
	Font* font = Font::LoadFromFile("font/hkyuan.ttf", 100);
	auto character_vec = font->LoadStr(str);
	//����ÿ���ַ����л���
	int offset_x = 0;
	for (auto character : character_vec) {
		offset_x += 2;
		//��Ϊfreetype��bitmap�����µߵ��ģ�����UV����ҲҪ��Ӧ��ת�������Ͻ���Ϊ���
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
		//����GameObject
		GameObject* go = new GameObject("quad_draw_font_color");
		go->set_layer(0x01);
		//����Transform���
		auto transform = dynamic_cast<Transform*>(go->AddComponent("Transform"));
		transform->set_position({ -8.f,0.f,0.f });
		//����MeshFilter���
		auto mesh_filter = dynamic_cast<MeshFilter*>(go->AddComponent("MeshFilter"));
		mesh_filter->CreateMesh(vertex_vector, index_vector);
		//����Material
		auto material = new Material();
		material->Parse("material/quad_draw_font_color.mat");
		//����MeshRenderer���
		auto mesh_renderer = dynamic_cast<MeshRenderer*>(go->AddComponent("MeshRenderer"));
		mesh_renderer->SetMaterial(material);
		//ʹ��������ͼ
		material->SetTexture("u_diffuse_texture", font->font_texture());
	}

}

void LoginScene::CreateUI() {
	//����UI���
	auto go_camera_ui = new GameObject("ui_camera");
	//����Transform���
	auto transform_camera_ui = dynamic_cast<Transform*>(go_camera_ui->AddComponent("Transform"));
	transform_camera_ui->set_position(glm::vec3(0, 0, 10));
	//����Camera���
	auto camera_ui = dynamic_cast<Camera*>(go_camera_ui->AddComponent("Camera"));
	camera_ui->set_depth(1);
	camera_ui->set_culling_mask(0x02);
	camera_ui->set_clear_flag(GL_DEPTH_BUFFER_BIT);
	camera_ui->SetView(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	camera_ui->SetOrthographic(-Screen::width() / 2.0, Screen::width() / 2.0, -Screen::height() / 2.0, Screen::height() / 2.0, -100, 100);

	//����GameObject
	auto go = new GameObject("image_mod_bag");
	go->set_layer(0x02);
	//����Transform���
	go->AddComponent("Transform");
	//����UIImage���
	auto ui_image_mod_bag = dynamic_cast<UIImage*>(go->AddComponent("UIImage"));
	ui_image_mod_bag->SetTexture(Texture2D::LoadFromCptFile("image/mod_bag.cpt"));
}