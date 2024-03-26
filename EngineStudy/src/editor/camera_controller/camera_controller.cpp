module camera_controller;

import glm;
import camera;
import input_manager;
import window_manager;
import stl;
import transform;
import service_locator;
import imgui;

namespace Engine::inline editor{
    CameraController::CameraController(Camera& camera,bool enableFocusInput):m_camera(camera),m_inputManager(ServiceLocator::Get<InputManager>()),m_windowManager(ServiceLocator::Get<WindowManager>()),m_enableFocusInput(enableFocusInput){

    }
    
    void CameraController::HandleInput(float deltaTime){

    }

    void CameraController::SetSpeed(float speed){
        m_cameraMoveSpeed=speed;
    }
    
    float CameraController::GetSpeed()const{
        return m_cameraMoveSpeed;
    }
    
    void CameraController::HandleCameraPan(glm::vec2 offset,bool firstMouse){
        m_windowManager.SetCursorShape(CursorShape::HAND);
        offset*=m_cameraDragSpeed;
        //glfw窗口的(0,0)点在左上角
        m_camera.m_position+=m_camera.m_rotation*Right*offset.x;
        m_camera.m_position-=m_camera.m_rotation*Up*offset.y;
    }
    
    void CameraController::HandleCameraOrbit(glm::vec2 offset,bool firstMouse){

    }
    
    void CameraController::HandleCameraFPSMouse(glm::vec2 offset,bool firstMouse){

    }
    
    void CameraController::HandleCameraZoom(){
        m_camera.m_position+=m_camera.m_rotation*Forward*ImGui::GetIO().MouseWheel;
    }
    
    void CameraController::HandleCameraFPSKeyboard(float deltaTime){

    }
    
    void CameraController::UpdateMouseState(){
        m_leftMouseButtonPressed=m_inputManager.IsMouseButtonPressed(MouseButton::MOUSE_BUTTON_LEFT);
        m_rightMouseButtonPressed=m_inputManager.IsMouseButtonPressed(MouseButton::MOUSE_BUTTON_RIGHT);
        m_middleMouseButtonPressed=m_inputManager.IsMouseButtonPressed(MouseButton::MOUSE_BUTTON_MIDDLE);
        if(!m_leftMouseButtonPressed||!m_middleMouseButtonPressed){
            m_firstMouse=true;
        }
        if(m_rightMouseButtonPressed){
            m_windowManager.SetCursorMode(CursorMode::DISABLED);
        }
        else{
            m_windowManager.SetCursorMode(CursorMode::NORMAL);
            m_firstMouse=true;
        }
    }
}