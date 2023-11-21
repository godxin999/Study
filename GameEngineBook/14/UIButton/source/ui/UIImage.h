#pragma once

#ifndef UIIMAGE_H
#define UIIMAGE_H

#include <iostream>
#include <vector>
#include "component\Component.h"
#include "renderer/texture2d.h"

class Texture2D;
class UIImage :public Component {
public:
	Texture2D* texture2D() {
		return texture2d_;
	}
	void SetTexture(Texture2D* texture2d) {
		texture2d_ = texture2d;
		width_ = texture2d_->width();
		height_ = texture2d_->height();
	}
	int width() {
		return width_;
	}
	int height() {
		return height_;
	}
public:
	void Update()override;
	void OnPreRender()override;
	void OnPostRender()override;
private:
	Texture2D* texture2d_{ nullptr };
	int width_{ 0 };
	int height_{ 0 };
};

#endif // !UIIMAGE_H