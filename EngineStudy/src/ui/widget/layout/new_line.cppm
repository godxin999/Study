export module new_line;

import imgui;
import widget;

namespace Engine::inline UI{
    export class NewLine:public Widget{
    public:
        NewLine()=default;
        ~NewLine()=default;
    protected:
        void DrawImpl()override;
    };
}

module : private;

namespace Engine::inline UI{
    void NewLine::DrawImpl(){
        ImGui::NewLine();
    }
}