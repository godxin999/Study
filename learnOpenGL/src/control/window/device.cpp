module;
#include <GLFW/glfw3.h>
module window_manager.device;

import input_manager.input_state;
import event;
import stl;

namespace Engine::inline Window{
    Device::Device(){
        BindErrorCallback();
        if(!glfwInit()){
            glfwTerminate();
            throw std::runtime_error("Fail to initialize GLFW");
        }
        CreateCursors();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);//主版本号
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);//次版本号
        glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);//核心模式
        m_IsInitialized=true;
    }
    Device::~Device(){
        if(m_IsInitialized){
            DestroyCursors();
            glfwTerminate();
        }
    }
    std::pair<uint16_t,uint16_t> Device::GetMonitorSize()const{
        const GLFWvidmode* mode=glfwGetVideoMode(glfwGetPrimaryMonitor());
        return {static_cast<uint16_t>(mode->width),static_cast<uint16_t>(mode->height)};
    }
    GLFWcursor* Device::GetCursorInstance(CursorShape shape)const{
        return m_Cursors.at(shape);
    }
    bool Device::HasVsync()const{
        return m_Vsync;
    }
    void Device::SetVsync(bool vsync){
        m_Vsync=vsync;
        glfwSwapInterval(m_Vsync?1:0);
    }
    void Device::PollEvents()const{
        glfwPollEvents();
    }
    float Device::GetElapsedTime()const{
        return static_cast<float>(glfwGetTime());
    }
    void Device::BindErrorCallback()const{
        glfwSetErrorCallback([](int error,const char* description){
            ErrorEvent.Invoke(static_cast<DeviceError>(error),description);
        });
    }
    void Device::CreateCursors(){
        m_Cursors.emplace(CursorShape::ARROW,glfwCreateStandardCursor(static_cast<int>(CursorShape::ARROW)));
        m_Cursors.emplace(CursorShape::IBEAM,glfwCreateStandardCursor(static_cast<int>(CursorShape::IBEAM)));
        m_Cursors.emplace(CursorShape::CROSSHAIR,glfwCreateStandardCursor(static_cast<int>(CursorShape::CROSSHAIR)));
        m_Cursors.emplace(CursorShape::HAND,glfwCreateStandardCursor(static_cast<int>(CursorShape::HAND)));
        m_Cursors.emplace(CursorShape::HRESIZE,glfwCreateStandardCursor(static_cast<int>(CursorShape::HRESIZE)));
        m_Cursors.emplace(CursorShape::VRESIZE,glfwCreateStandardCursor(static_cast<int>(CursorShape::VRESIZE)));
    }
    void Device::DestroyCursors(){
        for(auto* cursor:m_Cursors|std::views::values){
            glfwDestroyCursor(cursor);
        }
        m_Cursors.clear();
    }
}