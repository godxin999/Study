module;
#include "control/key_code.h"
module camera;

import glm;
import input_system;
import global_context.log_system;
import <format>;

Camera::Camera(float fov,float aspect,float nearClip,float farClip):m_Fov(fov),m_Aspect(aspect),m_NearClip(nearClip),m_FarClip(farClip){
    UpdateViewMatrix();
    UpdateProjMatrix();
}
void Camera::Update(){
    Zoom(1-((10-Input::GetMouseScroll())/10.f));
    if(Input::GetKeyDown(KEY_CODE_LEFT_ALT) && Input::GetMouseButtonDown(MOUSE_BUTTON_LEFT)){
        const auto MousePosition=Input::GetMousePosition();
        glm::vec2 delta = (MousePosition-m_LastMousePosition)*.03f;
        m_LastMousePosition = MousePosition;
        Rotate(delta.x,glm::vec3(0.f,1.f,0.f));
                RG_LogSystem->log(log_level::info,std::format("Camera position:({},{},{})",m_Position.x,m_Position.y,m_Position.z));
        RG_LogSystem->log(log_level::info,std::format("Focus position:({},{},{})",m_FocalPoint.x,m_FocalPoint.y,m_FocalPoint.z));
    }
    else{
        m_LastMousePosition=Input::GetMousePosition();
    }
    UpdateViewMatrix();
}
void Camera::SetAspect(float aspect){
    m_Aspect=aspect;
    UpdateProjMatrix();
}
void Camera::SetFov(float fov){
    m_Fov=fov;
    UpdateProjMatrix();
}
void Camera::SetPostion(const glm::vec3& pos){
    m_Position=pos;
}
void Camera::SetTarget(const glm::vec3& target){
    m_FocalPoint=target;
}
void Camera::Zoom(float delta){
    m_Position+=m_InvRotation*glm::vec3(0.f,0.f,delta);
    m_FocalPoint=m_Position+m_InvRotation*glm::vec3(0.f,0.f,-1.f);
}
void Camera::Rotate(float degree,const glm::vec3& axis){
    glm::quat rot=glm::angleAxis(glm::radians(degree),axis);
    m_Rotation=rot*m_Rotation;
    m_Rotation=glm::normalize(m_Rotation);
    m_InvRotation=glm::inverse(m_Rotation);
    m_FocalPoint=m_Position+m_InvRotation*glm::vec3(0.f,0.f,-1.f);
}
glm::vec3 Camera::GetUpDirection()const{
    return m_InvRotation*glm::vec3(0.f,1.f,0.f);
}
glm::vec3 Camera::GetForwardDirection()const{
    return m_InvRotation*glm::vec3(0.f,0.f,-1.f);
}
glm::vec3 Camera::GetRightDirection()const{
    return m_InvRotation*glm::vec3(1.f,0.f,0.f);
}
void Camera::UpdateProjMatrix(){
    m_ProjMatrix=glm::perspective(glm::radians(m_Fov),m_Aspect,m_NearClip,m_FarClip);
}
void Camera::UpdateViewMatrix(){
    m_ViewMatrix=glm::lookAt(m_Position,m_Position+GetForwardDirection(),glm::vec3(0.f,1.f,0.f));
}
    