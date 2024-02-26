module;
#include <glad/glad.h>
module vertex_array;

import stl;

namespace Engine::inline Render{
    VertexArray::VertexArray(){
        glGenVertexArrays(1, &m_ArrayID);
    }
    VertexArray::~VertexArray(){
        glDeleteVertexArrays(1, &m_ArrayID);
    }
    void VertexArray::Bind() const{
        glBindVertexArray(m_ArrayID);
    }
    void VertexArray::Unbind() const{
        glBindVertexArray(0);
    }
    void VertexArray::BindAttribute(uint32_t index, size_t size, DataType type, bool normalized, uint64_t stride, const void* pointer){
        glVertexAttribPointer(index, size, static_cast<GLenum>(type), normalized, stride, pointer);
        glEnableVertexAttribArray(index);
    }
}