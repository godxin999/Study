export module panel_menu_bar;

import panel;
import widget_container;
import imgui;

namespace Engine::inline UI{
    export class PanelMenuBar:public Panel{
    protected:
        void DrawImpl()override;
    };
}

module : private;

namespace Engine::inline UI{
    void PanelMenuBar::DrawImpl(){
        if(!m_Widgets.empty()&&ImGui::BeginMainMenuBar()){
            DrawWidgets();
            ImGui::EndMainMenuBar();
        }
    }
}