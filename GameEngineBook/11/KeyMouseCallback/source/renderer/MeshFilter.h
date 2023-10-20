#pragma once

#ifndef MESHFILTER_H
#define MESHFILTER_H

#include <glm/glm.hpp>
#include <string>
#include "../component/Component.h"

class MeshFilter :public Component {
public:
	~MeshFilter();
public:
	struct Vertex {
		glm::vec3 pos;
		glm::vec4 color;
		glm::vec2 uv;
	};
	struct MeshFileHeader {
		char type[4];
		unsigned short vertex_num;
		unsigned short vertex_index_num;
	};
	struct Mesh {
		unsigned short vertex_num;
		unsigned short vertex_index_num;
		Vertex* vertex_data;
		unsigned short* vertex_index_data;
	};
	void LoadMesh(std::string mesh_file_path);

	Mesh* mesh() {
		return mesh_;
	}

private:
	Mesh* mesh_{ nullptr };
};










#endif