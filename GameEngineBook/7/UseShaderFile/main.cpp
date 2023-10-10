/*************************************************************************
	> File Name: main.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/25 22:30:06
 ************************************************************************/

#include <iostream>
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <stdlib.h>
#include <stdio.h>
#include "renderer/MeshFilter.h"
#include "renderer/Shader.h"
#include "ShaderSource.h"
#include "texture2d.h"


static void error_callback(int error, const char* description) {
	fprintf(stderr, "Error:%s\n", description);
}

GLFWwindow* window;
GLint mvp_location, vpos_location, vcol_location, u_diffuse_texture_location, a_uv_location;
Texture2D* texture2d = nullptr;
MeshFilter* mesh_filter = nullptr;
GLuint kVBO, kIBO;
GLuint kVAO;


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
	window = glfwCreateWindow(960, 640, "GameEngineStudy", nullptr, nullptr);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	gladLoadGL(glfwGetProcAddress);
	glfwSwapInterval(1);
}


void CreateTexture(std::string image_file_path) {
	texture2d = Texture2D::LoadFromCptFile(image_file_path);
}

void GeneratorVertexArrayObject() {
	glGenVertexArrays(1, &kVAO);
}

void GenerateBufferObject() {
	//��������������
	glGenBuffers(1, &kVBO);
	//ָ��Ϊ���㻺��������
	glBindBuffer(GL_ARRAY_BUFFER, kVBO);
	//�ϴ��������ݵ�����������
	glBufferData(GL_ARRAY_BUFFER, mesh_filter->mesh()->vertex_num*sizeof(MeshFilter::Vertex),mesh_filter->mesh()->vertex_data, GL_STATIC_DRAW);
	
	//��������������
	glGenBuffers(1, &kIBO);
	//ָ��Ϊ��������������
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, kIBO);
	//�ϴ��������ݵ�����������
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh_filter->mesh()->vertex_index_num*sizeof(unsigned short),mesh_filter->mesh()->vertex_index_data, GL_STATIC_DRAW);
	
	//����VAO
	glBindVertexArray(kVAO);
	{
		//ָ����ǰʹ�õ�VBO
		glBindBuffer(GL_ARRAY_BUFFER, kVBO);
		//��λ�������붥����ɫ������(a_pos)���й���
		glVertexAttribPointer(vpos_location, 3, GL_FLOAT, false, sizeof(MeshFilter::Vertex), 0);
		//����ɫ�����붥����ɫ������(a_color)���й���
		glVertexAttribPointer(vcol_location, 4, GL_FLOAT, false, sizeof(MeshFilter::Vertex), (void*)(sizeof(float) * 3));
		//���������������붥����ɫ������(a_uv)���й���
		glVertexAttribPointer(a_uv_location, 2, GL_FLOAT, false, sizeof(MeshFilter::Vertex), (void*)(sizeof(float) * (3 + 4)));

		glEnableVertexAttribArray(vpos_location);
		glEnableVertexAttribArray(vcol_location);
		glEnableVertexAttribArray(a_uv_location);

		//ָ����ǰʹ�õ�IBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, kIBO);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);//�����������
}

int main() {
	init_opengl();
	
	mesh_filter = new MeshFilter();
	mesh_filter->LoadMesh("../../../assets/cube.mesh");

	CreateTexture("../../../assets/urban.cpt");

	Shader* shader = Shader::Find("../../../data/shader/unlit");

	//��ȡshader������ID
	mvp_location = glGetUniformLocation(shader->gl_program_id(), "u_mvp");
	vpos_location = glGetAttribLocation(shader->gl_program_id(), "a_pos");
	vcol_location = glGetAttribLocation(shader->gl_program_id(), "a_color");
	a_uv_location = glGetAttribLocation(shader->gl_program_id(), "a_uv");
	u_diffuse_texture_location = glGetUniformLocation(shader->gl_program_id(), "u_diffuse_texture");

	GeneratorVertexArrayObject();
	GenerateBufferObject();

	while (!glfwWindowShouldClose(window)) {
		float ratio;
		int width, height;
		glm::mat4 m, v, p, mvp;

		//��ȡ�������
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;

		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(49.f / 255, 77.f / 255, 121.f / 255, 1.f);

		//����任
		glm::mat4 trans = glm::translate(glm::vec3(0, 0, 0));//���ƶ���������
		static float rotate_eulerAngle = 0.f;
		rotate_eulerAngle += 1;
		glm::mat4 ratation = glm::eulerAngleYXZ(glm::radians(rotate_eulerAngle), glm::radians(rotate_eulerAngle), glm::radians(rotate_eulerAngle));//ŷ����
		glm::mat4 scale = glm::scale(glm::vec3(3.f, 3.f, 3.f));

		m = trans * scale * ratation;
		v = glm::lookAt(glm::vec3(0, 0, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		p = glm::perspective(glm::radians(60.f), ratio, 1.f, 1000.f);

		mvp = p * v * m;

		//ָ��GPU����
		glUseProgram(shader->gl_program_id());
		{
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);//���������޳�

			//�ϴ�mvp����
			glUniformMatrix4fv(mvp_location, 1, GL_FALSE, &mvp[0][0]);

			//��ͼ����
			glActiveTexture(GL_TEXTURE0);//����������Ԫ0
			glBindTexture(GL_TEXTURE_2D, texture2d->gl_texture_id);//�����ص���������󶨵�������Ԫ0
			glUniform1i(u_diffuse_texture_location, 0);//����Shader�����������Ԫ0�ж�������

			glBindVertexArray(kVAO);
			{
				glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
			}
			glBindVertexArray(0);//�����������İ�
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	exit(EXIT_SUCCESS);

	return 0;
}