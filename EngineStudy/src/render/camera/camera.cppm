export module camera;

import glm;
import stl;

namespace Engine::inline editor{
    export class CameraController;
}
namespace Engine::inline Render{
    export enum class CameraType{
        PERSPECTIVE,
        ORTHOGRAPHIC
    };
    export class Camera{
    public:
        friend class Engine::editor::CameraController;
        Camera()=default;
        ~Camera()=default;
        void UpdateCameraMatrices(uint16_t width,uint16_t height);
        float GetNearClip();
        float GetFarClip();
        float GetFov();
        float GetSize();
        const glm::vec3& GetClearColor()const;
        const glm::mat4& GetProjMatrix()const;
        const glm::mat4& GetViewMatrix()const;
        const glm::vec3& GetPosition()const;
        const glm::quat& GetRotation()const;
        CameraType GetCameraType()const;
        void SetNearClip(float nearClip);
        void SetFarClip(float farClip);
        void SetFov(float fov);
        void SetSize(float size);
        void SetClearColor(const glm::vec3& clearColor);
        void SetCameraType(CameraType mode);
        void SetPosition(const glm::vec3& position);
        void SetRotation(const glm::quat& rotation);
    private:
        void CalculateProjMatrix(uint16_t width,uint16_t height);
        void CalculateViewMatrix();
    private:
        glm::mat4 m_projMatrix{1.f};
        glm::mat4 m_viewMatrix{1.f};
    
        glm::vec3 m_position{-10.f,3.f,10.f};
        glm::quat m_rotation{glm::vec3(0.f,glm::radians(135.f),0.f)};
        CameraType m_mode{CameraType::PERSPECTIVE};

        float m_nearClip{ 1.f };
        float m_farClip{ 1000.f };
        float m_fov{ 45.f };//竖直方向的视野角度
        float m_size{ 5.f };

        glm::vec3 m_clearColor{0.f};
    };
}