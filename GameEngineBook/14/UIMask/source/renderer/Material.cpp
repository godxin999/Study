#include "Material.h"
#include <iostream>
#include <fstream>
#include <glad/glad.h>
#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_print.hpp>
#include <rapidxml/rapidxml_utils.hpp>
#include "Shader.h"
#include "texture2d.h"
#include "utils/Application.h"

void Material::Parse(std::string material_path) {
	rapidxml::file<> xml_file((Application::assets_path() + material_path).c_str());//�����ļ�·������XML�ļ�
	rapidxml::xml_document<> document;//����һ��document�������ڴ洢�������XML�ĵ�
	document.parse<0>(xml_file.data());//����XML�ļ�

	rapidxml::xml_node<>* material_node = document.first_node("material");//������Ϊmaterial�ĵ�һ���ڵ�
	if (!material_node)return;

	rapidxml::xml_attribute<>* material_shader_attribute = material_node->first_attribute("shader");//��material�ڵ������Ϊshader������
	if (!material_shader_attribute)return;

	shader_ = Shader::Find(material_shader_attribute->value());


	rapidxml::xml_node<>* material_texture_node = material_node->first_node("texture");//��material�ڵ������Ϊtexture�ĵ�һ���ڵ�
	while (material_texture_node) {
		rapidxml::xml_attribute<>* texture_name_attribute = material_texture_node->first_attribute("name");
		if (!texture_name_attribute)return;
		rapidxml::xml_attribute<>* texture_image_attribute = material_texture_node->first_attribute("image");
		if (!texture_image_attribute)return;

		std::string shader_property_name = texture_name_attribute->value();
		std::string image_path = texture_image_attribute->value();
		textures_.emplace_back(texture_name_attribute->value(), Texture2D::LoadFromCptFile(image_path));

		material_texture_node = material_texture_node->next_sibling("texture");//��material�ڵ������һ����Ϊtexture���ֵܽڵ�
	}
}

void Material::SetUniformMatrix4fv(std::string shader_property_name, float* pointer) {
	uniform_matrix4fv_vec.emplace_back(shader_property_name, pointer);
}

void Material::SetUniform1i(std::string shader_property_name, int value) {
	uniform_1i_vec.emplace_back(shader_property_name, value);
}

void Material::SetTexture(std::string property, Texture2D* texture2d) {
	for (auto& [name, texture] : textures_) {
		if (name == property) {
			delete texture;
			texture = texture2d;
			break;
		}
	}
}