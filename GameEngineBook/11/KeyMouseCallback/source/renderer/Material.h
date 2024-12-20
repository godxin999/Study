#pragma once

#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <vector>
#include <map>

class Shader;
class Texture2D;
class Material {
public:
	void Prase(std::string material_path);

	void SetUniformMatrix4fv(std::string shader_property_name, float* pointer);
	void SetUniform1i(std::string shader_property_name, int value);

	Shader* shader() {
		return shader_;
	}
	std::vector<std::pair<std::string, Texture2D*>>& textures() {
		return textures_;
	}
private:
	Shader* shader_{ nullptr };
	std::vector<std::pair<std::string, Texture2D*>> textures_;
	std::vector<std::pair<std::string, float*>> uniform_matrix4fv_vec;
	std::vector<std::pair<std::string, int>> uniform_1i_vec;
};

#endif