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

GLFWwindow* window;

static void error_callback(int error, const char* description) {
	std::cerr << "Error:" << description << '\n';
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Input::RecordKey(key, action);
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	Input::RecordKey(button, action);
	std::cout << "mouse_button_callback:" << button << "," << action << '\n';
}

static void mouse_move_callback(GLFWwindow* window, double x, double y) {
	Input::SetMousePosition(x, y);
	std::cout << "mouse_move_callback:" << x << "," << y << '\n';
}

static void mouse_scroll_callback(GLFWwindow* window, double x, double y) {
	Input::RecordScroll(y);
	std::cout << "mouse_scroll_callback:" << x << "," << y << '\n';
}

void init_opengl() {
	//设置错误回调函数
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


	//创建窗口
	window = glfwCreateWindow(960, 640, "OpenGLStudy", nullptr, nullptr);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);

	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, mouse_scroll_callback);
	glfwSetCursorPosCallback(window, mouse_move_callback);
}

int main() {
	Application::set_assets_path("../../../assets/");
	init_opengl();
	//创建GameObject
	GameObject* go = new GameObject("something");
	//挂载Transform组件
	auto transform = dynamic_cast<Transform*>(go->AddComponent("Transform"));
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
	auto transform_camera_1 = dynamic_cast<Transform*>(go_camera_1->AddComponent("Transform"));
	transform_camera_1->set_position(glm::vec3(0, 0, 10));
	//挂载Camera组件
	auto camera_1 = dynamic_cast<Camera*>(go_camera_1->AddComponent("Camera"));
	camera_1->set_depth(0);

	//创建相机2
	auto go_camera_2 = new GameObject("main_camera");
	//挂载Transform组件
	auto transform_camera_2 = dynamic_cast<Transform*>(go_camera_2->AddComponent("Transform"));
	transform_camera_2->set_position(glm::vec3(1, 0, 10));
	//挂载Camera组件
	auto camera_2 = dynamic_cast<Camera*>(go_camera_2->AddComponent("Camera"));
	camera_2->set_clear_flag(GL_DEPTH_BUFFER_BIT);//第二个相机不能清除之前的颜色，不然第一个相机矩阵渲染的物体就被清除了
	camera_2->set_depth(1);
	camera_2->set_culling_mask(0x02);

	vec2_ushort last_frame_mouse_position = Input::mouse_position();

	while (!glfwWindowShouldClose(window)) {
		float ratio;
		int width, height;

		//获取画面宽高
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);

		camera_1->setView(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		camera_1->setProjection(60.f, ratio, 1.f, 1000.f);

		camera_2->setView(glm::vec3(transform_camera_2->postion().x, 0, 0), glm::vec3(0, 1, 0));
		camera_2->setProjection(60.f, ratio, 1.f, 1000.f);

		//旋转物体
		if (Input::GetKeyDown(KEY_CODE_R) || Input::GetKeyDown(KEY_CODE_RIGHT_ALT)) {
			static float rotate_eulerAngle = 0.f;
			rotate_eulerAngle += 1;
			glm::vec3 rotation = transform->rotation();
			rotation.y = rotate_eulerAngle;
			transform->set_rotation(rotation);
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

		Input::Update();

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