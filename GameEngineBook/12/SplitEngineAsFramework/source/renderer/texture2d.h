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
	static Texture2D* LoadFromFile(std::string& image_file_path);
	static Texture2D* LoadFromCptFile(std::string& image_file_path);
	static void CompressImageFile(std::string& image_file_path, std::string& save_image_file_path);
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
public:
	int mipmap_level_{ 0 };
	int width_;
	int height_;
	GLenum gl_texture_format_;
	GLuint gl_texture_id_;
};





#endif // !TEXTURE2D_H