#pragma once

#ifndef MESHFILTER_H
#define MESHFILTER_H

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "component/Component.h"

class MeshFilter :public Component {
public:
	~MeshFilter();
public:
	struct Vertex {
		glm::vec3 pos_;
		glm::vec4 color_;
		glm::vec2 uv_;
	};
	struct MeshFileHeader {
		char type_[4];
		unsigned short vertex_num_;
		unsigned short vertex_index_num_;
	};
	struct Mesh {
		unsigned short vertex_num_;
		unsigned short vertex_index_num_;
		Vertex* vertex_data_;
		unsigned short* vertex_index_data_;
	};
	//从文件中加载网格数据
	void LoadMesh(std::string mesh_file_path);
	//直接从顶点和索引创建网格数据
	void CreateMesh(std::vector<Vertex>& vertex_data, std::vector<unsigned short>& vertex_index_data);
	Mesh* mesh() {
		return mesh_;
	}

private:
	Mesh* mesh_{ nullptr };
};

#endif