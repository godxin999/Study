module camera;

import glm;
import input_manager;
import input_state;
import stl;
import service_locator;
import window_manager;

namespace Engine::inline Render{
    Camera::Camera(float fov,float aspect,float nearClip,float farClip):m_Fov(fov),m_Aspect(aspect),m_NearClip(nearClip),m_FarClip(farClip),m_WindowManager(ServiceLocator::Get<WindowManager>()),m_InputManager(ServiceLocator::Get<InputManager>()){
        m_Distance=glm::distance(m_Position,m_FocalPoint);
        m_ViewMatrix=glm::lookAt(m_Position,m_FocalPoint,glm::vec3(0.f,1.f,0.f));
        m_WorldToCameraRotation=glm::quat_cast(m_ViewMatrix);
        m_CameraToWorldRotation=glm::inverse(m_WorldToCameraRotation);
        UpdateViewMatrix();
        UpdateProjMatrix();
    }
    void Camera::Update(){
        //Zoom((10-Input::GetMouseScroll())/10.f);
        if(m_InputManager.IsKeyPressed(Key::KEY_LEFT_ALT) && m_InputManager.IsMouseButtonPressed(MouseButton::MOUSE_BUTTON_LEFT)){
            glm::vec2 MousePosition=m_InputManager.GetMousePosition();
            if(MousePosition!=m_LastMousePosition){
                Rotate(m_LastMousePosition,MousePosition);
                m_LastMousePosition = MousePosition;
            }
        }
        else{
            m_LastMousePosition=m_InputManager.GetMousePosition();
        }
        UpdateViewMatrix();
        UpdateProjMatrix();
    }
    void Camera::SetFov(float fov){
        m_Fov=fov;
    }
    void Camera::SetPostion(const glm::vec3& pos){
        m_Position=pos;
        m_Distance=glm::distance(m_Position,m_FocalPoint);
    }
    void Camera::SetTarget(const glm::vec3& target){
        m_FocalPoint=target;
        m_Distance=glm::distance(m_Position,m_FocalPoint);
    }
    void Camera::Zoom(float delta){
        m_Distance=std::clamp(m_Distance*delta,1.f,100.f);
    }
    void Camera::Rotate(const glm::vec2& lastPos,const glm::vec2& currentPos){
        auto va=GetArcballVector(lastPos);
        auto vb=GetArcballVector(currentPos);
        float angle=std::acos(std::clamp(glm::dot(va,vb),-1.f,1.f));
        glm::vec3 axisInCameraSpace=glm::normalize(glm::cross(va,vb));
        m_CameraToWorldRotation=glm::angleAxis(angle,axisInCameraSpace)*m_CameraToWorldRotation;
        m_WorldToCameraRotation=glm::inverse(m_CameraToWorldRotation);
    }
    glm::vec3 Camera::GetUpDirection()const{
        return m_WorldToCameraRotation*glm::vec3(0.f,1.f,0.f);
    }
    glm::vec3 Camera::GetForwardDirection()const{
        return m_WorldToCameraRotation*glm::vec3(0.f,0.f,-1.f);
    }
    glm::vec3 Camera::GetRightDirection()const{
        return m_WorldToCameraRotation*glm::vec3(1.f,0.f,0.f);
    }
    void Camera::UpdateProjMatrix(){
        auto [width,height]=m_WindowManager.GetSize();
        m_Aspect=1.f*width/height;
        m_ProjMatrix=glm::perspective(glm::radians(m_Fov),m_Aspect,m_NearClip,m_FarClip);
    }
    void Camera::UpdateViewMatrix(){
        m_Position=CalculatePosition();
        m_ViewMatrix=glm::translate(glm::mat4(1.f),m_Position)*glm::mat4_cast(m_WorldToCameraRotation);
        m_ViewMatrix=glm::inverse(m_ViewMatrix);
    }

    glm::vec3 Camera::GetArcballVector(const glm::vec2& point){
        auto [width,height]=m_WindowManager.GetSize();
        glm::vec3 P=glm::vec3(1.f*point.x/width*2.f-1.f,1.f*point.y/height*2.f-1.f,0.f);
        P.y=-P.y;
        float OP_squared=P.x*P.x+P.y*P.y;
        if(OP_squared<=1.f){
            P.z=std::sqrt(1.f-OP_squared);
        }
        else{
            P=glm::normalize(P);
        }
        return P;
    }

    glm::vec3 Camera::CalculatePosition()const{
        return m_FocalPoint-GetForwardDirection()*m_Distance;
    }
}

