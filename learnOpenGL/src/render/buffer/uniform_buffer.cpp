module;
#include <glad/glad.h>
module uniform_buffer;

import gl_enum;
import shader;

namespace Engine::inline Render{
    UniformBuffer::UniformBuffer(size_t size,uint32_t bindingPoint,uint32_t offset,AccessSpecifier accessSpecifier){
        glGenBuffers(1,&m_BufferID);
        glBindBuffer(GL_UNIFORM_BUFFER,m_BufferID);
        glBufferData(GL_UNIFORM_BUFFER,size,nullptr,static_cast<GLenum>(accessSpecifier));
        glBindBuffer(GL_UNIFORM_BUFFER,0);
        //将uniform buffer绑定到binding point
        glBindBufferRange(GL_UNIFORM_BUFFER,bindingPoint,m_BufferID,offset,size);
    }
    UniformBuffer::~UniformBuffer(){
        glDeleteBuffers(1,&m_BufferID);
    }
    void UniformBuffer::Bind(){
        glBindBuffer(GL_UNIFORM_BUFFER,m_BufferID);
    }
    void UniformBuffer::Unbind(){
        glBindBuffer(GL_UNIFORM_BUFFER,0);
    }
    uint32_t UniformBuffer::GetID()const{
        return m_BufferID;
    }
    void UniformBuffer::BindBlockToShader(Shader& shader,uint32_t uniformBlockLocation,uint32_t bindingPoint){
        //将shader中的uniform block绑定到binding point
        glUniformBlockBinding(shader.m_ShaderID,uniformBlockLocation,bindingPoint);
    }
    uint32_t UniformBuffer::GetBlockLocation(Shader& shader,const std::string& uniformBlockName){
        //获取shader中uniform block的索引
        return glGetUniformBlockIndex(shader.m_ShaderID,uniformBlockName.c_str());
    }
}