module;
#include "key_code.h"
module input_system;

bool Input::GetKey(unsigned short keyCode) {
	return m_KeyEventMap.count(keyCode) > 0;
}

bool Input::GetKeyDown(unsigned short keyCode) {
	if (m_KeyEventMap.count(keyCode) == 0)return false;
	return m_KeyEventMap[keyCode] != KEY_ACTION_UP;
}

bool Input::GetKeyUp(unsigned short keyCode) {
	if (m_KeyEventMap.count(keyCode) == 0)return false;
	return m_KeyEventMap[keyCode] == KEY_ACTION_UP;
}

void Input::RecordKey(int keyCode,unsigned char keyAction) {
	if (keyCode < 0) {
		//assert(false);
		return;
	}
	m_KeyEventMap[keyCode] = keyAction;
}

void Input::Update() {
	for (auto it = m_KeyEventMap.begin(); it != m_KeyEventMap.end();) {
		if (it->second == KEY_ACTION_UP) {
			it = m_KeyEventMap.erase(it);//返回指向下一个元素的迭代器
		}
		else {
			++it;
		}
	}
	m_MouseScroll = 0;
}

bool Input::GetMouseButton(unsigned short mouseButtonIndex) {
	return GetKey(mouseButtonIndex);
}

bool Input::GetMouseButtonDown(unsigned short mouseButtonIndex) {
	return GetKeyDown(mouseButtonIndex);
}

bool Input::GetMouseButtonUp(unsigned short mouseButtonIndex) {
	return GetKeyUp(mouseButtonIndex);
}