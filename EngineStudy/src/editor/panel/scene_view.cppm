export module scene_view;

import view_controllable;
import panel_window;
import scene_manager;
import buffer;
import service_locator;
import stl;
import renderer;


namespace Engine::inline editor{
    export class SceneView:public ViewControllable{
    public:
        SceneView(const std::u8string& title,bool opened,const PanelWindowSettings& setting);
        ~SceneView()=default;
        void Update(float deltaTime)override;
        void RenderImpl()override;
        void RenderScene();
    private:
        SceneManager& m_sceneManager;
    };
}

module : private;

namespace Engine::inline editor{
    SceneView::SceneView(const std::u8string& title,bool opened,const PanelWindowSettings& setting):ViewControllable(title,opened,setting,true),m_sceneManager(ServiceLocator::Get<SceneManager>()){
        m_camera.SetClearColor({0.098f, 0.098f, 0.098f});
        m_camera.SetFarClip(1000.f);
    }


    void SceneView::Update(float deltaTime){
        ViewControllable::Update(deltaTime);

    }

    void SceneView::RenderImpl(){
        PrepareCamera();
        RenderScene();
    }

    void SceneView::RenderScene(){
        auto& currentScene=*m_sceneManager.GetCurrentScene();
        auto& renderer=ServiceLocator::Get<Renderer>();

        m_frameBuffer.Bind();
        renderer.SetStencilMask(0xFF);
        renderer.Clear(m_camera);
        renderer.SetStencilMask(0x00);

        m_frameBuffer.Unbind();

    }
}