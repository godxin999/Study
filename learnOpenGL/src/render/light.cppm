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
        Light(Transform& transform,LightType p_type):m_Transform(transform),type(static_cast<float>(p_type)){}
        ~Light() = default;
        [[nodiscard]]glm::mat4 GenerateDataMatrix()const;
        [[nodiscard]]float GetEffectRange()const;
        [[nodiscard]]const Transform& GetTransform()const{
            return m_Transform;
        }
    public:
        glm::vec3 color{1.f};
        glm::vec3 attCoeff{1.f, 0.f, 0.f};
        float intensity{1.f};
        float type{0};
        float cutoff{12.f};
        float outerCutoff{15.f};
    private:
        Transform& m_Transform;
    };
}
