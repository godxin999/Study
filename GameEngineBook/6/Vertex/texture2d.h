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

#include <glad/gl.h>
#include <glad/glext.h>
#include <iostream>

class Texture2D {
private:
	Texture2D() {

	}
	~Texture2D() {
	
	}
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
public:
	int mipmap_level{ 0 };
	int width;
	int height;
	GLenum gl_texture_format;
	GLuint gl_texture_id;
};





#endif // !TEXTURE2D_H