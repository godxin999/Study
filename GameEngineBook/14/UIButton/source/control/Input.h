#pragma once

#ifndef INPUT_H
#define INPUT_H

#include <unordered_map>
#include <glm/glm.hpp>


class Input {
public:
	static bool GetKey(unsigned short key_code);
	static bool GetKeyDown(unsigned short key_code);
	static bool GetKeyUp(unsigned short key_code);
	static bool GetMouseButton(unsigned short mouse_button_index);
	static bool GetMouseButtonDown(unsigned short mouse_button_index);
	static bool GetMouseButtonUp(unsigned short mouse_button_index);
	static void RecordKey(int key_code, unsigned char key_action);
	static glm::vec2 mouse_position() {
		return mouse_position_;
	}
	static void SetMousePosition(float x, float y) {
		mouse_position_.x = x;
		mouse_position_.y = y;
	}
	static short mouse_scroll() {
		return mouse_scroll_;
	}
	static void RecordScroll(short mouse_scroll) {
		mouse_scroll_ = mouse_scroll;
	}
	static void Update();
private:
	inline static std::unordered_map<unsigned short, unsigned char> key_event_map_;
	inline static glm::vec2 mouse_position_;//鼠标位置
	inline static short mouse_scroll_;//鼠标滚轮值
};

#endif