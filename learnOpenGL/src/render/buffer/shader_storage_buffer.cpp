module;
#include <glad/glad.h>
module shader_storage_buffer;

import stl;
import gl_enum;

namespace Engine::inline Render{
    ShaderStorageBuffer::ShaderStorageBuffer(AccessSpecifier accessSpecifier){
        glGenBuffers(1,&m_BufferID);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER,m_BufferID);
        glBufferData(GL_SHADER_STORAGE_BUFFER,0,nullptr,static_cast<GLenum>(accessSpecifier));
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER,0,m_BufferID);
    }
    ShaderStorageBuffer::~ShaderStorageBuffer(){
        glDeleteBuffers(1,&m_BufferID);
    }
    void ShaderStorageBuffer::Bind(uint32_t bindingPoint){
        m_BindingPoint=bindingPoint;
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER,m_BindingPoint,m_BufferID);
    }
    void ShaderStorageBuffer::Unbind(){
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER,m_BindingPoint,0);
    }
}