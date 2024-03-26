export module mesh;

import stl;
import buffer;
import glm;

namespace Engine::inline Render{
    export struct Vertex{
        glm::vec3 position;
        glm::vec2 texCoords;
        glm::vec3 normal;
        glm::vec3 tangent;
        glm::vec3 bitangent;
    };
    export class Mesh{
    public:
        Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, uint32_t materialIndex);
        virtual ~Mesh()=default;
        virtual void Bind();
        virtual void Unbind();
        virtual uint32_t GetVertexCount() const;
        virtual uint32_t GetIndexCount() const;
        [[nodiscard]]uint32_t GetMaterialIndex() const;
    private:
        void CreateBuffers(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
    private:
        const uint32_t m_vertexCount;
        const uint32_t m_indexCount;
        const uint32_t m_materialIndex;
        VertexArray m_vertexArray;
        std::unique_ptr<VertexBuffer<float>> m_vertexBuffer;
        std::unique_ptr<IndexBuffer> m_indexBuffer;
    };
}

module : private;

namespace Engine::inline Render{
    Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, uint32_t materialIndex)
        : m_vertexCount(static_cast<uint32_t>(vertices.size())), m_indexCount(static_cast<uint32_t>(indices.size())), m_materialIndex(materialIndex){
        CreateBuffers(vertices, indices);
    }

    void Mesh::Bind(){
        m_vertexArray.Bind();
    }

    void Mesh::Unbind(){
        m_vertexArray.Unbind();
    }

    uint32_t Mesh::GetVertexCount() const{
        return m_vertexCount;
    }

    uint32_t Mesh::GetIndexCount() const{
        return m_indexCount;
    }

    uint32_t Mesh::GetMaterialIndex() const{
        return m_materialIndex;
    }

    void Mesh::CreateBuffers(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices){
        std::vector<float> vertexData;
        for(const auto& v:vertices){
            vertexData.push_back(v.position.x);
            vertexData.push_back(v.position.y);
            vertexData.push_back(v.position.z);
            vertexData.push_back(v.texCoords.x);
            vertexData.push_back(v.texCoords.y);
            vertexData.push_back(v.normal.x);
            vertexData.push_back(v.normal.y);
            vertexData.push_back(v.normal.z);
            vertexData.push_back(v.tangent.x);
            vertexData.push_back(v.tangent.y);
            vertexData.push_back(v.tangent.z);
            vertexData.push_back(v.bitangent.x);
            vertexData.push_back(v.bitangent.y);
            vertexData.push_back(v.bitangent.z);
        }
        m_vertexBuffer = std::make_unique<VertexBuffer<float>>(vertexData);
        m_indexBuffer = std::make_unique<IndexBuffer>(indices);
        m_vertexArray.BindAttribute(0, 3, DataType::FLOAT, false, 14 * sizeof(float), (void*)0);
        m_vertexArray.BindAttribute(1, 2, DataType::FLOAT, false, 14 * sizeof(float), (void*)(3 * sizeof(float)));
        m_vertexArray.BindAttribute(2, 3, DataType::FLOAT, false, 14 * sizeof(float), (void*)(5 * sizeof(float)));
        m_vertexArray.BindAttribute(3, 3, DataType::FLOAT, false, 14 * sizeof(float), (void*)(8 * sizeof(float)));
        m_vertexArray.BindAttribute(4, 3, DataType::FLOAT, false, 14 * sizeof(float), (void*)(11 * sizeof(float)));
    }
}