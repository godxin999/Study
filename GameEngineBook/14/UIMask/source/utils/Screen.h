#pragma once

#ifndef SCREEN_H
#define SCREEN_H

class Screen {
public:
	static int width() {
		return width_;
	}
	static int height() {
		return height_;
	}
	static float aspect_ratio() {
		return aspect_ratio_;
	}
	static void set_width(int width) {
		width_ = width;
		calculate_screen_size_ratio();
	}
	static void set_height(int height) {
		height_ = height;
		calculate_screen_size_ratio();
	}
	static void set_width_height(int width, int height) {
		width_ = width;
		height_ = height;
		calculate_screen_size_ratio();
	}
private:
	static void calculate_screen_size_ratio() {
		aspect_ratio_ = width_ * 1.0f / height_;
	}
private:
	inline static int width_;
	inline static int height_;
	inline static float aspect_ratio_;
};

#endif