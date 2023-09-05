#pragma once
#include "KeyCodes.h"

class EventArgs {
public:
	EventArgs() {}
	using base = EventArgs;
};

class KeyEventArgs :public EventArgs {
public:
	enum KeyState {
		Released = 0,
		Pressed = 1
	};
public:
	KeyEventArgs(KeyCode::Key key, unsigned int c, KeyState state, bool ctrl, bool shift, bool alt)
		:Key(key)
		, Character(c)
		, State(state)
		, Ctrl(ctrl)
		, Shift(shift)
		, Alt(alt)
	{

	}
public:
	KeyCode::Key Key;
	unsigned int Character;
	KeyState State;

	bool Alt;
	bool Ctrl;
	bool Shift;
};

class MouseMotionEventArgs :public EventArgs {
public:
	MouseMotionEventArgs(bool leftButton, bool middleButton, bool rightButton, bool ctrl, bool shift, int x, int y)
		:LeftButton(leftButton)
		, MiddleButton(middleButton)
		, RightButton(rightButton)
		, Ctrl(ctrl)
		, Shift(shift)
		, X(x)
		, Y(y)
	{

	}
public:
	bool LeftButton;
	bool MiddleButton;
	bool RightButton;

	bool Ctrl;
	bool Shift;

	int X;
	int Y;

	int DeltaX;//相对于上一次的x坐标的变化量
	int DeltaY;//相对于上一次的y坐标的变化量
};

class MouseButtonEventArgs :public EventArgs {
public:
	enum MouseButton {
		None = 0,
		Left = 1,
		Right = 2,
		Middle = 3
	};
	enum ButtonState {
		Released = 0,
		Pressed = 1
	};
public:
	MouseButtonEventArgs(MouseButton button, ButtonState state, bool leftButton, bool middleButton, bool rightButton, bool ctrl, bool shift, int x, int y)
		:Button(button)
		, State(state)
		, LeftButton(leftButton)
		, MiddleButton(middleButton)
		, RightButton(rightButton)
		, Ctrl(ctrl)
		, Shift(shift)
		, X(x)
		, Y(y)
	{

	}
public:
	MouseButton Button;
	ButtonState State;

	bool LeftButton;
	bool MiddleButton;
	bool RightButton;

	bool Ctrl;
	bool Shift;

	int X;
	int Y;
};

class MouseWheelEventArgs :public EventArgs {
public:
	MouseWheelEventArgs::MouseWheelEventArgs(float wheelDelta, bool leftButton, bool middleButton, bool rightButton, bool ctrl, bool shift, int x, int y)
		:WheelDelta(wheelDelta)
		, LeftButton(leftButton)
		, MiddleButton(middleButton)
		, RightButton(rightButton)
		, Ctrl(ctrl)
		, Shift(shift)
		, X(x)
		, Y(y)
	{

	}
	float WheelDelta;//鼠标滚轮的滚动量，正值为向上滚动，负值为向下滚动

	bool LeftButton;
	bool MiddleButton;
	bool RightButton;

	bool Ctrl;
	bool Shift;

	int X;//光标的x坐标
	int Y;//光标的y坐标
};

class ResizeEventArgs :public EventArgs {
public:
	ResizeEventArgs(int width, int height) :Width(width), Height(height) {}
public:
	int Width;
	int Height;
};

class UpdateEventArgs :public EventArgs {
public:
	UpdateEventArgs(double elapsedTime, double totalTime) :ElapsedTime(elapsedTime), TotalTime(totalTime) {}
public:
	double ElapsedTime;
	double TotalTime;
};

class RenderEventArgs :public EventArgs {
public:
	RenderEventArgs(double elapsedTime, double totalTime) :ElapsedTime(elapsedTime), TotalTime(totalTime) {}
public:
	double ElapsedTime;
	double TotalTime;
};

class UserEventArgs :public EventArgs {
public:
	UserEventArgs(int code, void* data1, void* data2) :Code(code), Data1(data1), Data2(data2) {}
public:
	int Code;
	void* Data1;
	void* Data2;
};