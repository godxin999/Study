#include "Application.h"
#include <memory>
#include <algorithm>
#include "component/GameObject.h"
#include "renderer/Camera.h"
#include "renderer/MeshRenderer.h"
#include "control/Input.h"
#include "Screen.h"
#include "Debug.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

static void error_callback(int error, const char* description) {
	std::cerr << "Error:" << description << '\n';
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Input::RecordKey(key, action);
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	Input::RecordKey(button, action);
	//std::cout << "mouse_button_callback:" << button << "," << action << '\n';
}

static void mouse_move_callback(GLFWwindow* window, double x, double y) {
	Input::SetMousePosition(x, y);
	//std::cout << "mouse_move_callback:" << x << "," << y << '\n';
}

static void mouse_scroll_callback(GLFWwindow* window, double x, double y) {
	Input::RecordScroll(y);
	//std::cout << "mouse_scroll_callback:" << x << "," << y << '\n';
}

void Application::Init() {
	Debug::Init();
	DEBUG_LOG_INFO("game start");
	//设置错误回调函数
	glfwSetErrorCallback(error_callback);

	if (!glfwInit()) {
		DEBUG_LOG_ERROR("glfw init failed!");
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT.GL_TRUE);
#endif // __APPLE__


	//创建窗口
	glfw_window_ = glfwCreateWindow(960, 640, "OpenGLStudy", nullptr, nullptr);
	if (!glfw_window_) {
		DEBUG_LOG_ERROR("glfwCreateWindow error!");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(glfw_window_);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);

	glfwSetKeyCallback(glfw_window_, key_callback);
	glfwSetMouseButtonCallback(glfw_window_, mouse_button_callback);
	glfwSetScrollCallback(glfw_window_, mouse_scroll_callback);
	glfwSetCursorPosCallback(glfw_window_, mouse_move_callback);
}

void Application::Update() {
	UpdateScreenSize();

	GameObject::Foreach([](GameObject* game_object) {
		game_object->ForeachComponent([](Component* component) {
			component->Update();
			});
		});

	Input::Update();
}

void Application::Render() {
	//遍历所有相机，每个具有MeshRenderer组件的GameObject都会被渲染(根据mask)
	Camera::Foreach([]() {
		GameObject::Foreach([](GameObject* game_object) {
			auto component = game_object->GetComponent("MeshRenderer");
			if (!component)return;
			auto mesh_renderer = dynamic_cast<MeshRenderer*>(component);
			if (!mesh_renderer)return;
			mesh_renderer->Render();
			});
		});
}

void Application::Run() {
	while (!glfwWindowShouldClose(glfw_window_)) {
		Update();
		Render();

		glfwSwapBuffers(glfw_window_);
		glfwPollEvents();
	}

	glfwDestroyWindow(glfw_window_);

	glfwTerminate();
	exit(EXIT_SUCCESS);
}

void Application::UpdateScreenSize() {
	int width, height;
	glfwGetFramebufferSize(glfw_window_, &width, &height);
	glViewport(0, 0, width, height);
	Screen::set_width_height(width, height);
}