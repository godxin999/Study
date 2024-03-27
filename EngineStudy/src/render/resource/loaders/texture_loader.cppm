module;
#include <stb/stb_image.h>
#include <glad/glad.h>
export module texture_loader;

import texture;
import stl;
import gl_enum;

namespace Engine::inline Render{
    export class TextureLoader{
    public:
        TextureLoader()=delete;
        ~TextureLoader()=delete;
        static Texture* LoadTexture(
            const std::u8string& texturePath, 
            TextureFilteringMode minFilter, 
            TextureFilteringMode magFilter,
            bool generateMipmap
        );
        static Texture* LoadTextureFromMemory(
            const std::vector<uint8_t>& data, 
            uint32_t width, 
            uint32_t height,
            TextureFilteringMode minFilter, TextureFilteringMode magFilter, 
            bool generateMipmap
        );
        static Texture* CreateColorTexture(
            uint32_t color, 
            TextureFilteringMode minFilter, TextureFilteringMode magFilter, 
            bool generateMipmap
        );
        static void ReloadTexture(
            Texture& texture, 
            const std::u8string& texturePath, TextureFilteringMode minFilter, TextureFilteringMode magFilter, 
            bool generateMipmap
        );
        static bool UnloadTexture(Texture*& texture);
    };
}

module : private;

namespace Engine::inline Render{
    Texture* TextureLoader::LoadTexture(
        const std::u8string& texturePath, 
        TextureFilteringMode minFilter, 
        TextureFilteringMode magFilter,
        bool generateMipmap
    ){
        uint32_t textureID;
        int width,height,bpp;
        
        glGenTextures(1,&textureID);
        
        stbi_set_flip_vertically_on_load(true);
        
        unsigned char* data=stbi_load(reinterpret_cast<const char*>(texturePath.c_str()),&width,&height,&bpp,4);

        if(data){
            glBindTexture(GL_TEXTURE_2D,textureID);
            glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
            
            if(generateMipmap){
                glGenerateMipmap(GL_TEXTURE_2D);
            }
            
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,static_cast<GLenum>(minFilter));
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,static_cast<GLenum>(magFilter));
        }
        stbi_image_free(data);

        glBindTexture(GL_TEXTURE_2D,0);
        
        return new Texture(texturePath,textureID,width,height,bpp,minFilter,magFilter,generateMipmap);
    }

    Texture* TextureLoader::LoadTextureFromMemory(
        const std::vector<uint8_t>& data, 
        uint32_t width, 
        uint32_t height,
        TextureFilteringMode minFilter, TextureFilteringMode magFilter, 
        bool generateMipmap
    ){
        uint32_t textureID;
        glGenTextures(1,&textureID);
        glBindTexture(GL_TEXTURE_2D,textureID);

        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,data.data());

        if(generateMipmap){
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,static_cast<GLenum>(minFilter));
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,static_cast<GLenum>(magFilter));

        glBindTexture(GL_TEXTURE_2D,0);
        return new Texture(u8"",textureID,width,height,32,minFilter,magFilter,generateMipmap);
    }

    Texture* TextureLoader::CreateColorTexture(
        uint32_t color, 
        TextureFilteringMode minFilter, TextureFilteringMode magFilter, 
        bool generateMipmap
    ){
        uint32_t textureID;
        glGenTextures(1,&textureID);
        glBindTexture(GL_TEXTURE_2D,textureID);
        //创建单像素纹理
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,1,1,0,GL_RGBA,GL_UNSIGNED_BYTE,&color);

        if(generateMipmap){
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,static_cast<GLenum>(minFilter));
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,static_cast<GLenum>(magFilter));

        glBindTexture(GL_TEXTURE_2D,0);
        return new Texture(u8"",textureID,1,1,32,minFilter,magFilter,generateMipmap);
    }

    void TextureLoader::ReloadTexture(
        Texture& texture, 
        const std::u8string& texturePath, TextureFilteringMode minFilter, TextureFilteringMode magFilter, 
        bool generateMipmap
    ){
        if(Texture* newTexture=LoadTexture(texturePath,minFilter,magFilter,generateMipmap)){
            glDeleteTextures(1,&texture.textureID);
            texture=*newTexture;
            delete newTexture;
        }
    }

    bool TextureLoader::UnloadTexture(Texture*& texture){
        if(texture){
            glDeleteTextures(1,&texture->textureID);
            delete texture;
            texture=nullptr;
            return true;
        }
        return false;
    }
}
