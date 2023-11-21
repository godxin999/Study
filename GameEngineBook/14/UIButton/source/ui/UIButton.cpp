#include "UIButton.h"
#include "component/Transform.h"
#include "component/GameObject.h"
#include "renderer/Font.h"
#include "renderer/texture2d.h"
#include "utils/Screen.h"
#include "utils/Debug.h"
#include "control/Input.h"
#include "control/key_code.h"
#include "UIImage.h"
#include <rttr/registration>
#include <glm/glm.hpp>

RTTR_REGISTRATION{
	rttr::registration::class_<UIButton>("UIButton")
			.constructor<>()(rttr::policy::ctor::as_raw_ptr);
}

void UIButton::Update() {
	Component::Update();

	if (!image_normal_ || !image_pressed_)return;

	Transform* transform = dynamic_cast<Transform*>(game_object()->GetComponent("Transform"));
	Transform* transform_image_normal = dynamic_cast<Transform*>(image_normal_->game_object()->GetComponent("Transform"));
	Transform* transform_image_press = dynamic_cast<Transform*>(image_pressed_->game_object()->GetComponent("Transform"));
	transform_image_normal->set_position(transform->position());
	transform_image_press->set_position(transform->position());

	if (Input::GetMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		auto transform_postion = transform->position();
		//获取鼠标位置，并从左上角零点，转换为屏幕中心零点
		auto mouse_postion = Input::mouse_position();
		mouse_postion.x = mouse_postion.x - Screen::width() / 2;
		mouse_postion.y = Screen::height() / 2 - mouse_postion.y;
		//获取图片的宽高
		auto image_normal_width = image_normal_->width();
		auto image_normal_height = image_normal_->height();
		//计算鼠标点击是否在按键范围内
		if ((mouse_postion.x > transform_postion.x && mouse_postion.x < transform_postion.x + image_normal_width) &&
			(mouse_postion.y > transform_postion.y && mouse_postion.y < transform_postion.y + image_normal_height)) {
			if (pressed_) {
				return;
			}
			pressed_ = true;
			if (click_callback_) {
				click_callback_();
			}
		}
		else {
			pressed_ = false;
		}
	}
	else {
		pressed_ = false;
	}
	//当pressed为true时，关闭normal图片，打开press图片
	transform_image_normal->game_object()->SetActive(!pressed_);
	transform_image_press->game_object()->SetActive(pressed_);
}