#pragma once

#ifndef UIBUTTON_H
#define UIBUTTON_H

#include <iostream>
#include <string>
#include <functional>
#include "component/Component.h"

class UIImage;
class UIButton :public Component {
public:
	void SetImageNormal(UIImage* image) {
		image_normal_ = image;
	}
	void SetImagePressed(UIImage* image) {
		image_pressed_ = image;
	}
	void SetClickCallback(std::function<void()> click_callback) {
		click_callback_ = click_callback;
	}
public:
	void Update() override;
private:
	UIImage* image_normal_;
	UIImage* image_pressed_;
	bool pressed_{ false };
	std::function<void()> click_callback_{ nullptr };
};

#endif // !UIBUTTON_H
