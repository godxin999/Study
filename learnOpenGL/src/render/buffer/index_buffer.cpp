module;
#include <glad/glad.h>
module index_buffer;

import stl;

namespace Engine::inline Render{
    IndexBuffer::IndexBuffer(const uint32_t* data, uint32_t count){
        glGenBuffers(1,&m_BufferID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_BufferID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,count * sizeof(uint32_t),data,GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    }
    IndexBuffer::IndexBuffer(const std::vector<uint32_t>& data):IndexBuffer(data.data(),data.size()){

    }
    IndexBuffer::~IndexBuffer(){
        glDeleteBuffers(1,&m_BufferID);
    }
    void IndexBuffer::Bind() const{
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_BufferID);
    }
    void IndexBuffer::Unbind() const{
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    }
}

