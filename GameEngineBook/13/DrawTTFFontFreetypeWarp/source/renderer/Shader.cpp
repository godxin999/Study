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
	
	//ʹ��istreambuf_iterator<char>��ȡ�ļ����ݣ�Ĭ�Ϲ����istreambuf_iterator<char>��־�ļ����������������´���������Ƕ�ȡ�ļ���ȫ���ַ�
	std::ifstream vertex_shader_input_file_stream(vertex_shader_file_path);
	std::string vertex_shader_source((std::istreambuf_iterator<char>(vertex_shader_input_file_stream)), std::istreambuf_iterator<char>());

	std::ifstream fragment_shader_input_file_stream(fragment_shader_file_path);
	std::string fragment_shader_source((std::istreambuf_iterator<char>(fragment_shader_input_file_stream)), std::istreambuf_iterator<char>());
	

	CreateGPUProgram(vertex_shader_source.c_str(), fragment_shader_source.c_str());
}

void Shader::CreateGPUProgram(const char* vertex_shader_text, const char* fragment_shader_text) {
	//����������ɫ��
	unsigned vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	//ָ��shaderԴ��
	glShaderSource(vertex_shader, 1, &vertex_shader_text, nullptr);
	//����shader
	glCompileShader(vertex_shader);
	//��ȡ������
	GLint compile_status = GL_FALSE;
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compile_status);
	if (compile_status == GL_FALSE) {
		GLchar message[256];
		glGetShaderInfoLog(vertex_shader, sizeof(message), 0, message);
		std::cout << "compile vs error:" << message << '\n';
	}

	//����Ƭ����ɫ��
	unsigned fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	//ָ��shaderԴ��
	glShaderSource(fragment_shader, 1, &fragment_shader_text, nullptr);
	//����shader
	glCompileShader(fragment_shader);
	//��ȡ������
	compile_status = GL_FALSE;
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compile_status);
	if (compile_status == GL_FALSE) {
		GLchar message[256];
		glGetShaderInfoLog(fragment_shader, sizeof(message), 0, message);
		std::cout << "compile fs error:" << message << '\n';
	}

	//����GPU����
	gl_program_id_ = glCreateProgram();
	//����shader
	glAttachShader(gl_program_id_, vertex_shader);
	glAttachShader(gl_program_id_, fragment_shader);
	//����
	glLinkProgram(gl_program_id_);
	//��ȡ������
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