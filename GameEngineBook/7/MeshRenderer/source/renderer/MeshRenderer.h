#pragma once

#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include <glm/glm.hpp>

class Material;
class MeshFilter;
class Texture2D;
class MeshRenderer {
public:
	void SetMaterial(Material* material);
	Material* material() {
		return material_;
	}
	
	void SetMeshFilter(MeshFilter* mesh_filter);
	MeshFilter* mesh_filter() {
		return mesh_filter_;
	}
	
	void SetMVP(glm::mat4 mvp);
	
	void Render();
private:
	Material* material_;
	MeshFilter* mesh_filter_;

	glm::mat4 mvp_;

	unsigned vertex_buffer_object_ = 0;
	unsigned element_buffer_object_ = 0;
	unsigned vertex_array_object_ = 0;
};








#endif