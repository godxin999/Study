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

	stbi_set_flip_vertically_on_load(true);//��תͼƬ����������ͼƬ���ݴ����½ǿ�ʼ��������ΪOpenGL������������ʼ��Ϊ���½�
	int channels_in_file;//ͨ����
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
	glGenTextures(1, &(texture2d->gl_texture_id_));//֪ͨ�Կ�����������󣬷��ؾ��

	glBindTexture(GL_TEXTURE_2D, texture2d->gl_texture_id_);//������󶨵��ض�����Ŀ��

	//ʹ��GL_COMPRESSED_RGB��ʽ��ѹ���������ݣ������Դ�ռ��
	glTexImage2D(GL_TEXTURE_2D, texture2d->mipmap_level_, texture2d->gl_texture_format_, texture2d->width_, texture2d->height_, 0, image_data_format, GL_UNSIGNED_BYTE, data);//��ͼ��RGB�����ϴ���GPU

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//��������Ŵ���˷�ʽΪ���Բ�ֵ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//����������С���˷�ʽΪ���Բ�ֵ

	stbi_image_free(data);//�ͷ�ͼƬ�ļ��ڴ�
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

	glGenTextures(1, &(texture2d->gl_texture_id_));//֪ͨ�Կ�����������󣬷��ؾ��

	glBindTexture(GL_TEXTURE_2D, texture2d->gl_texture_id_);//������󶨵��ض�����Ŀ��

	glCompressedTexImage2D(GL_TEXTURE_2D, 0, texture2d->gl_texture_format_, texture2d->width_, texture2d->height_, 0, cpt_file_header.compress_size, data);//��ѹ�����������ϴ���GPU

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//��������Ŵ���˷�ʽΪ���Բ�ֵ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//����������С���˷�ʽΪ���Բ�ֵ

	free(data);
	return texture2d;
}


void Texture2D::CompressImageFile(std::string& image_file_path, std::string& save_image_file_path) {
	Texture2D* texture2d = LoadFromFile(image_file_path);

	//��ȡѹ���Ƿ�ɹ�
	GLint compress_succeeded = 0;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED, &compress_succeeded);
	//��ȡѹ���õ��������ݳߴ�
	GLint compress_size = 0;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &compress_size);
	//��ȡѹ���õ�����ѹ����ʽ
	GLint compress_format = 0;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &compress_format);
	//��GPU�н��Դ��б����ѹ���õ������������ص��ڴ���
	unsigned char* img = (unsigned char*)malloc(compress_size);
	glGetCompressedTexImage(GL_TEXTURE_2D, 0, img);
	//����Ϊ�ļ�
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
//server_formatΪGPU�ڲ��洢��ʽ��client_formatΪCPU�ڴ��д洢��ʽ
Texture2D* Texture2D::Create(unsigned short width, unsigned short height, unsigned server_format, unsigned client_format, unsigned data_type, unsigned char* data) {
	Texture2D* texture2d = new Texture2D();
	texture2d->width_ = width;
	texture2d->height_ = height;
	texture2d->gl_texture_format_ = server_format;

	glGenTextures(1, &(texture2d->gl_texture_id_));//֪ͨ�Կ�����������󣬷��ؾ��

	glBindTexture(GL_TEXTURE_2D, texture2d->gl_texture_id_);//������󶨵��ض�����Ŀ��

	glTexImage2D(GL_TEXTURE_2D, 0, texture2d->gl_texture_format_, texture2d->width_, texture2d->height_, 0, client_format, data_type, data);//��ͼ��RGB�����ϴ���GPU

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//��������Ŵ���˷�ʽΪ���Բ�ֵ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//����������С���˷�ʽΪ���Բ�ֵ

	return texture2d;
}

void Texture2D::UpdateSubImage(int x, int y, int width, int height, unsigned client_format, unsigned data_type, unsigned char* data){
	if (width <= 0 || height <= 0)return;
	glBindTexture(GL_TEXTURE_2D, gl_texture_id_);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, client_format, data_type, data);
}