module;
#include <GLFW/glfw3.h>
module input_manager;

import event;
import window_manager;
import stl;
import glm;

namespace Engine::inline Input{
    InputManager::InputManager(WindowManager& windowManager):m_windowManager(windowManager){
        m_keyPressedListener=m_windowManager.KeyPressedEvent.AddListener([this](Key key){
            m_keyStates[key]=KeyState::KEY_PRESS;
        });
        m_keyReleasedListener=m_windowManager.KeyReleasedEvent.AddListener([this](Key key){
            m_keyStates[key]=KeyState::KEY_RELEASE;
        });
        m_mouseButtonPressedListener=m_windowManager.MouseButtonPressedEvent.AddListener([this](MouseButton button){
            m_mouseButtonStates[button]=MouseButtonState::MOUSE_BUTTON_PRESS;
        });
        m_mouseButtonReleasedListener=m_windowManager.MouseButtonReleasedEvent.AddListener([this](MouseButton button){
            m_mouseButtonStates[button]=MouseButtonState::MOUSE_BUTTON_RELEASE;
        });
    }

    InputManager::~InputManager(){
        m_windowManager.KeyPressedEvent.RemoveListener(m_keyPressedListener);
        m_windowManager.KeyReleasedEvent.RemoveListener(m_keyReleasedListener);
        m_windowManager.MouseButtonPressedEvent.RemoveListener(m_mouseButtonPressedListener);
        m_windowManager.MouseButtonReleasedEvent.RemoveListener(m_mouseButtonReleasedListener);
    }

    
    KeyState InputManager::GetKeyState(Key key) const{
        switch(glfwGetKey(m_windowManager.GetGlfwWindow(),static_cast<int>(key))){
            case GLFW_PRESS: return KeyState::KEY_PRESS;
            case GLFW_RELEASE: return KeyState::KEY_RELEASE;
        }
        return KeyState::KEY_RELEASE;
    }
    
    MouseButtonState InputManager::GetMouseButtonState(MouseButton button) const{
        switch(glfwGetMouseButton(m_windowManager.GetGlfwWindow(),static_cast<int>(button))){
            case GLFW_PRESS: return MouseButtonState::MOUSE_BUTTON_PRESS;
            case GLFW_RELEASE: return MouseButtonState::MOUSE_BUTTON_RELEASE;
        }
        return MouseButtonState::MOUSE_BUTTON_RELEASE;
    }
    
    bool InputManager::IsKeyPressed(Key key) const{
        return m_keyStates.find(key)!=m_keyStates.end()&&m_keyStates.at(key)==KeyState::KEY_PRESS;
    }
    
    bool InputManager::IsMouseButtonPressed(MouseButton button) const{
        return m_mouseButtonStates.find(button)!=m_mouseButtonStates.end()&&m_mouseButtonStates.at(button)==MouseButtonState::MOUSE_BUTTON_PRESS;
    }
    
    bool InputManager::IsKeyReleased(Key key) const{
        return m_keyStates.find(key)!=m_keyStates.end()&&m_keyStates.at(key)==KeyState::KEY_RELEASE;
    }
    
    bool InputManager::IsMouseButtonReleased(MouseButton button) const{
        return m_mouseButtonStates.find(button)!=m_mouseButtonStates.end()&&m_mouseButtonStates.at(button)==MouseButtonState::MOUSE_BUTTON_RELEASE;
    }
    
    glm::vec2 InputManager::GetMousePosition() const{
        double x,y;
        glfwGetCursorPos(m_windowManager.GetGlfwWindow(),&x,&y);
        return {static_cast<float>(x),static_cast<float>(y)};
    }
    
    void InputManager::ClearStates(){
        m_keyStates.clear();
        m_mouseButtonStates.clear();
    }
}