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
import gl_enum;

namespace Engine::inline editor{
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

        m_UIManager=std::make_unique<UIManager>(m_WindowManager->GetGlfwWindow());
        m_UIManager->SetEditorLayoutAutoSaveInterval(60.f);
        m_UIManager->EnableEditorLayoutSave(true);
        m_UIManager->EnableDocking(true);

        ServiceLocator::Register<WindowManager>(*m_WindowManager);
        ServiceLocator::Register<InputManager>(*m_InputManager);


        m_UBO=std::make_unique<UniformBuffer>(
            sizeof(glm::mat4)*2+sizeof(glm::vec4),0,0,AccessSpecifier::STREAM_DRAW
        );
        m_LightSSBO=std::make_unique<ShaderStorageBuffer>(AccessSpecifier::STREAM_DRAW);

        Transform lightTransform;
        Transform spotLightTransform;
        Transform pointLightTransform;
        Transform ambientSphereLightTransform;
        std::vector<glm::mat4> lightData;

        lightTransform.SetLocalRotation(glm::quat(glm::vec3(glm::radians(45.f),glm::radians(180.f),glm::radians(10.f))));
        Light directionalLight(lightTransform,LightType::Directional);
        lightData.push_back(directionalLight.GenerateDataMatrix());

        spotLightTransform.SetWorldRotation(glm::angleAxis(glm::radians(180.f),glm::vec3(0.f,1.f,0.f)));
        spotLightTransform.SetWorldPosition(glm::vec3(0.f,0.f,2.0f));
        Light spotLight(spotLightTransform,LightType::Spot);
        spotLight.m_AttCoeff=glm::vec3(1.f,0.09f,0.032f);
        spotLight.m_Cutoff=15.f;
        spotLight.m_OuterCutoff=22.5f;
        lightData.push_back(spotLight.GenerateDataMatrix());

        pointLightTransform.SetWorldPosition(glm::vec3(5.f,0.f,-5.0f));
        Light pointLight(pointLightTransform,LightType::Point);
        lightData.push_back(pointLight.GenerateDataMatrix());

        ambientSphereLightTransform.SetWorldPosition(glm::vec3(0.f,0.f,0.f));
        Light ambientSphereLight(ambientSphereLightTransform,LightType::AmbientSphere);
        ambientSphereLight.m_AttCoeff=glm::vec3(1000.f);
        ambientSphereLight.m_Color=glm::vec3(0.5f);
        lightData.push_back(ambientSphereLight.GenerateDataMatrix());

        m_LightSSBO->SendBlocks(lightData.data(),lightData.size()*sizeof(glm::mat4));
        
        ServiceLocator::Register<Context>(*this);
        m_LogManager->Log(LogLevel::info,"Context initialized");
    }
    Context::~Context(){

    }
};