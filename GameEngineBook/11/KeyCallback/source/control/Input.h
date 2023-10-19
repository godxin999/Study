#pragma once

#ifndef INPUT_H
#define INPUT_H

#include <unordered_map>

class Input {
public:
	static bool GetKey(unsigned short key_code);
	static bool GetKeyDown(unsigned short key_code);
	static bool GetKeyUp(unsigned short key_code);
	static void RecordKey(int key_code, unsigned char key_action);
	static void Update();
private:
	inline static std::unordered_map<unsigned short, unsigned char> key_event_map_;
};

#endif