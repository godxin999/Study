export module view_controllable;

import view;
import camera_controller;
import panel_window;
import stl;

namespace Engine::inline editor{
    export class ViewControllable : public View{
    public:
        ViewControllable(const std::u8string& title,bool opened,const PanelWindowSettings& setting,bool enableFocusInput=false);
        ~ViewControllable()=default;
        void Update(float deltaTime)override;
        CameraController& GetCameraController();
    protected:
        CameraController m_cameraController;
    };
}

module : private;

namespace Engine::inline editor{
    ViewControllable::ViewControllable(const std::u8string& title,bool opened,const PanelWindowSettings& setting,bool enableFocusInput):View(title,opened,setting),m_cameraController(m_camera,enableFocusInput){

    }

    void ViewControllable::Update(float deltaTime){
        if(IsHovered()){
            m_cameraController.HandleInput(deltaTime);
        }
        View::Update(deltaTime);
    }

    CameraController& ViewControllable::GetCameraController(){
        return m_cameraController;
    }
}