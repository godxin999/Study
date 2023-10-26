#pragma once

#ifndef FONT_H
#define FONT_H

#include <iostream>
#include <unordered_map>
#include <freetype/ftglyph.h>

class Texture2D;
class Font {
public:
	//从文件中加载ttf字体
	static Font* LoadFromFile(std::string font_file_path, unsigned short font_size);
	static Font* GetFont(std::string font_file_path);
public:
	//为字符生成bitmap
	void LoadCharacter(char c);

	Texture2D* font_texture() {
		return font_texture_;
	}
private:
	unsigned short font_size_{ 20 };//默认字体大小
	char* font_file_buffer_{ nullptr };
	FT_Library ft_library_{ nullptr };
	FT_Face ft_face_{ nullptr };
	Texture2D* font_texture_{ nullptr };
	unsigned short font_texture_size_{ 1024 };
private:
	inline static std::unordered_map<std::string, Font*> font_map_;//存储加载的字体
};

#endif