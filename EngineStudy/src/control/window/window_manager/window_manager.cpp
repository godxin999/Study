module;
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
module window_manager;

import input_manager.input_state;
import event;
import stl;
import log_manager;
import service_locator;
import device;

namespace Engine::inline Window{
    WindowManager::WindowManager(const Device& device):m_device(device){
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

        ResizeEvent+=[this](uint16_t width,uint16_t height){
            m_size.first=width;
            m_size.second=height;
            ServiceLocator::Get<LogManager>().Log(LogLevel::info,std::format("Window resized to {}x{}",width,height));
        };
        FrameBufferResizeEvent+=[this](uint16_t width,uint16_t height){
            glViewport(0,0,width,height);
            ServiceLocator::Get<LogManager>().Log(LogLevel::info,std::format("Framebuffer resized to {}x{}",width,height));
        };
        MoveEvent+=[this](int16_t x,int16_t y){
            if(!m_isFullscreen){
                m_position.first=x;
                m_position.second=y;
            }
            ServiceLocator::Get<LogManager>().Log(LogLevel::info,std::format("Window moved to ({},{})",x,y));
        };
        CloseEvent+=[this](){
            ServiceLocator::Get<LogManager>().Log(LogLevel::info,"Window closed");
        };
    }
    
    WindowManager::~WindowManager(){
        Windows.erase(m_window);
        glfwDestroyWindow(m_window);
    }
    
    void WindowManager::SetIcon(const std::u8string& path){}
    
    void WindowManager::SetIconFromMemory(uint8_t* data,uint32_t width,uint32_t height){}
    
    void WindowManager::SetTitle(const std::u8string& title){
        m_title=title;
        glfwSetWindowTitle(m_window,reinterpret_cast<const char*>(m_title.c_str()));
    }
    
    void WindowManager::SetSize(uint16_t width,uint16_t height){
        glfwSetWindowSize(m_window,static_cast<int>(width),static_cast<int>(height));
    }
    
    void WindowManager::SetPosition(int16_t x,int16_t y){
        glfwSetWindowPos(m_window,static_cast<int>(x),static_cast<int>(y));
    }
    
    void WindowManager::SetMinimumSize(int16_t width,int16_t height){
        m_minimumSize.first=width;
        m_minimumSize.second=height;
        UpdateSizeLimit();
    }
    
    void WindowManager::SetMaximumSize(int16_t width,int16_t height){
        m_maximumSize.first=width;
        m_maximumSize.second=height;
        UpdateSizeLimit();
    }
    
    void WindowManager::SetFullscreen(bool fullscreen){
        m_isFullscreen=fullscreen;
        glfwSetWindowMonitor(m_window,m_isFullscreen?glfwGetPrimaryMonitor():nullptr,static_cast<int>(m_position.first),static_cast<int>(m_position.second),static_cast<int>(m_size.first),static_cast<int>(m_size.second),m_refreshRate);   
    }
    
    void WindowManager::SetCursorMode(CursorMode mode){
        m_cursorMode=mode;
        glfwSetInputMode(m_window,GLFW_CURSOR,static_cast<int>(m_cursorMode));
    }
    
    void WindowManager::SetCursorShape(CursorShape shape){
        m_cursorShape=shape;
        glfwSetCursor(m_window,m_device.GetCursorInstance(m_cursorShape));
    }
    
    void WindowManager::SetShouldClose(bool shouldClose)const{
        glfwSetWindowShouldClose(m_window,shouldClose);
    }
    
    void WindowManager::Minimize()const{
        glfwIconifyWindow(m_window);
    }
    
    void WindowManager::Maximize()const{
        glfwMaximizeWindow(m_window);
    }
    
    void WindowManager::Restore()const{
        glfwRestoreWindow(m_window);
    }
    
    void WindowManager::Focus()const{
        glfwFocusWindow(m_window);
    }
    
    void WindowManager::Hide()const{
        glfwHideWindow(m_window);
    }
    
    void WindowManager::Show()const{
        glfwShowWindow(m_window);
    }
    
    void WindowManager::ToggleFullscreen(){
        SetFullscreen(!m_isFullscreen);
    }
    
    bool WindowManager::IsShouldClose()const{
        return glfwWindowShouldClose(m_window);
    }
    
    bool WindowManager::IsMinimized()const{
        return glfwGetWindowAttrib(m_window,GLFW_ICONIFIED);
    }
    
    bool WindowManager::IsMaximized()const{
        return glfwGetWindowAttrib(m_window,GLFW_MAXIMIZED);
    }
    
    bool WindowManager::IsFocused()const{
        return glfwGetWindowAttrib(m_window,GLFW_FOCUSED);
    }
    
    bool WindowManager::IsVisible()const{
        return glfwGetWindowAttrib(m_window,GLFW_VISIBLE);
    }
    
    bool WindowManager::IsResizable()const{
        return glfwGetWindowAttrib(m_window,GLFW_RESIZABLE);
    }
    
    bool WindowManager::IsDecorated()const{
        return glfwGetWindowAttrib(m_window,GLFW_DECORATED);
    }
    
    void WindowManager::MakeCurrentContext()const{
        glfwMakeContextCurrent(m_window);
    }
    
    void WindowManager::SwapBuffers()const{
        glfwSwapBuffers(m_window);
    }
    
    std::pair<uint16_t,uint16_t> WindowManager::GetFramebufferSize()const{
        int width,height;
        glfwGetFramebufferSize(m_window,&width,&height);
        return {static_cast<uint16_t>(width),static_cast<uint16_t>(height)};
    }
    
    std::pair<uint16_t,uint16_t> WindowManager::GetSize()const{
        int width,height;
        glfwGetWindowSize(m_window,&width,&height);
        return {static_cast<uint16_t>(width),static_cast<uint16_t>(height)};
    }
    
    std::pair<int16_t,int16_t> WindowManager::GetPosition()const{
        int x,y;
        glfwGetWindowPos(m_window,&x,&y);
        return {static_cast<int16_t>(x),static_cast<int16_t>(y)};
    }
    
    WindowManager* WindowManager::FindWindowManager(GLFWwindow* window){
        return Windows.find(window)!=Windows.end()?Windows[window]:nullptr;
    }
    
    void WindowManager::CreateGlfwWindow(){
        GLFWmonitor* monitor=nullptr;
        if(m_isFullscreen){
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
        

        m_window=glfwCreateWindow(m_size.first,m_size.second,reinterpret_cast<const char*>(m_title.c_str()),monitor,nullptr);
        if(!m_window){
            throw std::runtime_error("Failed to create window");
        }
        UpdateSizeLimit();
        m_position=GetPosition();
        Windows[m_window]=this;
    }
    
    void WindowManager::LoadGlad()const{
        if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
            glfwTerminate();
            throw std::runtime_error("Failed to initialize GLAD");
        }
    }
    
    void WindowManager::BindKeyCallback()const{
        glfwSetKeyCallback(m_window,[](GLFWwindow* window,int key,int scancode,int action,int mods){
            if(WindowManager* manager=FindWindowManager(window)){
                if(action==GLFW_PRESS){
                    manager->KeyPressedEvent(static_cast<Key>(key));
                }
                else if(action==GLFW_RELEASE){
                    manager->KeyReleasedEvent(static_cast<Key>(key));
                }
            }
        });
    }
    
    void WindowManager::BindMouseCallback()const{
        glfwSetMouseButtonCallback(m_window,[](GLFWwindow* window,int button,int action,int mods){
            if(WindowManager* manager=FindWindowManager(window)){
                if(action==GLFW_PRESS){
                    manager->MouseButtonPressedEvent(static_cast<MouseButton>(button));
                }
                else if(action==GLFW_RELEASE){
                    manager->MouseButtonReleasedEvent(static_cast<MouseButton>(button));
                }
            }
        });
    }
    
    void WindowManager::BindResizeCallback()const{
        glfwSetWindowSizeCallback(m_window,[](GLFWwindow* window,int width,int height){
            if(WindowManager* manager=FindWindowManager(window)){
                manager->ResizeEvent(static_cast<uint16_t>(width),static_cast<uint16_t>(height));
            }
        });
    }
    
    void WindowManager::BindFrameBufferResizeCallback()const{
        glfwSetFramebufferSizeCallback(m_window,[](GLFWwindow* window,int width,int height){
            if(WindowManager* manager=FindWindowManager(window)){
                manager->FrameBufferResizeEvent(static_cast<uint16_t>(width),static_cast<uint16_t>(height));
            }
        });
    }
    
    void WindowManager::BindMoveCallback()const{
        glfwSetWindowPosCallback(m_window,[](GLFWwindow* window,int x,int y){
            if(WindowManager* manager=FindWindowManager(window)){
                manager->MoveEvent(static_cast<int16_t>(x),static_cast<int16_t>(y));
            }
        });
    }
    
    void WindowManager::BindCursorMoveCallback()const{
        glfwSetCursorPosCallback(m_window,[](GLFWwindow* window,double x,double y){
            if(WindowManager* manager=FindWindowManager(window)){
                manager->CursorMoveEvent(static_cast<int16_t>(x),static_cast<int16_t>(y));
            }
        });
    }
    
    void WindowManager::BindIconifyCallback()const{
        glfwSetWindowIconifyCallback(m_window,[](GLFWwindow* window,int iconified){
            if(WindowManager* manager=FindWindowManager(window)){
                if(iconified){
                    manager->MinimizeEvent();
                }
                else{
                    manager->MaximizeEvent();
                }
            }
        });
    }
    
    void WindowManager::BindFocusCallback()const{
        glfwSetWindowFocusCallback(m_window,[](GLFWwindow* window,int focused){
            if(WindowManager* manager=FindWindowManager(window)){
                if(focused){
                    manager->GainFocusEvent();
                }
                else{
                    manager->LostFocusEvent();
                }
            }
        });
    }
    
    void WindowManager::BindCloseCallback()const{
        glfwSetWindowCloseCallback(m_window,[](GLFWwindow* window){
            if(WindowManager* manager=FindWindowManager(window)){
                manager->CloseEvent();
            }
        });
    }
    
    void WindowManager::UpdateSizeLimit()const{
        glfwSetWindowSizeLimits(m_window,static_cast<int>(m_minimumSize.first),static_cast<int>(m_minimumSize.second),static_cast<int>(m_maximumSize.first),static_cast<int>(m_maximumSize.second));
    }
};