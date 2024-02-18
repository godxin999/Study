module;
#include <GLFW/glfw3.h>
module input_manager;

import event;
import window_manager;
import stl;
import glm;

namespace Engine::inline Input{
    InputManager::InputManager(WindowManager& windowManager):m_WindowManager(windowManager){
        m_KeyPressedListener=m_WindowManager.KeyPressedEvent.AddListener(std::bind(&InputManager::OnKeyPressed,this,std::placeholders::_1));
        m_KeyReleasedListener=m_WindowManager.KeyReleasedEvent.AddListener(std::bind(&InputManager::OnKeyReleased,this,std::placeholders::_1));
        m_MouseButtonPressedListener=m_WindowManager.MouseButtonPressedEvent.AddListener(std::bind(&InputManager::OnMouseButtonPressed,this,std::placeholders::_1));
        m_MouseButtonReleasedListener=m_WindowManager.MouseButtonReleasedEvent.AddListener(std::bind(&InputManager::OnMouseButtonReleased,this,std::placeholders::_1));
    }

    InputManager::~InputManager(){
        m_WindowManager.KeyPressedEvent.RemoveListener(m_KeyPressedListener);
        m_WindowManager.KeyReleasedEvent.RemoveListener(m_KeyReleasedListener);
        m_WindowManager.MouseButtonPressedEvent.RemoveListener(m_MouseButtonPressedListener);
        m_WindowManager.MouseButtonReleasedEvent.RemoveListener(m_MouseButtonReleasedListener);
    }

    KeyState InputManager::GetKeyState(Key key) const{
        switch(glfwGetKey(m_WindowManager.GetGlfwWindow(),static_cast<int>(key))){
            case GLFW_PRESS: return KeyState::KEY_PRESS;
            case GLFW_RELEASE: return KeyState::KEY_RELEASE;
        }
        return KeyState::KEY_RELEASE;
    }
    MouseButtonState InputManager::GetMouseButtonState(MouseButton button) const{
        switch(glfwGetMouseButton(m_WindowManager.GetGlfwWindow(),static_cast<int>(button))){
            case GLFW_PRESS: return MouseButtonState::MOUSE_BUTTON_PRESS;
            case GLFW_RELEASE: return MouseButtonState::MOUSE_BUTTON_RELEASE;
        }
        return MouseButtonState::MOUSE_BUTTON_RELEASE;
    }
    bool InputManager::IsKeyPressed(Key key) const{
        return m_KeyStates.find(key)!=m_KeyStates.end()&&m_KeyStates.at(key)==KeyState::KEY_PRESS;
    }
    bool InputManager::IsMouseButtonPressed(MouseButton button) const{
        return m_MouseButtonStates.find(button)!=m_MouseButtonStates.end()&&m_MouseButtonStates.at(button)==MouseButtonState::MOUSE_BUTTON_PRESS;
    }
    bool InputManager::IsKeyReleased(Key key) const{
        return m_KeyStates.find(key)!=m_KeyStates.end()&&m_KeyStates.at(key)==KeyState::KEY_RELEASE;
    }
    bool InputManager::IsMouseButtonReleased(MouseButton button) const{
        return m_MouseButtonStates.find(button)!=m_MouseButtonStates.end()&&m_MouseButtonStates.at(button)==MouseButtonState::MOUSE_BUTTON_RELEASE;
    }
    glm::vec2 InputManager::GetMousePosition() const{
        double x,y;
        glfwGetCursorPos(m_WindowManager.GetGlfwWindow(),&x,&y);
        return {static_cast<float>(x),static_cast<float>(y)};
    }
    void InputManager::ClearStates(){
        m_KeyStates.clear();
        m_MouseButtonStates.clear();
    }
    void InputManager::OnKeyPressed(Key key){
        m_KeyStates[key]=KeyState::KEY_PRESS;
    }
    void InputManager::OnKeyReleased(Key key){
        m_KeyStates[key]=KeyState::KEY_RELEASE;
    }
    void InputManager::OnMouseButtonPressed(MouseButton button){
        m_MouseButtonStates[button]=MouseButtonState::MOUSE_BUTTON_PRESS;
    }
    void InputManager::OnMouseButtonReleased(MouseButton button){
        m_MouseButtonStates[button]=MouseButtonState::MOUSE_BUTTON_RELEASE;
    }
}