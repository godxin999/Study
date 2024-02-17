module context;

import stl;
import service_locator;
import window_manager;
import log_manager;
import input_manager;

namespace Engine::inline Editor{
    Context::Context(){
        m_LogManager=std::make_unique<LogManager>();
        ServiceLocator::Register<LogManager>(*m_LogManager);
        m_Device=std::make_unique<Device>();
        m_WindowManager=std::make_unique<WindowManager>(*m_Device);
        m_InputManager=std::make_unique<InputManager>(*m_WindowManager);

        m_WindowManager->MakeCurrentContext();
        m_WindowManager->LoadGlad();
        

        auto [monitorWidth,monitorHeight]=m_Device->GetMonitorSize();
        auto [windowWidth,windowHeight]=m_WindowManager->GetSize();
        m_WindowManager->SetPosition((monitorWidth-windowWidth)/2,(monitorHeight-windowHeight)/2);

        m_Device->SetVsync(true);

        ServiceLocator::Register<WindowManager>(*m_WindowManager);
        ServiceLocator::Register<InputManager>(*m_InputManager);

        m_LogManager->Log(LogLevel::info,"Context initialized");
    }
    Context::~Context(){

    }
};