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
	struct Character {
		//��¼�����ַ�bitmap��ͼ���е����꣬��UV��Ϣ����������ͬ�ߴ�Ķ�������
		float left_top_x_;
		float left_top_y_;
		float right_bottom_x_;
		float right_bottom_y_;
		Character(float left_top_x, float left_top_y, float right_bottom_x, float right_bottom_y) :
			left_top_x_(left_top_x), left_top_y_(left_top_y), right_bottom_x_(right_bottom_x), right_bottom_y_(right_bottom_y) {}
	};
public:
	//Ϊ�ַ�����bitmap
	void LoadCharacter(char c);
	//Ϊ�ַ�������bitmap
	std::vector<Character*> LoadStr(std::string str);
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
	unsigned short font_texture_fill_x = 0;//��ǰ�е���ʼ���x����
	unsigned short font_texture_fill_y = 0;//��ǰ�е���ʼ���y����
	std::unordered_map<char, Character*> character_map_;//�Ѿ�����bitmap���ַ�
private:
	inline static std::unordered_map<std::string, Font*> font_map_;//�洢���ص�����
};

#endif