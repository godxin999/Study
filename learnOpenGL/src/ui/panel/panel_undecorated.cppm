module;
#include <imgui.h>
export module panel_undecorated;

import panel_transformable;
import stl;

namespace Engine::inline UI{
    export class PanelUndecorated : public PanelTransformable{
    public:
        void DrawImpl()override;
    private:
        int CollectFlags();
    };
}

module : private;

namespace Engine::inline UI{
    void PanelUndecorated::DrawImpl(){
        auto& style=ImGui::GetStyle();
        auto previousPadding=style.WindowPadding;
        auto previousMinSize=style.WindowMinSize;
        style.WindowPadding={0,0};
        style.WindowMinSize={0,0};
        if(ImGui::Begin(panelID.c_str(),nullptr,CollectFlags())){
            style.WindowPadding=previousPadding;
            style.WindowMinSize=previousMinSize;
            Update();
            DrawWidgets();
            ImGui::End();
        }
        else{
            style.WindowPadding=previousPadding;
        }
    }

    int PanelUndecorated::CollectFlags(){
        int flags = ImGuiWindowFlags_AlwaysUseWindowPadding|
                    ImGuiWindowFlags_NoResize|
                    ImGuiWindowFlags_NoMove|
                    ImGuiWindowFlags_NoScrollbar|
                    ImGuiWindowFlags_NoScrollWithMouse|
                    ImGuiWindowFlags_NoCollapse|
                    ImGuiWindowFlags_NoTitleBar|
                    ImGuiWindowFlags_NoBackground;
        if(autoSize){
            flags |= ImGuiWindowFlags_AlwaysAutoResize;
        }
        return flags;
    }
}