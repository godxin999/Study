#pragma once

#ifndef DRAWTRIANGLE_VERTEXDATA_H
#define DRAWTRIANGLE_VERTEXDATA_H

#include <glm/glm.hpp>

static const glm::vec3 kPos[3] = {
	glm::vec3{-1.0f,-1.0f,0.0f},
	glm::vec3{1.0f,-1.0f,0.0f},
	glm::vec3{0.0f,1.0f,0.0f}
};

static const glm::vec4 kColors[3] = {
	glm::vec4{1.f,0.f,0.f,1.f},
	glm::vec4{0.f,1.f,0.f,1.f},
	glm::vec4{0.f,0.f,1.f,1.f},
};

#endif 