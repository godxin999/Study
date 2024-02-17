module;
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
module window_manager;

import input_state;
import event;
import stl;
import log_manager;
import service_locator;

namespace Engine::inline Window{
    WindowManager::WindowManager(const Device& device):m_Device(device){
        CreateGlfwWindow();
        SetCursorMode(CursorMode::NORMAL);
        SetCursorShape(CursorShape::ARROW);

        BindKeyCallback();
        BindMouseCallback();
        BindResizeCallback();
        BindFrameBufferResizeCallback();
        BindMoveCallback();
        BindCursorMoveCallback();
        BindIconifyCallback();
        BindFocusCallback();
        BindCloseCallback();

        ResizeEvent+=std::bind(&WindowManager::OnResize,this,std::placeholders::_1,std::placeholders::_2);
        FrameBufferResizeEvent+=std::bind(&WindowManager::OnFrameBufferResize,this,std::placeholders::_1,std::placeholders::_2);
        MoveEvent+=std::bind(&WindowManager::OnMove,this,std::placeholders::_1,std::placeholders::_2);
        CloseEvent+=std::bind(&WindowManager::OnClose,this);
    }
    WindowManager::~WindowManager(){
        Windows.erase(m_Window);
        glfwDestroyWindow(m_Window);
    }
    void WindowManager::SetIcon(const std::string& path){}
    void WindowManager::SetIconFromMemory(uint8_t* data,uint32_t width,uint32_t height){}
    void WindowManager::SetTitle(const std::string& title){
        m_Title=title;
        glfwSetWindowTitle(m_Window,m_Title.c_str());
    }
    void WindowManager::SetSize(uint16_t width,uint16_t height){
        glfwSetWindowSize(m_Window,static_cast<int>(width),static_cast<int>(height));
    }
    void WindowManager::SetPosition(int16_t x,int16_t y){
        glfwSetWindowPos(m_Window,static_cast<int>(x),static_cast<int>(y));
    }
    void WindowManager::SetMinimumSize(int16_t width,int16_t height){
        m_MinimumSize.first=width;
        m_MinimumSize.second=height;
        UpdateSizeLimit();
    }
    void WindowManager::SetMaximumSize(int16_t width,int16_t height){
        m_MaximumSize.first=width;
        m_MaximumSize.second=height;
        UpdateSizeLimit();
    }
    void WindowManager::SetFullscreen(bool fullscreen){
        m_IsFullscreen=fullscreen;
        glfwSetWindowMonitor(m_Window,m_IsFullscreen?glfwGetPrimaryMonitor():nullptr,static_cast<int>(m_Position.first),static_cast<int>(m_Position.second),static_cast<int>(m_Size.first),static_cast<int>(m_Size.second),m_RefreshRate);   
    }
    void WindowManager::SetCursorMode(CursorMode mode){
        m_CursorMode=mode;
        glfwSetInputMode(m_Window,GLFW_CURSOR,static_cast<int>(m_CursorMode));
    }
    void WindowManager::SetCursorShape(CursorShape shape){
        m_CursorShape=shape;
        glfwSetCursor(m_Window,m_Device.GetCursorInstance(m_CursorShape));
    }
    void WindowManager::SetShouldClose(bool shouldClose)const{
        glfwSetWindowShouldClose(m_Window,shouldClose);
    }
    void WindowManager::Minimize()const{
        glfwIconifyWindow(m_Window);
    }
    void WindowManager::Maximize()const{
        glfwMaximizeWindow(m_Window);
    }
    void WindowManager::Restore()const{
        glfwRestoreWindow(m_Window);
    }
    void WindowManager::Focus()const{
        glfwFocusWindow(m_Window);
    }
    void WindowManager::Hide()const{
        glfwHideWindow(m_Window);
    }
    void WindowManager::Show()const{
        glfwShowWindow(m_Window);
    }
    void WindowManager::ToggleFullscreen(){
        SetFullscreen(!m_IsFullscreen);
    }
    bool WindowManager::IsShouldClose()const{
        return glfwWindowShouldClose(m_Window);
    }
    bool WindowManager::IsMinimized()const{
        return glfwGetWindowAttrib(m_Window,GLFW_ICONIFIED);
    }
    bool WindowManager::IsMaximized()const{
        return glfwGetWindowAttrib(m_Window,GLFW_MAXIMIZED);
    }
    bool WindowManager::IsFocused()const{
        return glfwGetWindowAttrib(m_Window,GLFW_FOCUSED);
    }
    bool WindowManager::IsVisible()const{
        return glfwGetWindowAttrib(m_Window,GLFW_VISIBLE);
    }
    bool WindowManager::IsResizable()const{
        return glfwGetWindowAttrib(m_Window,GLFW_RESIZABLE);
    }
    bool WindowManager::IsDecorated()const{
        return glfwGetWindowAttrib(m_Window,GLFW_DECORATED);
    }
    void WindowManager::MakeCurrentContext()const{
        glfwMakeContextCurrent(m_Window);
    }
    void WindowManager::SwapBuffers()const{
        glfwSwapBuffers(m_Window);
    }
    std::pair<uint16_t,uint16_t> WindowManager::GetFramebufferSize()const{
        int width,height;
        glfwGetFramebufferSize(m_Window,&width,&height);
        return {static_cast<uint16_t>(width),static_cast<uint16_t>(height)};
    }
    std::pair<uint16_t,uint16_t> WindowManager::GetSize()const{
        int width,height;
        glfwGetWindowSize(m_Window,&width,&height);
        return {static_cast<uint16_t>(width),static_cast<uint16_t>(height)};
    }
    std::pair<int16_t,int16_t> WindowManager::GetPosition()const{
        int x,y;
        glfwGetWindowPos(m_Window,&x,&y);
        return {static_cast<int16_t>(x),static_cast<int16_t>(y)};
    }
    WindowManager* WindowManager::FindWindowManager(GLFWwindow* window){
        return Windows.find(window)!=Windows.end()?Windows[window]:nullptr;
    }
    void WindowManager::CreateGlfwWindow(){
        GLFWmonitor* monitor=nullptr;
        if(m_IsFullscreen){
            monitor=glfwGetPrimaryMonitor();
        }

        glfwWindowHint(GLFW_RESIZABLE,1);
        glfwWindowHint(GLFW_DECORATED,1);
        glfwWindowHint(GLFW_VISIBLE,1);
        glfwWindowHint(GLFW_FOCUSED,1);
        glfwWindowHint(GLFW_AUTO_ICONIFY,1);
        glfwWindowHint(GLFW_FLOATING,0);
        glfwWindowHint(GLFW_MAXIMIZED,0);
        glfwWindowHint(GLFW_REFRESH_RATE,-1);
        glfwWindowHint(GLFW_SAMPLES,4);
        

        m_Window=glfwCreateWindow(m_Size.first,m_Size.second,m_Title.c_str(),monitor,nullptr);
        if(!m_Window){
            throw std::runtime_error("Failed to create window");
        }
        UpdateSizeLimit();
        m_Position=GetPosition();
        Windows[m_Window]=this;
    }
    void WindowManager::LoadGlad()const{
        if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
            glfwTerminate();
            throw std::runtime_error("Failed to initialize GLAD");
        }
    }
    void WindowManager::BindKeyCallback()const{
        glfwSetKeyCallback(m_Window,[](GLFWwindow* window,int key,int scancode,int action,int mods){
            if(WindowManager* manager=FindWindowManager(window)){
                if(action==GLFW_PRESS){
                    manager->KeyPressedEvent.Invoke(static_cast<Key>(key));
                }
                else if(action==GLFW_RELEASE){
                    manager->KeyReleasedEvent.Invoke(static_cast<Key>(key));
                }
            }
        });
    }
    void WindowManager::BindMouseCallback()const{
        glfwSetMouseButtonCallback(m_Window,[](GLFWwindow* window,int button,int action,int mods){
            if(WindowManager* manager=FindWindowManager(window)){
                if(action==GLFW_PRESS){
                    manager->MouseButtonPressedEvent.Invoke(static_cast<MouseButton>(button));
                }
                else if(action==GLFW_RELEASE){
                    manager->MouseButtonReleasedEvent.Invoke(static_cast<MouseButton>(button));
                }
            }
        });
    }
    void WindowManager::BindResizeCallback()const{
        glfwSetWindowSizeCallback(m_Window,[](GLFWwindow* window,int width,int height){
            if(WindowManager* manager=FindWindowManager(window)){
                manager->ResizeEvent.Invoke(static_cast<uint16_t>(width),static_cast<uint16_t>(height));
            }
        });
    }
    void WindowManager::BindFrameBufferResizeCallback()const{
        glfwSetFramebufferSizeCallback(m_Window,[](GLFWwindow* window,int width,int height){
            if(WindowManager* manager=FindWindowManager(window)){
                manager->FrameBufferResizeEvent.Invoke(static_cast<uint16_t>(width),static_cast<uint16_t>(height));
            }
        });
    }
    void WindowManager::BindMoveCallback()const{
        glfwSetWindowPosCallback(m_Window,[](GLFWwindow* window,int x,int y){
            if(WindowManager* manager=FindWindowManager(window)){
                manager->MoveEvent.Invoke(static_cast<int16_t>(x),static_cast<int16_t>(y));
            }
        });
    }
    void WindowManager::BindCursorMoveCallback()const{
        glfwSetCursorPosCallback(m_Window,[](GLFWwindow* window,double x,double y){
            if(WindowManager* manager=FindWindowManager(window)){
                manager->CursorMoveEvent.Invoke(static_cast<int16_t>(x),static_cast<int16_t>(y));
            }
        });
    }
    void WindowManager::BindIconifyCallback()const{
        glfwSetWindowIconifyCallback(m_Window,[](GLFWwindow* window,int iconified){
            if(WindowManager* manager=FindWindowManager(window)){
                if(iconified){
                    manager->MinimizeEvent.Invoke();
                }
                else{
                    manager->MaximizeEvent.Invoke();
                }
            }
        });
    }
    void WindowManager::BindFocusCallback()const{
        glfwSetWindowFocusCallback(m_Window,[](GLFWwindow* window,int focused){
            if(WindowManager* manager=FindWindowManager(window)){
                if(focused){
                    manager->GainFocusEvent.Invoke();
                }
                else{
                    manager->LostFocusEvent.Invoke();
                }
            }
        });
    }
    void WindowManager::BindCloseCallback()const{
        glfwSetWindowCloseCallback(m_Window,[](GLFWwindow* window){
            if(WindowManager* manager=FindWindowManager(window)){
                manager->CloseEvent.Invoke();
            }
        });
    }
    void WindowManager::OnResize(uint16_t width,uint16_t height){
        m_Size.first=width;
        m_Size.second=height;
    }
    void WindowManager::OnMove(int16_t x,int16_t y){
        if(!m_IsFullscreen){
            m_Position.first=x;
            m_Position.second=y;
        }
    }
    void WindowManager::OnClose(){
        ServiceLocator::Get<LogManager>().Log(LogLevel::info,"Window closed");
    }
    void WindowManager::OnFrameBufferResize(uint16_t width,uint16_t height){
        glViewport(0,0,width,height);
    }
    void WindowManager::UpdateSizeLimit()const{
        glfwSetWindowSizeLimits(m_Window,static_cast<int>(m_MinimumSize.first),static_cast<int>(m_MinimumSize.second),static_cast<int>(m_MaximumSize.first),static_cast<int>(m_MaximumSize.second));
    }
};