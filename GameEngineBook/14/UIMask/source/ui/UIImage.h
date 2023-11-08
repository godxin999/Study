#pragma once

#ifndef UIIMAGE_H
#define UIIMAGE_H

#include <iostream>
#include <vector>
#include "..\component\Component.h"

class Texture2D;
class UIImage :public Component {
public:
	UIImage() = default;
	~UIImage() = default;
	Texture2D* texture2D() {
		return texture2d_;
	}
	void SetTexture(Texture2D* texture2d) {
		texture2d_ = texture2d;
	}
public:
	void Update()override;
	void OnPreRender()override;
	void OnPostRender()override;
private:
	Texture2D* texture2d_{ nullptr };
};

#endif // !UIIMAGE_H