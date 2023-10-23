#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <unordered_map>

class Shader {
public:
	void Prase(std::string shader_name);//����Shader������
	void CreateGPUProgram(const char* vertex_shader_text, const char* fragment_shader_text);//����Shader������GPU����

	void Active();
	void InActive();

	unsigned gl_program_id() {
		return gl_program_id_;//glCreateProgram���ص�GPU������
	}
private:
	std::string shader_name_;
	unsigned gl_program_id_{ 0 };
public:
	static Shader* Find(std::string shader_name);
private:
	inline static std::unordered_map<std::string, Shader*> kShaderMap;
};











#endif