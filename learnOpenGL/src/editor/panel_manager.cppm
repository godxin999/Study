export module panel_manager;

import stl;
import panel;
import panel_window;
import menu_bar;
import canvas;

namespace Engine::inline editor{
    export class PanelManager{
    public:
        PanelManager(Canvas& canvas):m_Canvas(canvas){

        }
        ~PanelManager()=default;
        template <typename T,typename... Args>
        void CreatePanel(const std::string& name,Args&&... args){
            //如果是PanelWindow的派生类，那么需要注册到菜单栏
            if constexpr (std::derived_from<T,PanelWindow>){
                m_Panels.emplace(name,std::make_unique<T>(std::forward<Args>(args)...));
                T& panel=GetPanel<T>(name);
                GetPanel<MenuBar>("Menu Bar").RegisterPanel(name,panel);
            }
            else{
                m_Panels.emplace(name,std::make_unique<T>(std::forward<Args>(args)...));
            }
            m_Canvas.AddPanel(*m_Panels[name]);
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