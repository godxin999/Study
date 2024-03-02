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
        [[nodiscard]]KeyState GetKeyState(Key key) const;
        [[nodiscard]]MouseButtonState GetMouseButtonState(MouseButton button) const;
        [[nodiscard]]bool IsKeyPressed(Key key) const;
        [[nodiscard]]bool IsMouseButtonPressed(MouseButton button) const;
        [[nodiscard]]bool IsKeyReleased(Key key) const;
        [[nodiscard]]bool IsMouseButtonReleased(MouseButton button) const;
        [[nodiscard]]glm::vec2 GetMousePosition() const;
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