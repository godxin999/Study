module context;

import stl;
import window_manager;
import log_manager;

namespace Engine::inline Editor{
    Context::Context(){
        m_LogManager=std::make_unique<LogManager>();
        m_Device=std::make_unique<Device>();
        m_WindowManager=std::make_unique<WindowManager>(*m_Device);

        m_WindowManager->MakeCurrentContext();
        auto [monitorWidth,monitorHeight]=m_Device->GetMonitorSize();
        auto [windowWidth,windowHeight]=m_WindowManager->GetSize();
        m_WindowManager->SetPosition((monitorWidth-windowWidth)/2,(monitorHeight-windowHeight)/2);

        m_Device->SetVsync(true);

    }
    Context::~Context(){

    }
};