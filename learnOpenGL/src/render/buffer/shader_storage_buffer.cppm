module;
#include <glad/glad.h>
export module buffer.shader_storage_buffer;

import stl;
import gl_enum;

namespace Engine::inline Render{
    export class ShaderStorageBuffer{
    public:
        ShaderStorageBuffer(AccessSpecifier accessSpecifier);
        ~ShaderStorageBuffer();
        void Bind(uint32_t bindingPoint);
        void Unbind();
        template <typename T>
        void SendBlocks(T* data,size_t size){
            glBindBuffer(GL_SHADER_STORAGE_BUFFER,m_BufferID);
            glBufferData(GL_SHADER_STORAGE_BUFFER,size,data,GL_DYNAMIC_DRAW);
            glBindBuffer(GL_SHADER_STORAGE_BUFFER,0);
        }
    private:
        uint32_t m_BufferID;
        uint32_t m_BindingPoint;
    };
}