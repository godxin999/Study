#pragma once 
#ifndef UITEXT_H
#define UITEXT_H

#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include "component/Component.h"

class Font;
class MeshShader;
class UIText :public Component {
public:
	void SetFont(Font* font) {
		font_ = font;
	}
	Font* font() {
		return font_;
	}
	void SetText(std::string text);
	std::string text() {
		return text_;
	}
	void SetColor(glm::vec4 color) {
		color_ = color;
	}
	glm::vec4 color() {
		return color_;
	}
public:
	void Update()override;
	void OnPreRender()override;
	void OnPostRender()override;
private:
	Font* font_{ nullptr };
	std::string text_;
	bool dirty_{ false };//是否变化需要重新生成Mesh
	glm::vec4 color_{ 1,1,1,1 };
};

#endif // !UITEXT_H
