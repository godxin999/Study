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
#include <vector>

struct Vertex {
	glm::vec3 pos;
	glm::vec4 color;
	glm::vec2 uv;
};

static const Vertex kVertices[36] = {
	//Front
	glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec4(1, 0, 0, 1),glm::vec2(0.0f, 0.0f),
	glm::vec3(1.0f, -1.0f, 1.0f), glm::vec4(1, 0, 0, 1),glm::vec2(1.0f, 0.0f),
	glm::vec3(1.0f, 1.0f, 1.0f),glm::vec4(1, 0, 0, 1),glm::vec2(1.0f, 1.0f),

	glm::vec3(-1.0f, -1.0f, 1.0f),glm::vec4(1, 0, 0, 1),glm::vec2(0.0f, 0.0f),
	glm::vec3(1.0f, 1.0f, 1.0f), glm::vec4(1, 0, 0, 1),glm::vec2(1.0f, 1.0f),
	glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec4(1, 0, 0, 1),glm::vec2(0.0f, 1.0f),

	//back
	glm::vec3(1.0f, -1.0f, -1.0f),glm::vec4(1, 0, 0, 1),glm::vec2(0.0f, 0.0f),
	glm::vec3(-1.0f, -1.0f, -1.0f),glm::vec4(1, 0, 0, 1),glm::vec2(1.0f, 0.0f),
	glm::vec3(-1.0f, 1.0f, -1.0f),glm::vec4(1, 0, 0, 1),glm::vec2(1.0f, 1.0f),

	glm::vec3(1.0f, -1.0f, -1.0f),glm::vec4(1, 0, 0, 1),glm::vec2(0.0f, 0.0f),
	glm::vec3(-1.0f, 1.0f, -1.0f),glm::vec4(1, 0, 0, 1),glm::vec2(1.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, -1.0f),glm::vec4(1, 0, 0, 1),glm::vec2(0.0f, 1.0f),

	//left
	glm::vec3(-1.0f, -1.0f, -1.0f),glm::vec4(0, 1, 0, 1),glm::vec2(0.0f, 0.0f),
	glm::vec3(-1.0f, -1.0f, 1.0f),glm::vec4(0, 1, 0, 1),glm::vec2(1.0f, 0.0f),
	glm::vec3(-1.0f, 1.0f, 1.0f),glm::vec4(0, 1, 0, 1),glm::vec2(1.0f, 1.0f),

	glm::vec3(-1.0f, -1.0f, -1.0f),glm::vec4(0, 1, 0, 1),glm::vec2(0.0f, 0.0f),
	glm::vec3(-1.0f, 1.0f, 1.0f),glm::vec4(0, 1, 0, 1),glm::vec2(1.0f, 1.0f),
	glm::vec3(-1.0f, 1.0f, -1.0f),glm::vec4(0, 1, 0, 1),glm::vec2(0.0f, 1.0f),

	//right
	glm::vec3(1.0f, -1.0f, 1.0f),glm::vec4(0, 1, 0, 1),glm::vec2(0.0f, 0.0f),
	glm::vec3(1.0f, -1.0f, -1.0f),glm::vec4(0, 1, 0, 1),glm::vec2(1.0f, 0.0f),
	glm::vec3(1.0f, 1.0f, -1.0f),glm::vec4(0, 1, 0, 1),glm::vec2(1.0f, 1.0f),

	glm::vec3(1.0f, -1.0f, 1.0f),glm::vec4(0, 1, 0, 1),glm::vec2(0.0f, 0.0f),
	glm::vec3(1.0f, 1.0f, -1.0f),glm::vec4(0, 1, 0, 1),glm::vec2(1.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, 1.0f),glm::vec4(0, 1, 0, 1),glm::vec2(0.0f, 1.0f),

	//up
	glm::vec3(-1.0f, 1.0f, 1.0f),glm::vec4(0, 0, 1, 1),glm::vec2(0.0f, 0.0f),
	glm::vec3(1.0f, 1.0f, 1.0f),glm::vec4(0, 0, 1, 1),glm::vec2(1.0f, 0.0f),
	glm::vec3(1.0f, 1.0f, -1.0f),glm::vec4(0, 0, 1, 1),glm::vec2(1.0f, 1.0f),

	glm::vec3(-1.0f, 1.0f, 1.0f),glm::vec4(0, 0, 1, 1),glm::vec2(0.0f, 0.0f),
	glm::vec3(1.0f, 1.0f, -1.0f),glm::vec4(0, 0, 1, 1),glm::vec2(1.0f, 1.0f),
	glm::vec3(-1.0f, 1.0f, -1.0f),glm::vec4(0, 0, 1, 1),glm::vec2(0.0f, 1.0f),

	//down
	glm::vec3(-1.0f, -1.0f, -1.0f),glm::vec4(0, 0, 1, 1),glm::vec2(0.0f, 0.0f),
	glm::vec3(1.0f, -1.0f, -1.0f),glm::vec4(0, 0, 1, 1),glm::vec2(1.0f, 0.0f),
	glm::vec3(1.0f, -1.0f, 1.0f),glm::vec4(0, 0, 1, 1),glm::vec2(1.0f, 1.0f),

	glm::vec3(-1.0f, -1.0f, -1.0f),glm::vec4(0, 0, 1, 1),glm::vec2(0.0f, 0.0f),
	glm::vec3(1.0f, -1.0f, 1.0f),glm::vec4(0, 0, 1, 1),glm::vec2(1.0f, 1.0f),
	glm::vec3(-1.0f, -1.0f, 1.0f),glm::vec4(0, 0, 1, 1),glm::vec2(0.0f, 1.0f)
};


static std::vector<Vertex> kVertexRemoveDumplicateVector;
static std::vector<unsigned short> kVertexIndexVector;


static void VertexRemoveDumplicate() {
	for (int i = 0; i < 36; ++i) {
		const Vertex& vertex = kVertices[i];
		//ÅÐ¶Ï¶¥µãÊÇ·ñ´æÔÚ
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


#endif 