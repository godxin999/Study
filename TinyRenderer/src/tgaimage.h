#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <algorithm>
#include <fstream>

#pragma pack(push,1)
struct TGA_Header {
	char idlength;
	char colormaptype;
	char datatypecode;
	short colormaporigin;
	short colormaplength;
	char colormapdepth;
	short x_origin;
	short y_origin;
	short width;
	short height;
	char  bitsperpixel;
	char  imagedescriptor;
};
#pragma pack(pop)



struct TGAColor {
	unsigned char bgra[4]{};
	unsigned char bytespp{1};

	TGAColor()=default;
	TGAColor(const TGAColor &c)=default;
	TGAColor& operator=(const TGAColor &c)=default;

	TGAColor(unsigned char R, unsigned char G, unsigned char B, unsigned char A=255) : bytespp(4) {
        bgra[0] = B;
        bgra[1] = G;
        bgra[2] = R;
        bgra[3] = A;
	}
	explicit TGAColor(unsigned char v){
        bgra[0]=v;
	}
	TGAColor(const unsigned char *p, unsigned char bpp) : bytespp(bpp) {
		for (int i=0; i<static_cast<int>(bpp); i++) {
			bgra[i] = p[i];
		}
        for(int i=bpp; i<4; i++){
            bgra[i]=0;
        }
	}
    TGAColor operator*(float intensity){
        TGAColor res=*this;
        intensity=std::clamp(intensity, 0.f, 1.f);
        for(int i=0; i<4; i++){
            res.bgra[i]=bgra[i]*intensity;
        }
        return res;
    }
    unsigned char& operator[](const int i){
        return bgra[i];
    }
};


class TGAImage {
protected:
    unsigned char* data;
    int width;
    int height;
    int bytespp;

    bool   load_rle_data(std::ifstream &in);
    bool unload_rle_data(std::ofstream &out);
public:
    enum Format {
        GRAYSCALE=1, RGB=3, RGBA=4
    };

    TGAImage();
    TGAImage(int w, int h, int bpp);
    TGAImage(const TGAImage &img);
    bool read_tga_file(const char *filename);
    bool write_tga_file(const char *filename, bool rle=true);
    bool flip_horizontally();
    bool flip_vertically();
    bool scale(int w, int h);
    [[nodiscard]]TGAColor get(int x, int y)const;
    bool set(int x, int y, TGAColor &c);
    bool set(int x, int y, const TGAColor &c);
    ~TGAImage();
    TGAImage & operator =(const TGAImage &img);
    [[nodiscard]]int get_width()const;
    [[nodiscard]]int get_height()const;
    [[nodiscard]]int get_bytespp()const;
    unsigned char *buffer();
    void clear();
};

#endif //__IMAGE_H__
