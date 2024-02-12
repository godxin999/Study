module;

export module input_system;

import <unordered_map>;
import glm;

export class Input {
public:
	static bool GetKey(unsigned short keyCode);
	static bool GetKeyDown(unsigned short keyCode);
	static bool GetKeyUp(unsigned short keyCode);
	static bool GetMouseButton(unsigned short mouseButtonIndex);
	static bool GetMouseButtonDown(unsigned short mouseButtonIndex);
	static bool GetMouseButtonUp(unsigned short mouseButtonIndex);
	static void RecordKey(int keyCode, unsigned char key_action);
	static glm::vec2 GetMousePosition() {
		return m_MousePosition;
	}
	static void SetMousePosition(unsigned short x, unsigned short y) {
		m_MousePosition.x = x;
		m_MousePosition.y = y;
	}
	static short GetMouseScroll() {
		return m_MouseScroll;
	}
	static void RecordScroll(short mouseScroll) {
		m_MouseScroll = mouseScroll;
	}
	static void Update();
private:
	inline static std::unordered_map<unsigned short, unsigned char> m_KeyEventMap;
	inline static glm::vec2 m_MousePosition{0.f,0.f};//鼠标位置
	inline static short m_MouseScroll{0};//鼠标滚轮值
};
