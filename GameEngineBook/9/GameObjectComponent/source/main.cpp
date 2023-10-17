/*************************************************************************
	> File Name: main.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/25 22:30:06
 ************************************************************************/

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "renderer/MeshFilter.h"
#include "renderer/Shader.h"
#include "renderer/Material.h"
#include "renderer/MeshRenderer.h"
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
	gladLoadGL(glfwGetProcAddress);
	glfwSwapInterval(1);
}

int main() {
	Application::set_assets_path("../../../assets/");
	init_opengl();

	GameObject* go = new GameObject("something");

	//挂载Transform组件
	auto transform =go->AddComponent<Transform>();
	//挂载MeshFilter组件
	auto mesh_filter = go->AddComponent<MeshFilter>();
	mesh_filter->LoadMesh("model/fishsoup_pot.mesh");
	//挂载MeshRenderer组件
	auto mesh_renderer=go->AddComponent<MeshRenderer>();
	Material* material = new Material();
	material->Prase("material/fishsoup_pot.mat");
	mesh_renderer->SetMaterial(material);

	while (!glfwWindowShouldClose(window)) {
		float ratio;
		int width, height;

		//获取画面宽高
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;

		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(49.f / 255, 77.f / 255, 121.f / 255, 1.f);

		//旋转物体
		static float rotate_eulerAngle = 0.f;
		rotate_eulerAngle += 1;
		glm::vec3 rotation = transform->rotation();
		rotation.y = rotate_eulerAngle;
		transform->set_rotation(rotation);
		
		glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		glm::mat4 projection = glm::perspective(glm::radians(60.f), ratio, 1.f, 1000.f);

		mesh_renderer->SetView(view);
		mesh_renderer->SetProjection(projection);
		mesh_renderer->Render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	exit(EXIT_SUCCESS);

	return 0;
}