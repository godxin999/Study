/*************************************************************************
	> File Name: VertexData.h
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/25 22:30:17
 ************************************************************************/

#pragma once

#ifndef VERTEXDATA_H
#define VERTEXDATA_H

#include <glm/glm.hpp>
#include <algorithm>
#include <fstream>
#include <vector>

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

static const Vertex kVertices[36] = {
	//Front
	glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec4(1, 1, 1, 1),glm::vec2(0.0f, 0.0f),
	glm::vec3(1.0f, -1.0f, 1.0f), glm::vec4(1, 1, 1, 1),glm::vec2(1.0f, 0.0f),
	glm::vec3(1.0f, 1.0f, 1.0f),glm::vec4(1, 1, 1, 1),glm::vec2(1.0f, 1.0f),

	glm::vec3(-1.0f, -1.0f, 1.0f),glm::vec4(1, 1, 1, 1),glm::vec2(0.0f, 0.0f),
	glm::vec3(1.0f, 1.0f, 1.0f), glm::vec4(1, 1, 1, 1),glm::vec2(1.0f, 1.0f),
	glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec4(1, 1, 1, 1),glm::vec2(0.0f, 1.0f),

	//back
	glm::vec3(1.0f, -1.0f, -1.0f),glm::vec4(1, 1, 1, 1),glm::vec2(0.0f, 0.0f),
	glm::vec3(-1.0f, -1.0f, -1.0f),glm::vec4(1, 1, 1, 1),glm::vec2(1.0f, 0.0f),
	glm::vec3(-1.0f, 1.0f, -1.0f),glm::vec4(1, 1, 1, 1),glm::vec2(1.0f, 1.0f),

	glm::vec3(1.0f, -1.0f, -1.0f),glm::vec4(1, 1, 1, 1),glm::vec2(0.0f, 0.0f),
	glm::vec3(-1.0f, 1.0f, -1.0f),glm::vec4(1, 1, 1, 1),glm::vec2(1.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, -1.0f),glm::vec4(1, 1, 1, 1),glm::vec2(0.0f, 1.0f),

	//left
	glm::vec3(-1.0f, -1.0f, -1.0f),glm::vec4(1, 1, 1, 1),glm::vec2(0.0f, 0.0f),
	glm::vec3(-1.0f, -1.0f, 1.0f),glm::vec4(1, 1, 1, 1),glm::vec2(1.0f, 0.0f),
	glm::vec3(-1.0f, 1.0f, 1.0f),glm::vec4(1, 1, 1, 1),glm::vec2(1.0f, 1.0f),

	glm::vec3(-1.0f, -1.0f, -1.0f),glm::vec4(1, 1, 1, 1),glm::vec2(0.0f, 0.0f),
	glm::vec3(-1.0f, 1.0f, 1.0f),glm::vec4(1, 1, 1, 1),glm::vec2(1.0f, 1.0f),
	glm::vec3(-1.0f, 1.0f, -1.0f),glm::vec4(1, 1, 1, 1),glm::vec2(0.0f, 1.0f),

	//right
	glm::vec3(1.0f, -1.0f, 1.0f),glm::vec4(1, 1, 1, 1),glm::vec2(0.0f, 0.0f),
	glm::vec3(1.0f, -1.0f, -1.0f),glm::vec4(1, 1, 1, 1),glm::vec2(1.0f, 0.0f),
	glm::vec3(1.0f, 1.0f, -1.0f),glm::vec4(1, 1, 1, 1),glm::vec2(1.0f, 1.0f),

	glm::vec3(1.0f, -1.0f, 1.0f),glm::vec4(1, 1, 1, 1),glm::vec2(0.0f, 0.0f),
	glm::vec3(1.0f, 1.0f, -1.0f),glm::vec4(1, 1, 1, 1),glm::vec2(1.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, 1.0f),glm::vec4(1, 1, 1, 1),glm::vec2(0.0f, 1.0f),

	//up
	glm::vec3(-1.0f, 1.0f, 1.0f),glm::vec4(1, 1, 1, 1),glm::vec2(0.0f, 0.0f),
	glm::vec3(1.0f, 1.0f, 1.0f),glm::vec4(1, 1, 1, 1),glm::vec2(1.0f, 0.0f),
	glm::vec3(1.0f, 1.0f, -1.0f),glm::vec4(1, 1, 1, 1),glm::vec2(1.0f, 1.0f),

	glm::vec3(-1.0f, 1.0f, 1.0f),glm::vec4(1, 1, 1, 1),glm::vec2(0.0f, 0.0f),
	glm::vec3(1.0f, 1.0f, -1.0f),glm::vec4(1, 1, 1, 1),glm::vec2(1.0f, 1.0f),
	glm::vec3(-1.0f, 1.0f, -1.0f),glm::vec4(1, 1, 1, 1),glm::vec2(0.0f, 1.0f),

	//down
	glm::vec3(-1.0f, -1.0f, -1.0f),glm::vec4(1, 1, 1, 1),glm::vec2(0.0f, 0.0f),
	glm::vec3(1.0f, -1.0f, -1.0f),glm::vec4(1, 1, 1, 1),glm::vec2(1.0f, 0.0f),
	glm::vec3(1.0f, -1.0f, 1.0f),glm::vec4(1, 1, 1, 1),glm::vec2(1.0f, 1.0f),

	glm::vec3(-1.0f, -1.0f, -1.0f),glm::vec4(1, 1, 1, 1),glm::vec2(0.0f, 0.0f),
	glm::vec3(1.0f, -1.0f, 1.0f),glm::vec4(1, 1, 1, 1),glm::vec2(1.0f, 1.0f),
	glm::vec3(-1.0f, -1.0f, 1.0f),glm::vec4(1, 1, 1, 1),glm::vec2(0.0f, 1.0f)
};


static std::vector<Vertex> kVertexRemoveDumplicateVector;
static std::vector<unsigned short> kVertexIndexVector;


static void VertexRemoveDumplicate() {
	for (int i = 0; i < 36; ++i) {
		const Vertex& vertex = kVertices[i];
		//判断顶点是否存在
		int index = -1;
		for (int j = 0; j < kVertexRemoveDumplicateVector.size(); ++j) {
			if (vertex.pos == kVertexRemoveDumplicateVector[j].pos && vertex.color == kVertexRemoveDumplicateVector[j].color && vertex.uv == kVertexRemoveDumplicateVector[j].uv) {
				index = j;
				break;
			}
		}
		if (index < 0) {
			kVertexRemoveDumplicateVector.push_back(vertex);
			kVertexIndexVector.push_back(kVertexRemoveDumplicateVector.size() - 1);
		}
		else {
			kVertexIndexVector.push_back(index);
		}
	}
};

static void ExportMesh(std::string save_path) {
	std::ofstream output_file_stream(save_path, std::ios::out | std::ios::binary);

	MeshFileHeader mesh_file_header;
	mesh_file_header.type[0] = 'm';
	mesh_file_header.type[1] = 'e';
	mesh_file_header.type[2] = 's';
	mesh_file_header.type[3] = 'h';
	mesh_file_header.vertex_num = kVertexRemoveDumplicateVector.size();
	mesh_file_header.vertex_index_num = kVertexIndexVector.size();
	//写入文件头
	output_file_stream.write((char*)&mesh_file_header, sizeof(MeshFileHeader));
	//写入顶点数据
	output_file_stream.write((char*)&kVertexRemoveDumplicateVector[0], sizeof(Vertex) * kVertexRemoveDumplicateVector.size());
	//写入索引数据
	output_file_stream.write((char*)&kVertexIndexVector[0], sizeof(unsigned short) * kVertexIndexVector.size());
	output_file_stream.close();
}





#endif 