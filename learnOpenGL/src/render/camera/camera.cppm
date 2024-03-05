export module camera;

import glm;
import window_manager;
import input_manager;

namespace Engine::inline Render{
    export class Camera{
    public:
        Camera(float fov,float aspect,float nearClip,float farClip);
        void Update();
        const glm::mat4& GetProjMatrix()const{return m_projMatrix;}
        const glm::mat4& GetViewMatrix()const{return m_viewMatrix;}
        const glm::vec3& GetPosition()const{return m_position;}
        float GetFov()const{return m_fov;}
        float GetAspect()const{return m_aspect;}
        void SetFov(float fov);
        void SetPostion(const glm::vec3& pos);
        void SetTarget(const glm::vec3& target);
        void Zoom(float delta);
        void Rotate(const glm::vec2& lastPos,const glm::vec2& currentPos);
        glm::vec3 GetUpDirection()const;
        glm::vec3 GetForwardDirection()const;
        glm::vec3 GetRightDirection()const;
    private:
        void UpdateProjMatrix();
        void UpdateViewMatrix();
        glm::vec3 GetArcballVector(const glm::vec2& point);
        glm::vec3 CalculatePosition()const;
    
        glm::mat4 m_projMatrix{1.f};
        glm::mat4 m_viewMatrix{1.f};
    
        glm::vec3 m_position{0.f,0.f,10.f};
        glm::vec3 m_focalPoint{0.f,0.f,0.f};
        //glm::vec3 m_focalPoint{1.5f,0.2f,-1.5f};
        glm::quat m_cameraToWorldRotation{1.f,0.f,0.f,0.f};
        glm::quat m_worldToCameraRotation{1.f,0.f,0.f,0.f};
        
        glm::vec2 m_lastMousePosition{0.f,0.f};
    
        float m_nearClip{ 1.f };
        float m_farClip{ 1000.f };
        float m_fov{ 45.f };//竖直方向的视野角度，单位为弧度
        float m_aspect{ 1.778f };
        float m_distance{ 10.f };
    
        WindowManager& m_windowManager;
        InputManager& m_inputManager;
    };

}
