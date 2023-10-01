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

	stbi_set_flip_vertically_on_load(true);//��תͼƬ����������ͼƬ���ݴ����½ǿ�ʼ��������ΪOpenGL������������ʼ��Ϊ���½�
	int channels_in_file;//ͨ����
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

		glGenTextures(1, &(texture2d->gl_texture_id));//֪ͨ�Կ�����������󣬷��ؾ��

		glBindTexture(GL_TEXTURE_2D, texture2d->gl_texture_id);//������󶨵��ض�����Ŀ��

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture2d->width, texture2d->height, 0, texture2d->gl_texture_format, GL_UNSIGNED_BYTE, data);//��ͼ��RGB�����ϴ���GPU
		
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);//��������Ŵ���˷�ʽΪ���Բ�ֵ
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);//����������С���˷�ʽΪ���Բ�ֵ

		stbi_image_free(data);//�ͷ�ͼƬ�ļ��ڴ�
		return texture2d;
	}
}