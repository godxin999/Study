export module model;

import stl;
import buffer;
import glm;
import mesh;

namespace Engine::inline Render{
    export class ModelLoader;
    export class Model{
    public:
        friend class ModelLoader;
        [[nodiscard]]const std::vector<std::unique_ptr<Mesh>>& GetMeshes()const;
        [[nodiscard]]const std::vector<std::u8string>& GetMaterialID()const;
    private:
        Model(const std::u8string& path);
        ~Model()=default;
    public:
        const std::u8string path;
    private:
        std::vector<std::unique_ptr<Mesh>> m_meshes;
        std::vector<std::u8string> m_materialID;
    };
}

module : private;

namespace Engine::inline Render{
    Model::Model(const std::u8string& path):path(path){

    }

    const std::vector<std::unique_ptr<Mesh>>& Model::GetMeshes() const{
        return m_meshes;
    }

    const std::vector<std::u8string>& Model::GetMaterialID() const{
        return m_materialID;
    }
}