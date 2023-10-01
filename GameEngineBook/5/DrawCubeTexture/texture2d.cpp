/*************************************************************************
	> File Name: texture2d.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/30 20:34:31
 ************************************************************************/

#include "texture2d.h"
#include <stb/stb_image.h>

Texture2D* Texture2D::LoadFromFile(std::string& image_file_path) {
	Texture2D* texture2d=new Texture2D();

	stbi_set_flip_vertically_on_load(true);//翻转图片，解析出的图片数据从左下角开始，这是因为OpenGL的纹理坐标起始点为左下角
	int channels_in_file;//通道数
	unsigned char* data=stbi_load(image_file_path.c_str(),&texture2d->width,&texture2d->height,&channels_in_file,0);
	if (data) {
		switch (channels_in_file) {
		case 1:
		{
			texture2d->gl_texture_format = GL_ALPHA;
			break;
		}
		case 3:
		{
			texture2d->gl_texture_format = GL_RGB;
			break;
		}
		case 4:
		{
			texture2d->gl_texture_format = GL_RGBA;
			break;
		}
		}

		glGenTextures(1, &(texture2d->gl_texture_id));//通知显卡创建纹理对象，返回句柄

		glBindTexture(GL_TEXTURE_2D, texture2d->gl_texture_id);//将纹理绑定到特定纹理目标

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture2d->width, texture2d->height, 0, texture2d->gl_texture_format, GL_UNSIGNED_BYTE, data);//将图像RGB数据上传至GPU
		
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);//设置纹理放大过滤方式为线性插值
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);//设置纹理缩小过滤方式为线性插值

		stbi_image_free(data);//释放图片文件内存
		return texture2d;
	}
}