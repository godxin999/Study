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
import service_locator;

namespace Engine::inline editor{
    Context::Context(){
        logManager=std::make_unique<LogManager>();
        ServiceLocator::Register<LogManager>(*logManager);
        device=std::make_unique<Device>();
        windowManager=std::make_unique<WindowManager>(*device);
        inputManager=std::make_unique<InputManager>(*windowManager);

        windowManager->MakeCurrentContext();
        windowManager->LoadGlad();
        

        auto [monitorWidth,monitorHeight]=device->GetMonitorSize();
        auto [windowWidth,windowHeight]=windowManager->GetSize();
        windowManager->SetPosition((monitorWidth-windowWidth)/2,(monitorHeight-windowHeight)/2);

        device->SetVsync(true);

        uiManager=std::make_unique<UIManager>(windowManager->GetGlfwWindow());
        uiManager->LoadFont("LXGWBrightGB-Regular_Big","../../../../assets/fonts/LXGWBrightGB-Regular.ttf",35.f);
        uiManager->LoadFont("LXGWBrightGB-Regular_Medium","../../../../assets/fonts/LXGWBrightGB-Regular.ttf",30.f);
        uiManager->LoadFont("LXGWBrightGB-Regular_Small","../../../../assets/fonts/LXGWBrightGB-Regular.ttf",25.f);
        uiManager->UseFont("LXGWBrightGB-Regular_Medium");
        uiManager->SetEditorLayoutAutoSaveInterval(60.f);
        uiManager->EnableEditorLayoutSave(true);
        uiManager->EnableDocking(true);

        ServiceLocator::Register<WindowManager>(*windowManager);
        ServiceLocator::Register<InputManager>(*inputManager);


        ubo=std::make_unique<UniformBuffer>(
            sizeof(glm::mat4)*2+sizeof(glm::vec4),0,0,AccessSpecifier::STREAM_DRAW
        );
        lightSSBO=std::make_unique<ShaderStorageBuffer>(AccessSpecifier::STREAM_DRAW);

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
        spotLight.attCoeff=glm::vec3(1.f,0.09f,0.032f);
        spotLight.cutoff=15.f;
        spotLight.outerCutoff=22.5f;
        lightData.push_back(spotLight.GenerateDataMatrix());

        pointLightTransform.SetWorldPosition(glm::vec3(5.f,0.f,-5.0f));
        Light pointLight(pointLightTransform,LightType::Point);
        lightData.push_back(pointLight.GenerateDataMatrix());

        ambientSphereLightTransform.SetWorldPosition(glm::vec3(0.f,0.f,0.f));
        Light ambientSphereLight(ambientSphereLightTransform,LightType::AmbientSphere);
        ambientSphereLight.attCoeff=glm::vec3(1000.f);
        ambientSphereLight.color=glm::vec3(0.5f);
        lightData.push_back(ambientSphereLight.GenerateDataMatrix());

        lightSSBO->SendBlocks(lightData.data(),lightData.size()*sizeof(glm::mat4));
        
        ServiceLocator::Register<Context>(*this);
        logManager->Log(LogLevel::info,"Context initialized");
    }
    
    Context::~Context(){

    }
};