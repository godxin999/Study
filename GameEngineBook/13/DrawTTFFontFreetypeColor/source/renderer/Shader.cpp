#include "Shader.h"
#include "../utils/Application.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <glad/glad.h>

Shader* Shader::Find(std::string shader_name) {
	if (auto it = kShaderMap.find(shader_name); it != kShaderMap.end()) {
		return it->second;
	}

	Shader* shader = new Shader();
	shader->Prase(shader_name);
	kShaderMap.emplace(shader_name, shader);

	return shader;
}

void Shader::Prase(std::string shader_name) {
	shader_name_ = shader_name;

	std::string vertex_shader_file_path = Application::assets_path() + shader_name + ".vs";
	std::string fragment_shader_file_path = Application::assets_path() + shader_name + ".fs";
	
	//使用istreambuf_iterator<char>读取文件内容，默认构造的istreambuf_iterator<char>标志文件结束符，所以以下代码的作用是读取文件中全部字符
	std::ifstream vertex_shader_input_file_stream(vertex_shader_file_path);
	std::string vertex_shader_source((std::istreambuf_iterator<char>(vertex_shader_input_file_stream)), std::istreambuf_iterator<char>());

	std::ifstream fragment_shader_input_file_stream(fragment_shader_file_path);
	std::string fragment_shader_source((std::istreambuf_iterator<char>(fragment_shader_input_file_stream)), std::istreambuf_iterator<char>());
	

	CreateGPUProgram(vertex_shader_source.c_str(), fragment_shader_source.c_str());
}

void Shader::CreateGPUProgram(const char* vertex_shader_text, const char* fragment_shader_text) {
	//创建顶点着色器
	unsigned vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	//指定shader源码
	glShaderSource(vertex_shader, 1, &vertex_shader_text, nullptr);
	//编译shader
	glCompileShader(vertex_shader);
	//获取编译结果
	GLint compile_status = GL_FALSE;
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compile_status);
	if (compile_status == GL_FALSE) {
		GLchar message[256];
		glGetShaderInfoLog(vertex_shader, sizeof(message), 0, message);
		std::cout << "compile vs error:" << message << '\n';
	}

	//创建片段着色器
	unsigned fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	//指定shader源码
	glShaderSource(fragment_shader, 1, &fragment_shader_text, nullptr);
	//编译shader
	glCompileShader(fragment_shader);
	//获取编译结果
	compile_status = GL_FALSE;
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compile_status);
	if (compile_status == GL_FALSE) {
		GLchar message[256];
		glGetShaderInfoLog(fragment_shader, sizeof(message), 0, message);
		std::cout << "compile fs error:" << message << '\n';
	}

	//创建GPU程序
	gl_program_id_ = glCreateProgram();
	//附加shader
	glAttachShader(gl_program_id_, vertex_shader);
	glAttachShader(gl_program_id_, fragment_shader);
	//链接
	glLinkProgram(gl_program_id_);
	//获取编译结果
	GLint link_status = GL_FALSE;
	glGetProgramiv(gl_program_id_, GL_LINK_STATUS, &link_status);
	if (link_status == GL_FALSE) {
		GLchar message[256];
		glGetProgramInfoLog(gl_program_id_, sizeof(message), 0, message);
		std::cout << "link error:" << message << '\n';
	}
}

void Shader::Active() {
	glUseProgram(gl_program_id_);
}

void Shader::InActive() {

}