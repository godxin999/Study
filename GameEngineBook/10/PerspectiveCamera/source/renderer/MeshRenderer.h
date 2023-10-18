#pragma once

#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include <glm/glm.hpp>
#include <memory>
#include "../component/Component.h"

class Material;
class MeshFilter;
class Texture2D;
class MeshRenderer :public Component {
public:
	void SetMaterial(Material* material) {
		material_ = material;
	}
	Material* material() {
		return material_;
	}
	void SetProjection(glm::mat4 projection){
		projection_ = projection;
	}
	void SetView(glm::mat4 view) {
		view_ = view;
	}

	void Render();
private:
	Material* material_;

	glm::mat4 view_;
	glm::mat4 projection_;

	unsigned vertex_buffer_object_ = 0;
	unsigned element_buffer_object_ = 0;
	unsigned vertex_array_object_ = 0;
};








#endif