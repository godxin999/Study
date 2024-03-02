export module texture;
import stl;

namespace Engine::inline Render{
    export class Texture{
    public:
        Texture()=default;
        void LoadTexture(const std::string& texturePath);
        void Bind(uint32_t slot) const;
        void Unbind() const;
        ~Texture()=default;
        uint32_t m_TextureID{0};
    };
}