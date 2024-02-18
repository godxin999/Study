module;
#include <glad/glad.h>
module buffer.vertex_buffer;

import stl;

namespace Engine::inline Render{
    template class VertexBuffer<float>;
    
    template <typename T>
    VertexBuffer<T>::VertexBuffer(const T* data, uint32_t count){
        glGenBuffers(1,&m_BufferID);
        glBindBuffer(GL_ARRAY_BUFFER,m_BufferID);
        glBufferData(GL_ARRAY_BUFFER,count * sizeof(T),data,GL_STATIC_DRAW);
    }
    template <typename T>
    VertexBuffer<T>::VertexBuffer(const std::vector<T>& data):VertexBuffer(data.data(),data.size()){

    }
    template <typename T>
    VertexBuffer<T>::~VertexBuffer(){
        glDeleteBuffers(1,&m_BufferID);
    }
    template <typename T>
    void VertexBuffer<T>::Bind() const{
        glBindBuffer(GL_ARRAY_BUFFER,m_BufferID);
    }
    template <typename T>
    void VertexBuffer<T>::Unbind() const{
        glBindBuffer(GL_ARRAY_BUFFER,0);
    }
}