export module input_manager;
export import input_manager.input_state;

import event;
import window_manager;
import stl;
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
        [[nodiscard]]std::pair<double, double> GetMousePosition() const;
        void ClearStates();
    private:
        WindowManager& m_windowManager;
        ListenerID m_keyPressedListener;
        ListenerID m_keyReleasedListener;
        ListenerID m_mouseButtonPressedListener;
        ListenerID m_mouseButtonReleasedListener;
        std::unordered_map<Key,KeyState> m_keyStates;
        std::unordered_map<MouseButton,MouseButtonState> m_mouseButtonStates;
    };
}