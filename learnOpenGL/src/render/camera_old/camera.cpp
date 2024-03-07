module camera_old;

import glm;
import input_manager;
import stl;
import service_locator;
import window_manager;

namespace Engine::inline Render{
    Camera::Camera(float fov,float aspect,float nearClip,float farClip):m_fov(fov),m_aspect(aspect),m_nearClip(nearClip),m_farClip(farClip),m_windowManager(ServiceLocator::Get<WindowManager>()),m_inputManager(ServiceLocator::Get<InputManager>()){
        m_distance=glm::distance(m_position,m_focalPoint);
        m_viewMatrix=glm::lookAt(m_position,m_focalPoint,glm::vec3(0.f,1.f,0.f));
        m_worldToCameraRotation=glm::quat_cast(m_viewMatrix);
        m_cameraToWorldRotation=glm::inverse(m_worldToCameraRotation);
        UpdateViewMatrix();
        UpdateProjMatrix();
    }
    void Camera::Update(){
        //Zoom((10-Input::GetMouseScroll())/10.f);
        if(m_inputManager.IsKeyPressed(Key::KEY_LEFT_ALT) && m_inputManager.IsMouseButtonPressed(MouseButton::MOUSE_BUTTON_LEFT)){
            /*glm::vec2 MousePosition=m_inputManager.GetMousePosition();
            if(MousePosition!=m_lastMousePosition){
                Rotate(m_lastMousePosition,MousePosition);
                m_lastMousePosition = MousePosition;
            }*/
        }
        else{
            //m_lastMousePosition=m_inputManager.GetMousePosition();
        }
        UpdateViewMatrix();
        UpdateProjMatrix();
    }
    void Camera::SetFov(float fov){
        m_fov=fov;
    }
    void Camera::SetPostion(const glm::vec3& pos){
        m_position=pos;
        m_distance=glm::distance(m_position,m_focalPoint);
    }
    void Camera::SetTarget(const glm::vec3& target){
        m_focalPoint=target;
        m_distance=glm::distance(m_position,m_focalPoint);
    }
    void Camera::Zoom(float delta){
        m_distance=std::clamp(m_distance*delta,1.f,100.f);
    }
    void Camera::Rotate(const glm::vec2& lastPos,const glm::vec2& currentPos){
        auto va=GetArcballVector(lastPos);
        auto vb=GetArcballVector(currentPos);
        float radians=std::acos(std::clamp(glm::dot(va,vb),-1.f,1.f));
        glm::vec3 axisInCameraSpace=glm::normalize(glm::cross(va,vb));
        m_cameraToWorldRotation=glm::angleAxis(radians,axisInCameraSpace)*m_cameraToWorldRotation;
        m_worldToCameraRotation=glm::inverse(m_cameraToWorldRotation);
    }
    glm::vec3 Camera::GetUpDirection()const{
        return m_worldToCameraRotation*glm::vec3(0.f,1.f,0.f);
    }
    glm::vec3 Camera::GetForwardDirection()const{
        return m_worldToCameraRotation*glm::vec3(0.f,0.f,-1.f);
    }
    glm::vec3 Camera::GetRightDirection()const{
        return m_worldToCameraRotation*glm::vec3(1.f,0.f,0.f);
    }
    void Camera::UpdateProjMatrix(){
        auto [width,height]=m_windowManager.GetSize();
        m_aspect=1.f*width/height;
        m_projMatrix=glm::perspective(glm::radians(m_fov),m_aspect,m_nearClip,m_farClip);
    }
    void Camera::UpdateViewMatrix(){
        m_position=CalculatePosition();
        m_viewMatrix=glm::translate(glm::mat4(1.f),m_position)*glm::mat4_cast(m_worldToCameraRotation);
        m_viewMatrix=glm::inverse(m_viewMatrix);
    }

    glm::vec3 Camera::GetArcballVector(const glm::vec2& point){
        auto [width,height]=m_windowManager.GetSize();
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
        return m_focalPoint-GetForwardDirection()*m_distance;
    }
}

