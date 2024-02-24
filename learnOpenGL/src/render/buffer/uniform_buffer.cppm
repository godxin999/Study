module;
#include <glad/glad.h>
export module buffer.uniform_buffer;

import gl_enum;
import shader;
import stl;

namespace Engine::inline Render{
    export class UniformBuffer{
    public:
        UniformBuffer(size_t size,uint32_t bindingPoint=0,uint32_t offset=0,AccessSpecifier accessSpecifier=AccessSpecifier::DYNAMIC_DRAW);
        ~UniformBuffer();
        void Bind();
        void Unbind();
        uint32_t GetID()const;
        template <typename T>
        void SetSubData(const T& data,size_t offset){
            Bind();
            glBufferSubData(GL_UNIFORM_BUFFER,offset,sizeof(T),std::addressof(data));
            Unbind();
        }
        template <typename T>
        void SetSubData(const T& data,size_t& offset){
            Bind();
            size_t size=sizeof(T);
            glBufferSubData(GL_UNIFORM_BUFFER,offset,size,std::addressof(data));
            offset+=size;
            Unbind();
        }
    public:
        static void BindBlockToShader(Shader& shader,uint32_t uniformBlockLocation,uint32_t bindingPoint=0);
        static uint32_t GetBlockLocation(Shader& shader,const std::string& uniformBlockName);
    private:
        uint32_t m_BufferID{};
    };
}