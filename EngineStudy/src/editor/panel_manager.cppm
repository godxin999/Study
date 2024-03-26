export module panel_manager;

import stl;
import panel;
import panel_window;
import menu_bar;
import canvas;

namespace Engine::inline editor{
    export class PanelManager{
    public:
        PanelManager(Canvas& canvas):m_canvas(canvas){

        }
        ~PanelManager()=default;
        template <typename T,typename... Args>
        void CreatePanel(const std::u8string& name,Args&&... args){
            //如果是PanelWindow的派生类，那么需要注册到菜单栏
            if constexpr (std::derived_from<T,PanelWindow>){
                m_panels.emplace(name,std::make_unique<T>(name,std::forward<Args>(args)...));
                T& panel=GetPanel<T>(name);
                GetPanel<MenuBar>(u8"Menu Bar").RegisterPanel(name,panel);
            }
            else{
                m_panels.emplace(name,std::make_unique<T>(std::forward<Args>(args)...));
            }
            m_canvas.AddPanel(*m_panels[name]);
        }
        template <typename T>
        T& GetPanel(const std::u8string& name){
            return *dynamic_cast<T*>(m_panels[name].get());
        }
    private:
        std::unordered_map<std::u8string,std::unique_ptr<Panel>> m_panels;
        Canvas& m_canvas;
    };
}