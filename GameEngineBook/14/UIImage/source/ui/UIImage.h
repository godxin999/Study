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
		return texture2D_;
	}
	void SetTexture(Texture2D* texture2D) {
		texture2D_ = texture2D;
	}
public:
	void Awake()override;
	void Update()override;
private:
	Texture2D* texture2D_{ nullptr };
};

#endif // !UIIMAGE_H