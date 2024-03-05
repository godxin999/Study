module;
#include <glad/glad.h>
module shader_storage_buffer;

import stl;
import gl_enum;

namespace Engine::inline Render{
    ShaderStorageBuffer::ShaderStorageBuffer(AccessSpecifier accessSpecifier){
        glGenBuffers(1,&m_bufferID);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER,m_bufferID);
        glBufferData(GL_SHADER_STORAGE_BUFFER,0,nullptr,static_cast<GLenum>(accessSpecifier));
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER,0,m_bufferID);
    }
    
    ShaderStorageBuffer::~ShaderStorageBuffer(){
        glDeleteBuffers(1,&m_bufferID);
    }
    
    void ShaderStorageBuffer::Bind(uint32_t bindingPoint){
        m_bindingPoint=bindingPoint;
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER,m_bindingPoint,m_bufferID);
    }
    
    void ShaderStorageBuffer::Unbind(){
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER,m_bindingPoint,0);
    }
}