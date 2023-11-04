#include "Input.h"
#include <iostream>
#include <cassert>
#include "key_code.h"

bool Input::GetKey(unsigned short key_code) {
	return key_event_map_.count(key_code) > 0;
}

bool Input::GetKeyDown(unsigned short key_code) {
	if (key_event_map_.count(key_code) == 0)return false;
	return key_event_map_[key_code] != KEY_ACTION_UP;
}

bool Input::GetKeyUp(unsigned short key_code) {
	if (key_event_map_.count(key_code) == 0)return false;
	return key_event_map_[key_code] == KEY_ACTION_UP;
}

void Input::RecordKey(int key_code,unsigned char key_action) {
	if (key_code < 0) {
		assert(false);
		return;
	}
	key_event_map_[key_code] = key_action;
}

void Input::Update() {
	for (auto it = key_event_map_.begin(); it != key_event_map_.end();) {
		if (it->second == KEY_ACTION_UP) {
			it = key_event_map_.erase(it);//返回指向下一个元素的迭代器
		}
		else {
			++it;
		}
	}
	mouse_scroll_ = 0;
}

bool Input::GetMouseButton(unsigned short mouse_button_index) {
	return GetKey(mouse_button_index);
}

bool Input::GetMouseButtonDown(unsigned short mouse_button_index) {
	return GetKeyDown(mouse_button_index);
}

bool Input::GetMouseButtonUp(unsigned short mouse_button_index) {
	return GetKeyUp(mouse_button_index);
}