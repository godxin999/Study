/*************************************************************************
	> File Name: texture2d.h
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/30 20:34:26
 ************************************************************************/

#pragma once

#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <KHR/glext.h>
#include <iostream>

class Texture2D {
public:
	static Texture2D* LoadFromFile(const std::string& image_file_path);
	static Texture2D* LoadFromCptFile(const std::string& image_file_path);
	//压缩图片文件
	static void CompressImageFile(const std::string& image_file_path, const std::string& save_image_file_path);
	//创建Texture(不压缩)
	static Texture2D* Create(unsigned short width, unsigned short height, unsigned server_format, unsigned client_format, unsigned data_type, unsigned char* data);
public:
	struct CptFileHeader {
		char type[3];
		int mipmap_level;
		int width;
		int height;
		int gl_texture_format;
		int compress_size;
	};
	int mipmap_level() {
		return mipmap_level_;
	}
	int width() {
		return width_;
	}
	int height() {
		return height_;
	}
	GLenum gl_texture_format() {
		return gl_texture_format_;
	}
	GLuint gl_texture_id() {
		return gl_texture_id_;
	}
	//部分更新Texture
	void UpdateSubImage(int x, int y, int width, int height, unsigned client_format, unsigned data_type, unsigned char* data);
private:
	int mipmap_level_{ 0 };
	int width_{ 0 };
	int height_{ 0 };
	GLenum gl_texture_format_;
	GLuint gl_texture_id_;
};





#endif // !TEXTURE2D_H