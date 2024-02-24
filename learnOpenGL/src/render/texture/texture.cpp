module;
#include <stb/stb_image.h>
#include <glad/glad.h>
module texture;
import stl;


namespace Engine::inline Render{
    void Texture::LoadTexture(const std::string& texturePath){
        glGenTextures(1,&m_TextureID);
        int width,height,nrChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data=stbi_load(texturePath.c_str(),&width,&height,&nrChannels,4);
        if(data){
            glBindTexture(GL_TEXTURE_2D,m_TextureID);
            glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,width,height,0,GL_RGBA,  GL_UNSIGNED_BYTE,data);
            glGenerateMipmap(GL_TEXTURE_2D);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,    GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        }
        stbi_image_free(data);
        glBindTexture(GL_TEXTURE_2D,0);
    }
    void Texture::Bind(uint32_t slot) const{
        glActiveTexture(GL_TEXTURE0+slot);
        glBindTexture(GL_TEXTURE_2D,m_TextureID);
    }
    void Texture::Unbind() const{
        glBindTexture(GL_TEXTURE_2D,0);
    }
}