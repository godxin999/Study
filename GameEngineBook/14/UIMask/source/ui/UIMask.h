#pragma once
#ifndef UIMASK_H
#define UIMASK_H
#include "component/Component.h"

class Texture2D;
class UIMask :public Component {
public:
	UIMask() = default;
	~UIMask() = default;
	Texture2D* texture2D() {
		return texture2d_;
	}
	void SetTexture(Texture2D* texture2d) {
		texture2d_ = texture2d;
	}
public:
	void OnEnable() override;
	void Update() override;
	void OnPreRender() override;
	void OnPostRender() override;
	void OnDisable() override;
private:
	Texture2D* texture2d_{ nullptr };
};

#endif // !UIMASK_H
