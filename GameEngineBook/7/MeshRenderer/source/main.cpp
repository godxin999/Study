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
#include "renderer/texture2d.h"
#include "renderer/Material.h"
#include "renderer/MeshRenderer.h"
#include "utils/Application.h"



static void error_callback(int error, const char* description) {
	//fprintf(stderr, "Error:%s\n", description);
	std::cerr << "Error:" << description << std::endl;
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

	MeshFilter* mesh_filter = new MeshFilter();
	mesh_filter->LoadMesh("model/cube.mesh");

	Material* material = new Material();
	material->Prase("material/cube.mat");

	MeshRenderer* mesh_renderer = new MeshRenderer();
	mesh_renderer->SetMaterial(material);
	mesh_renderer->SetMeshFilter(mesh_filter);

	while (!glfwWindowShouldClose(window)) {
		float ratio;
		int width, height;

		//获取画面宽高
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;

		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(49.f / 255, 77.f / 255, 121.f / 255, 1.f);

		//坐标变换
		glm::mat4 trans = glm::translate(glm::vec3(0, 0, 0));//不移动顶点坐标
		static float rotate_eulerAngle = 0.f;
		rotate_eulerAngle += 1;
		glm::mat4 ratation = glm::eulerAngleYXZ(glm::radians(rotate_eulerAngle), glm::radians(rotate_eulerAngle), glm::radians(rotate_eulerAngle));//欧拉角
		glm::mat4 scale = glm::scale(glm::vec3(3.f, 3.f, 3.f));

		glm::mat4 model = trans * scale * ratation;
		glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		glm::mat4 projection = glm::perspective(glm::radians(60.f), ratio, 1.f, 1000.f);

		glm::mat4 mvp = projection * view * model;

		mesh_renderer->SetMVP(mvp);
		mesh_renderer->Render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	exit(EXIT_SUCCESS);

	return 0;
}