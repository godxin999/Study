export module camera;

import glm;

export class Camera{
public:
    Camera(float fov,float aspect,float nearClip,float farClip);
    
private:
    glm::vec3 m_Position{0.f,0.f,0.f};
    glm::quat m_Rotation{1.f,0.f,0.f,0.f};
    glm::quat m_InvRotation{1.f,0.f,0.f,0.f};
    float m_NearClip{ 1.f };
    float m_FarClip{ 1000.f };
    float m_Fov{ 45.f };
    float m_Aspect{ 0.f };
};