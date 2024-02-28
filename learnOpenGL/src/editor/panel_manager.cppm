export module panel_manager;

import stl;
import panel;
import canvas;

namespace Engine::inline editor{
    export class PanelManager{
    public:
        PanelManager(Canvas& canvas):m_Canvas(canvas){

        }
        ~PanelManager()=default;
        template <typename T,typename... Args>
        void CreatePanel(const std::string& name,Args&&... args){

        }
        template <typename T>
        T& GetPanel(const std::string& name){
            return *dynamic_cast<T*>(m_Panels[name].get());
        }
    private:
        std::unordered_map<std::string,std::unique_ptr<Panel>> m_Panels;
        Canvas& m_Canvas;
    };
}