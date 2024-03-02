export module light;

import glm;
import stl;
import transform;

namespace Engine::inline Render{
    export enum class LightType: uint32_t{
        Directional,
        Point,
        Spot,
        AmbientSphere
    };
    export class Light{
    public:
        Light(Transform& transform,LightType type):m_Transform(transform),m_Type(static_cast<float>(type)){}
        ~Light() = default;
        [[nodiscard]]glm::mat4 GenerateDataMatrix()const;
        [[nodiscard]]float GetEffectRange()const;
        [[nodiscard]]const Transform& GetTransform()const{
            return m_Transform;
        }
    public:
        glm::vec3 m_Color{1.f};
        glm::vec3 m_AttCoeff{1.f, 0.f, 0.f};
        float m_Intensity{1.f};
        float m_Type{0};
        float m_Cutoff{12.f};
        float m_OuterCutoff{15.f};
    private:
        Transform& m_Transform;
    };
}
