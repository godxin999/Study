export module index_buffer;

import stl;

namespace Engine::inline Render{
    export class IndexBuffer{
    public:
        IndexBuffer(const uint32_t* data, uint32_t count);
        IndexBuffer(const std::vector<uint32_t>& data);
        ~IndexBuffer();
        void Bind();
        void Unbind();
        uint32_t GetID()const{
            return m_bufferID;
        }
    private:
        uint32_t m_bufferID;
    };
}