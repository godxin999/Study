module;
#include <GLFW/glfw3.h>
export module window_manager;

import input_manager.input_state;
import event;
import stl;

namespace Engine::inline Window{
    export class Device;
    export class WindowManager{
    public:
        WindowManager(const Device& device);
        ~WindowManager();

        void SetIcon(const std::u8string& path);
        void SetIconFromMemory(uint8_t* data,uint32_t width,uint32_t height);
        void SetTitle(const std::u8string& title);
        void SetSize(uint16_t width,uint16_t height);
        void SetPosition(int16_t x,int16_t y);
        void SetMinimumSize(int16_t width,int16_t height);
        void SetMaximumSize(int16_t width,int16_t height);
        void SetFullscreen(bool fullscreen);
        void SetCursorMode(CursorMode mode);
        void SetCursorShape(CursorShape shape);
        void SetShouldClose(bool shouldClose)const;
        void SetRefreshRate(int32_t refreshRate){m_refreshRate=refreshRate;}
        
        void Minimize()const;
        void Maximize()const;
        void Restore()const;
        void Focus()const;
        void Hide()const;
        void Show()const;

        bool IsShouldClose()const;
        bool IsMinimized()const;
        bool IsMaximized()const;
        bool IsFocused()const;
        bool IsVisible()const;
        bool IsResizable()const;
        bool IsDecorated()const;
        bool IsFullscreen()const{return m_isFullscreen;}
        
        void MakeCurrentContext()const;
        void LoadGlad()const;
        void SwapBuffers()const;
        void ToggleFullscreen();

        std::pair<uint16_t,uint16_t> GetFramebufferSize()const;
        std::pair<uint16_t,uint16_t> GetSize()const;
        std::pair<int16_t,int16_t> GetPosition()const;
        std::pair<int16_t,int16_t> GetMinimumSize()const{return m_minimumSize;}
        std::pair<int16_t,int16_t> GetMaximumSize()const{return m_maximumSize;}
        CursorShape GetCursorShape()const{return m_cursorShape;}
        CursorMode GetCursorMode()const{return m_cursorMode;}
        int32_t GetRefreshRate()const{return m_refreshRate;}
        GLFWwindow* GetGlfwWindow()const{return m_window;}

        static WindowManager* FindWindowManager(GLFWwindow* window);
    private:
        void CreateGlfwWindow();
        //bind glfw callback
        void BindKeyCallback()const;
        void BindMouseCallback()const;
        void BindResizeCallback()const;
        void BindFrameBufferResizeCallback()const;
        void BindMoveCallback()const;
        void BindCursorMoveCallback()const;
        void BindIconifyCallback()const;
        void BindFocusCallback()const;
        void BindCloseCallback()const;
        
        void UpdateSizeLimit()const;
    public:
        //Input Event
        Event<Key> KeyPressedEvent{};
        Event<Key> KeyReleasedEvent{};
        Event<MouseButton> MouseButtonPressedEvent{};
        Event<MouseButton> MouseButtonReleasedEvent{};
        //Window Event
        Event<uint16_t,uint16_t> ResizeEvent{};
        Event<uint16_t,uint16_t> FrameBufferResizeEvent{};
        Event<int16_t,int16_t> MoveEvent{};
        Event<int16_t,int16_t> CursorMoveEvent{};
        Event<> MinimizeEvent{};
        Event<> MaximizeEvent{};
        Event<> GainFocusEvent{};
        Event<> LostFocusEvent{};
        Event<> CloseEvent{};
    private:
        const Device& m_device;
        GLFWwindow* m_window{nullptr};
        std::u8string m_title{u8"莫名其妙的窗口"};
        std::pair<uint16_t,uint16_t> m_size{1600,900};
        std::pair<int16_t,int16_t> m_position{0,0};
        std::pair<int16_t,int16_t> m_minimumSize{-1,-1};
        std::pair<int16_t,int16_t> m_maximumSize{-1,-1};
        bool m_isFullscreen{false};
        int32_t m_refreshRate{-1};
        CursorMode m_cursorMode{CursorMode::NORMAL};
        CursorShape m_cursorShape{CursorShape::ARROW};

        inline static std::unordered_map<GLFWwindow*,WindowManager*> Windows{};
    };
};