export module spacing;

import widget;
import imgui;
import stl;

namespace Engine::inline UI{
    //Spacing是固定大小的空白控件
    export class Spacing:public Widget{
    public:
        Spacing()=default;
        Spacing(uint16_t spaces);
        ~Spacing()=default;
    protected:
        void DrawImpl()override;
    public:
        uint16_t spaces{1};
    };
}

module : private;

namespace Engine::inline UI{
    Spacing::Spacing(uint16_t spaces):spaces(spaces){

    }
    void Spacing::DrawImpl(){
        for(auto i=0;i<spaces;++i){
            ImGui::Spacing();
            if(i+1<spaces){
                ImGui::SameLine();
            }
        }
    }
}