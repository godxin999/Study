export module buffer.vertex_buffer;

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
            return m_BufferID;
        }
    private:
        uint32_t m_BufferID;
    };
    export template class VertexBuffer<float>;
}