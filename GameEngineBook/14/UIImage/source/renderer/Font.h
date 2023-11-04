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
	struct Character {
		//记录单个字符bitmap在图集中的坐标，即UV信息，用于生成同尺寸的顶点数据
		float left_top_x_;
		float left_top_y_;
		float right_bottom_x_;
		float right_bottom_y_;
		Character(float left_top_x, float left_top_y, float right_bottom_x, float right_bottom_y) :
			left_top_x_(left_top_x), left_top_y_(left_top_y), right_bottom_x_(right_bottom_x), right_bottom_y_(right_bottom_y) {}
	};
public:
	//为字符生成bitmap
	void LoadCharacter(char c);
	//为字符串生成bitmap
	std::vector<Character*> LoadStr(std::string str);
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
	unsigned short font_texture_fill_x = 0;//当前行的起始点的x坐标
	unsigned short font_texture_fill_y = 0;//当前行的起始点的y坐标
	std::unordered_map<char, Character*> character_map_;//已经生成bitmap的字符
private:
	inline static std::unordered_map<std::string, Font*> font_map_;//存储加载的字体
};

#endif