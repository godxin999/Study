export module separator;

import widget;
import imgui;


namespace Engine::inline UI{
    export class Separator:public Widget{
    public:
        Separator()=default;
        ~Separator()=default;
    protected:
        void DrawImpl() override;
    };
}

module : private;

namespace Engine::inline UI{
    void Separator::DrawImpl(){
        ImGui::Separator();
    }
}