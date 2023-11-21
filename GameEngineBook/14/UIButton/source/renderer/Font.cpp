#include "Font.h"
#include <fstream>
#include "freetype/ftbitmap.h"
#include "utils/Application.h"
#include "utils/Debug.h"
#include "texture2d.h"
#include <spdlog/spdlog.h>

Font* Font::LoadFromFile(std::string font_file_path, unsigned short font_size) {
	if (Font* font = GetFont(font_file_path); font) {
		return font;
	}
	Font* font = new Font();
	//��ȡttf�����ļ�
	std::ifstream input_file_stream(Application::assets_path() + font_file_path, std::ios::in | std::ios::binary);
	input_file_stream.seekg(0, std::ios::end);
	int len = input_file_stream.tellg();
	input_file_stream.seekg(0, std::ios::beg);
	font->font_file_buffer_ = new char[len];
	input_file_stream.read(font->font_file_buffer_, len);
	//��ttf����FreeType���н����������������font
	FT_Init_FreeType(&font->ft_library_);//FreeType��ʼ��
	FT_Error error = FT_New_Memory_Face(font->ft_library_, (const FT_Byte*)font->font_file_buffer_, len, 0, &font->ft_face_);
	if (error) {
		spdlog::error("FT_New_Memory_Face return error {}!", error);
		return nullptr;
	}
	FT_Select_Charmap(font->ft_face_, FT_ENCODING_UNICODE);
	FT_F26Dot6 ft_size = (FT_F26Dot6)(font_size * (1 << 6));
	FT_Set_Char_Size(font->ft_face_, ft_size, 0, 72, 72);
	if (!font->ft_face_) {
		spdlog::error("FT_Set_Char_Size error!");
		return nullptr;
	}
	font->font_size_ = font_size;
	font_map_[font_file_path] = font;
	//�����հ׵ġ���Alphaͨ������������������
	unsigned char* pixels = new unsigned char[font->font_texture_size_ * font->font_texture_size_];
	memset(pixels, 0, font->font_texture_size_ * font->font_texture_size_);
	font->font_texture_ = Texture2D::Create(font->font_texture_size_, font->font_texture_size_, GL_RED, GL_RED, GL_UNSIGNED_BYTE, pixels);
	delete[] pixels;
	return font;
}

void Font::LoadCharacter(char c) {
	if (character_map_[c])return;
	//�����ֵ����ε�ft_face_��ȥ��
	FT_Load_Glyph(ft_face_, FT_Get_Char_Index(ft_face_, c), FT_LOAD_DEFAULT);
	//��ft_face_�϶�ȡ������Ϣ��ft_glyph
	FT_Glyph ft_glyph;
	FT_Get_Glyph(ft_face_->glyph, &ft_glyph);
	//��ȾΪ256���Ҷ�ͼ
	FT_Glyph_To_Bitmap(&ft_glyph, ft_render_mode_normal, 0, 1);

	FT_BitmapGlyph ft_bitmap_glyph = (FT_BitmapGlyph)ft_glyph;
	FT_Bitmap& ft_bitmap = ft_bitmap_glyph->bitmap;
	//���������ɵ��ַ���ͼ���е�����
	if (font_texture_fill_x + ft_bitmap.width >= font_texture_size_) {
		//�����Ͻ������Ͻ���䣬���˾ͻ�һ��
		font_texture_fill_x = 0;
		font_texture_fill_y += font_size_;
	}
	if (font_texture_fill_y + font_size_ >= font_texture_size_) {
		spdlog::error("{} is out of font_texture y", c);
		return;
	}
	font_texture_->UpdateSubImage(font_texture_fill_x, font_texture_fill_y, ft_bitmap.width, ft_bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, ft_bitmap.buffer);
	//�洢�ַ���Ϣ

	auto calcUV = [font_texture_size = this->font_texture_size_](unsigned x) {
		return x * 1.0f / font_texture_size;
		};

	Character* character = new Character(calcUV(font_texture_fill_x), calcUV(font_texture_fill_y), calcUV(font_texture_fill_x + ft_bitmap.width), calcUV(font_texture_fill_y + ft_bitmap.rows));
	character_map_[c] = character;

	font_texture_fill_x += ft_bitmap.width;
}

std::vector<Font::Character*> Font::LoadStr(std::string str) {
	//���������ַ���bitmap
	for (auto c : str) {
		LoadCharacter(c);
	}
	//��ȡ�����ַ�����Ϣ
	std::vector<Character*> character_vec_;
	for (auto c : str) {
		if (!character_map_[c]) {
			spdlog::error("LoadStr error,no bitmap, c:{}", c);
			continue;
		}
		character_vec_.push_back(character_map_[c]);
	}
	return character_vec_;
}

Font* Font::GetFont(std::string font_file_path) {
	return font_map_[font_file_path];
}
