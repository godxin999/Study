module;
#include <glad/glad.h>
export module vertex_buffer;

import stl;

namespace Engine::inline Render{
    export template <typename T>
    class VertexBuffer{
    public:
        VertexBuffer(const T* data, uint32_t count);
        VertexBuffer(const std::vector<T>& data);
        ~VertexBuffer();
        void Bind() const;
        void Unbind() const;
        uint32_t GetID()const{
            return m_bufferID;
        }
    private:
        uint32_t m_bufferID;
    };
}

module : private;

namespace Engine::inline Render{
    template <typename T>
    VertexBuffer<T>::VertexBuffer(const T* data, uint32_t count){
        glGenBuffers(1,&m_bufferID);
        glBindBuffer(GL_ARRAY_BUFFER,m_bufferID);
        glBufferData(GL_ARRAY_BUFFER,count * sizeof(T),data,GL_STATIC_DRAW);
    }

    template <typename T>
    VertexBuffer<T>::VertexBuffer(const std::vector<T>& data):VertexBuffer(data.data(),data.size()){

    }

    template <typename T>
    VertexBuffer<T>::~VertexBuffer(){
        glDeleteBuffers(1,&m_bufferID);
    }

    template <typename T>
    void VertexBuffer<T>::Bind() const{
        glBindBuffer(GL_ARRAY_BUFFER,m_bufferID);
    }
    
    template <typename T>
    void VertexBuffer<T>::Unbind() const{
        glBindBuffer(GL_ARRAY_BUFFER,0);
    }
}