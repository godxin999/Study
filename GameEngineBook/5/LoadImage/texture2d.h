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
#include <iostream>

class Texture2D {
private:
	Texture2D() {

	}
	~Texture2D() {
	
	}
public:
	static Texture2D* LoadFromFile(std::string& image_file_path);
	int width;
	int height;
	GLenum gl_texture_format;
};





#endif // !TEXTURE2D_H