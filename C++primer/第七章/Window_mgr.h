#pragma once
#include <vector>
#include "Screen.h"
class Window_mgr {
public:
	using ScreenIndex = std::vector<Screen>::size_type;
	void clear(ScreenIndex);
	ScreenIndex addScreen(const Screen&);//向显示器添加一个新的屏幕
private:
	std::vector<Screen> screens{Screen(24,80,' ')};//初始化为含有一个Screen类型的vector
};