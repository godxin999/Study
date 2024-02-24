module context;

import stl;
import service_locator;
import window_manager;
import log_manager;
import input_manager;
import transform;
import light;
import glm;
import buffer;

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


        m_UBO=std::make_unique<UniformBuffer>(
            sizeof(glm::mat4)*2+sizeof(glm::vec4),0,0,AccessSpecifier::STREAM_DRAW
        );
        m_LightSSBO=std::make_unique<ShaderStorageBuffer>(AccessSpecifier::STREAM_DRAW);

        Transform lightTransform;
        lightTransform.SetLocalRotation(glm::quat(glm::vec3(glm::radians(45.f),glm::radians(180.f),glm::radians(10.f))));
        Light directionalLight(lightTransform,LightType::Directional);
        std::vector<glm::mat4> lightData;
        lightData.push_back(directionalLight.GenerateDataMatrix());
        m_LightSSBO->SendBlocks(lightData.data(),lightData.size()*sizeof(glm::mat4));
        
        
        ServiceLocator::Register<Context>(*this);
        m_LogManager->Log(LogLevel::info,"Context initialized");
    }
    Context::~Context(){

    }
};