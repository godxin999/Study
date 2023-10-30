#pragma once

#ifndef INPUT_H
#define INPUT_H

#include <unordered_map>
#include <glm/glm.hpp>

struct vec2_ushort {
	unsigned short x_{ 0 };
	unsigned short y_{ 0 };
};

class Input {
public:
	static bool GetKey(unsigned short key_code);
	static bool GetKeyDown(unsigned short key_code);
	static bool GetKeyUp(unsigned short key_code);
	static bool GetMouseButton(unsigned short mouse_button_index);
	static bool GetMouseButtonDown(unsigned short mouse_button_index);
	static bool GetMouseButtonUp(unsigned short mouse_button_index);
	static void RecordKey(int key_code, unsigned char key_action);
	static vec2_ushort mouse_position() {
		return mouse_position_;
	}
	static void SetMousePosition(unsigned short x, unsigned short y) {
		mouse_position_.x_ = x;
		mouse_position_.y_ = y;
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
	inline static vec2_ushort mouse_position_;//鼠标位置
	inline static short mouse_scroll_;//鼠标滚轮值
};

#endif