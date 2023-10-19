/*************************************************************************
	> File Name: main.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/25 22:30:06
 ************************************************************************/

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "renderer/MeshFilter.h"
#include "renderer/Shader.h"
#include "renderer/Material.h"
#include "renderer/MeshRenderer.h"
#include "renderer/Camera.h"
#include "utils/Application.h"

#include "component/Component.h"
#include "component/GameObject.h"
#include "component/Transform.h"

static void error_callback(int error, const char* description) {
	//fprintf(stderr, "Error:%s\n", description);
	std::cerr << "Error:" << description << '\n';
}

GLFWwindow* window;

void init_opengl() {
	//���ô���ص�����
	glfwSetErrorCallback(error_callback);

	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT.GL_TRUE);
#endif // __APPLE__


	//��������
	window = glfwCreateWindow(960, 640, "OpenGLStudy", nullptr, nullptr);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);
}

int main() {
	Application::set_assets_path("../../../assets/");
	init_opengl();
	//����GameObject
	GameObject* go = new GameObject("something");
	//����Transform���
	auto transform = dynamic_cast<Transform*>(go->AddComponent("Transform"));
	//����MeshFilter���
	auto mesh_filter = dynamic_cast<MeshFilter*>(go->AddComponent("MeshFilter"));
	mesh_filter->LoadMesh("model/fishsoup_pot.mesh");
	//����MeshRenderer���
	auto mesh_renderer = dynamic_cast<MeshRenderer*>(go->AddComponent("MeshRenderer"));
	Material* material = new Material();
	material->Prase("material/fishsoup_pot.mat");
	mesh_renderer->SetMaterial(material);

	//�������1
	auto go_camera_1 = new GameObject("main_camera");
	//����Transform���
	auto transform_camera_1 = dynamic_cast<Transform*>(go_camera_1->AddComponent("Transform"));
	transform_camera_1->set_position(glm::vec3(0, 0, 10));
	//����Camera���
	auto camera_1 = dynamic_cast<Camera*>(go_camera_1->AddComponent("Camera"));
	camera_1->set_depth(0);

	//�������2
	auto go_camera_2 = new GameObject("main_camera");
	//����Transform���
	auto transform_camera_2 = dynamic_cast<Transform*>(go_camera_2->AddComponent("Transform"));
	transform_camera_2->set_position(glm::vec3(1, 0, 10));
	//����Camera���
	auto camera_2 = dynamic_cast<Camera*>(go_camera_2->AddComponent("Camera"));
	camera_2->set_clear_flag(GL_DEPTH_BUFFER_BIT);//�ڶ�������������֮ǰ����ɫ����Ȼ��һ�����������Ⱦ������ͱ������
	camera_2->set_depth(1);
	
	while (!glfwWindowShouldClose(window)) {
		float ratio;
		int width, height;

		//��ȡ������
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);

		camera_1->setView(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		camera_1->setProjection(60.f, ratio, 1.f, 1000.f);

		camera_2->setView(glm::vec3(transform_camera_2->postion().x, 0, 0), glm::vec3(0, 1, 0));
		camera_2->setProjection(60.f, ratio, 1.f, 1000.f);

		//��ת����
		static float rotate_eulerAngle = 0.f;
		rotate_eulerAngle += 1;
		glm::vec3 rotation = transform->rotation();
		rotation.y = rotate_eulerAngle;
		transform->set_rotation(rotation);

		Camera::Foreach([&]() {
			mesh_renderer->Render();
		});

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	exit(EXIT_SUCCESS);

	return 0;
}