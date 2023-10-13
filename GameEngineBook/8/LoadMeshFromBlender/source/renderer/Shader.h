#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <unordered_map>

class Shader {
public:
	void Prase(std::string shader_name);//加载Shader并解析
	void CreateGPUProgram(const char* vertex_shader_text, const char* fragment_shader_text);//编译Shader，创建GPU程序

	unsigned gl_program_id() {
		return gl_program_id_;//glCreateProgram返回的GPU程序句柄
	}
private:
	std::string shader_name_;
	unsigned gl_program_id_;
public:
	static Shader* Find(std::string shader_name);
private:
	inline static std::unordered_map<std::string, Shader*> kShaderMap;
};











#endif