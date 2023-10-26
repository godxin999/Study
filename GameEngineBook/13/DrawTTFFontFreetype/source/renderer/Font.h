#pragma once

#ifndef FONT_H
#define FONT_H

#include <iostream>
#include <unordered_map>
#include <freetype/ftglyph.h>

class Texture2D;
class Font {
public:
	//���ļ��м���ttf����
	static Font* LoadFromFile(std::string font_file_path, unsigned short font_size);
	static Font* GetFont(std::string font_file_path);
public:
	//Ϊ�ַ�����bitmap
	void LoadCharacter(char c);

	Texture2D* font_texture() {
		return font_texture_;
	}
private:
	unsigned short font_size_{ 20 };//Ĭ�������С
	char* font_file_buffer_{ nullptr };
	FT_Library ft_library_{ nullptr };
	FT_Face ft_face_{ nullptr };
	Texture2D* font_texture_{ nullptr };
	unsigned short font_texture_size_{ 1024 };
private:
	inline static std::unordered_map<std::string, Font*> font_map_;//�洢���ص�����
};

#endif