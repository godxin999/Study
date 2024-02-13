export module camera;

import glm;

export class Camera{
public:
    Camera(float fov,float aspect,float nearClip,float farClip);
    void Update();
    const glm::mat4& GetProjMatrix()const{return m_ProjMatrix;}
    const glm::mat4& GetViewMatrix()const{return m_ViewMatrix;}
    const glm::vec3& GetPostion()const{return m_Position;}
    float GetFov()const{return m_Fov;}
    float GetAspect()const{return m_Aspect;}
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

    glm::mat4 m_ProjMatrix{1.f};
    glm::mat4 m_ViewMatrix{1.f};

    glm::vec3 m_Position{0.f,0.f,10.f};
    glm::vec3 m_FocalPoint{0.f,0.f,0.f};
    //glm::vec3 m_FocalPoint{1.5f,0.2f,-1.5f};
    glm::quat m_CameraToWorldRotation{1.f,0.f,0.f,0.f};
    glm::quat m_WorldToCameraRotation{1.f,0.f,0.f,0.f};
    
    glm::vec2 m_LastMousePosition{0.f,0.f};

    float m_NearClip{ 1.f };
    float m_FarClip{ 1000.f };
    float m_Fov{ 45.f };//竖直方向的视野角度，单位为弧度
    float m_Aspect{ 1.778f };
    float m_Distance{ 10.f };
};