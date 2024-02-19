export module input_manager;
export import input_manager.input_state;

import event;
import window_manager;
import stl;
import glm;

namespace Engine::inline Input{
    export class InputManager{
    public:
        InputManager(WindowManager& windowManager);
        ~InputManager();
        KeyState GetKeyState(Key key) const;
        MouseButtonState GetMouseButtonState(MouseButton button) const;
        bool IsKeyPressed(Key key) const;
        bool IsMouseButtonPressed(MouseButton button) const;
        bool IsKeyReleased(Key key) const;
        bool IsMouseButtonReleased(MouseButton button) const;
        glm::vec2 GetMousePosition() const;
        void ClearStates();
    private:
        WindowManager& m_WindowManager;
        ListenerID m_KeyPressedListener;
        ListenerID m_KeyReleasedListener;
        ListenerID m_MouseButtonPressedListener;
        ListenerID m_MouseButtonReleasedListener;
        std::unordered_map<Key,KeyState> m_KeyStates;
        std::unordered_map<MouseButton,MouseButtonState> m_MouseButtonStates;
    };
}