#pragma once
#include "Screen.h"
#include <vector>
class Window_mgr {
private:
	std::vector<Screen> screens{ Screen(24,80,' ') };//初始化为含有一个Screen类型的vector



};