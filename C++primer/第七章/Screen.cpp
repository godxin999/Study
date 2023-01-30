#include "Screen.h"

inline Screen& Screen::move(pos r, pos c) {
	pos row = r * width;
	cursor = row + c;
	return *this;
}
char Screen::get(pos r, pos c) const{
	pos row = r * width;
	return contents[row + c];
}
void Screen::some_member()const {
	++access_ctr;
}
Screen& Screen::set(char c) {//返回*this的成员函数
	contents[cursor] = c;
	return *this;
}
inline Screen& Screen::set(pos r, pos c, char ch) {
	contents[r * width + c] = ch;
	return *this;
}