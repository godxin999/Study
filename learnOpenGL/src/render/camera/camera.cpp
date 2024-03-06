module camera;

import glm;
import stl;
import transform;


namespace Engine::inline Render{
    void Camera::UpdateCameraMatrices(uint16_t width,uint16_t height){
        CalculateProjMatrix(width,height);
        CalculateViewMatrix();
    }

    float Camera::GetNearClip(){
        return m_nearClip;
    }

    float Camera::GetFarClip(){
        return m_farClip;
    }

    float Camera::GetFov(){
        return m_fov;
    }

    float Camera::GetSize(){
        return m_size;
    }

    const glm::vec3& Camera::GetClearColor()const{
        return m_clearColor;
    }

    const glm::mat4& Camera::GetProjMatrix()const{
        return m_projMatrix;
    }

    const glm::mat4& Camera::GetViewMatrix()const{
        return m_viewMatrix;
    }

    const glm::vec3& Camera::GetPosition()const{
        return m_position;
    }

    const glm::quat& Camera::GetRotation()const{
        return m_rotation;
    }

    CameraType Camera::GetCameraType()const{
        return m_mode;
    }

    void Camera::SetNearClip(float nearClip){
        m_nearClip = nearClip;
    }

    void Camera::SetFarClip(float farClip){
        m_farClip = farClip;
    }
    
    void Camera::SetFov(float fov){
        m_fov = fov;
    }
    
    void Camera::SetSize(float size){
        m_size = size;
    }
    
    void Camera::SetClearColor(const glm::vec3& clearColor){
        m_clearColor = clearColor;
    }
    
    void Camera::SetCameraType(CameraType mode){
        m_mode = mode;
    }
    
    void Camera::SetPosition(const glm::vec3& position){
        m_position = position;
    }
    
    void Camera::SetRotation(const glm::quat& rotation){
        m_rotation = rotation;
    }
    
    void Camera::CalculateProjMatrix(uint16_t width,uint16_t height){
        const float aspect=width/static_cast<float>(height);
        if(m_mode==CameraType::PERSPECTIVE){
            m_projMatrix=glm::perspective(glm::radians(m_fov),aspect,m_nearClip,m_farClip);
        }
        else{
            //left,right,bottom,top,near,far
            //m_size是正交投影视距高度的一半
            m_projMatrix=glm::ortho(-m_size*aspect,m_size*aspect,-m_size,m_size,m_nearClip,m_farClip);
        }
    }
    
    void Camera::CalculateViewMatrix(){
        m_viewMatrix=glm::lookAt(m_position,m_position+glm::rotate(m_rotation,Engine::Forward),glm::rotate(m_rotation,Engine::Up));
    }
}