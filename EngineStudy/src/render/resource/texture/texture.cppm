module;
#include <glad/glad.h>
export module texture;
import stl;
import gl_enum;

namespace Engine::inline Render{
    export class TextureLoader;
    export class Texture{
    public:
        friend class TextureLoader;
        void Bind(uint32_t slot=0) const;
        void Unbind()const;
    private:
        Texture(const std::u8string& p_path, uint32_t id, uint32_t p_width, uint32_t p_height, uint32_t bpp, TextureFilteringMode p_minFilter, TextureFilteringMode p_magFilter, bool generateMipmap);
        ~Texture()=default;
        Texture(const Texture&)=default;
        Texture& operator=(const Texture&)=default;
    public:
        std::u8string path;
        uint32_t textureID;
        uint32_t width;
        uint32_t height;
        uint32_t bitsPerPixel;
        TextureFilteringMode minFilter;
        TextureFilteringMode magFilter;
        bool isMipmap;
    };
}

module : private;

namespace Engine::inline Render{
    Texture::Texture(const std::u8string& p_path, uint32_t id, uint32_t p_width, uint32_t p_height, uint32_t bpp, TextureFilteringMode p_minFilter, TextureFilteringMode p_magFilter, bool generateMipmap)
        : path(p_path),textureID(id), width(p_width), height(p_height), bitsPerPixel(bpp), minFilter(p_minFilter), magFilter(p_magFilter),isMipmap(generateMipmap){

    }

    void Texture::Bind(uint32_t slot) const{
        glActiveTexture(GL_TEXTURE0+slot);
        glBindTexture(GL_TEXTURE_2D,textureID);
    }

    void Texture::Unbind()const{
        glBindTexture(GL_TEXTURE_2D,0);
    }
}