/*************************************************************************
	> File Name: texture2d.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/30 20:34:31
 ************************************************************************/

#include "texture2d.h"
#include "../utils/Application.h"
#include <fstream>
#include <stb/stb_image.h>

Texture2D* Texture2D::LoadFromFile(std::string& image_file_path) {
	Texture2D* texture2d = new Texture2D();

	stbi_set_flip_vertically_on_load(true);//翻转图片，解析出的图片数据从左下角开始，这是因为OpenGL的纹理坐标起始点为左下角
	int channels_in_file;//通道数
	unsigned char* data = stbi_load(image_file_path.c_str(), &(texture2d->width_), &(texture2d->height_), &channels_in_file, 0);
	int image_data_format = GL_RGB;
	if (data) {
		switch (channels_in_file) {
		case 1:
		{
			image_data_format = GL_ALPHA;
			break;
		}
		case 3:
		{
			image_data_format = GL_RGB;
			texture2d->gl_texture_format_ = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
			break;
		}
		case 4:
		{
			image_data_format = GL_RGBA;
			texture2d->gl_texture_format_ = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			break;
		}
		}
	}
	glGenTextures(1, &(texture2d->gl_texture_id_));//通知显卡创建纹理对象，返回句柄

	glBindTexture(GL_TEXTURE_2D, texture2d->gl_texture_id_);//将纹理绑定到特定纹理目标

	//使用GL_COMPRESSED_RGB格式，压缩纹理数据，减少显存占用
	glTexImage2D(GL_TEXTURE_2D, texture2d->mipmap_level_, texture2d->gl_texture_format_, texture2d->width_, texture2d->height_, 0, image_data_format, GL_UNSIGNED_BYTE, data);//将图像RGB数据上传至GPU

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//设置纹理放大过滤方式为线性插值
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//设置纹理缩小过滤方式为线性插值

	stbi_image_free(data);//释放图片文件内存
	return texture2d;
}

Texture2D* Texture2D::LoadFromCptFile(std::string& image_file_path) {
	Texture2D* texture2d = new Texture2D();

	std::ifstream input_file_stream(Application::assets_path() + image_file_path, std::ios::in | std::ios::binary);
	CptFileHeader cpt_file_header;
	input_file_stream.read((char*)&cpt_file_header, sizeof(CptFileHeader));
	unsigned char* data = (unsigned char*)malloc(cpt_file_header.compress_size);
	input_file_stream.read((char*)data, cpt_file_header.compress_size);
	input_file_stream.close();
	texture2d->gl_texture_format_ = cpt_file_header.gl_texture_format;
	texture2d->width_ = cpt_file_header.width;
	texture2d->height_ = cpt_file_header.height;

	glGenTextures(1, &(texture2d->gl_texture_id_));//通知显卡创建纹理对象，返回句柄

	glBindTexture(GL_TEXTURE_2D, texture2d->gl_texture_id_);//将纹理绑定到特定纹理目标

	glCompressedTexImage2D(GL_TEXTURE_2D, 0, texture2d->gl_texture_format_, texture2d->width_, texture2d->height_, 0, cpt_file_header.compress_size, data);//将压缩纹理数据上传至GPU

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//设置纹理放大过滤方式为线性插值
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//设置纹理缩小过滤方式为线性插值

	free(data);
	return texture2d;
}


void Texture2D::CompressImageFile(std::string& image_file_path, std::string& save_image_file_path) {
	Texture2D* texture2d = LoadFromFile(image_file_path);

	//获取压缩是否成功
	GLint compress_succeeded = 0;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED, &compress_succeeded);
	//获取压缩好的纹理数据尺寸
	GLint compress_size = 0;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &compress_size);
	//获取压缩好的纹理压缩格式
	GLint compress_format = 0;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &compress_format);
	//从GPU中将显存中保存的压缩好的纹理数据下载到内存中
	unsigned char* img = (unsigned char*)malloc(compress_size);
	glGetCompressedTexImage(GL_TEXTURE_2D, 0, img);
	//保存为文件
	std::ofstream output_file_stream(save_image_file_path, std::ios::out | std::ios::binary);
	CptFileHeader cpt_file_header;
	cpt_file_header.type[0] = 'c';
	cpt_file_header.type[1] = 'p';
	cpt_file_header.type[2] = 't';
	cpt_file_header.mipmap_level = texture2d->mipmap_level_;
	cpt_file_header.width = texture2d->width_;
	cpt_file_header.height = texture2d->height_;
	cpt_file_header.gl_texture_format = compress_format;
	cpt_file_header.compress_size = compress_size;
	output_file_stream.write((char*)&cpt_file_header, sizeof(CptFileHeader));
	output_file_stream.write((char*)img, compress_size);
	output_file_stream.close();
	free(img);
}
//server_format为GPU内部存储格式，client_format为CPU内存中存储格式
Texture2D* Texture2D::Create(unsigned short width, unsigned short height, unsigned server_format, unsigned client_format, unsigned data_type, unsigned char* data) {
	Texture2D* texture2d = new Texture2D();
	texture2d->width_ = width;
	texture2d->height_ = height;
	texture2d->gl_texture_format_ = server_format;

	glGenTextures(1, &(texture2d->gl_texture_id_));//通知显卡创建纹理对象，返回句柄

	glBindTexture(GL_TEXTURE_2D, texture2d->gl_texture_id_);//将纹理绑定到特定纹理目标

	glTexImage2D(GL_TEXTURE_2D, 0, texture2d->gl_texture_format_, texture2d->width_, texture2d->height_, 0, client_format, data_type, data);//将图像RGB数据上传至GPU

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//设置纹理放大过滤方式为线性插值
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//设置纹理缩小过滤方式为线性插值

	return texture2d;
}

void Texture2D::UpdateSubImage(int x, int y, int width, int height, unsigned client_format, unsigned data_type, unsigned char* data){
	if (width <= 0 || height <= 0)return;
	glBindTexture(GL_TEXTURE_2D, gl_texture_id_);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, client_format, data_type, data);
}