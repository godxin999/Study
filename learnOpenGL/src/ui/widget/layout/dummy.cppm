export module dummy;

import converter;
import glm;
import widget;
import imgui;

namespace Engine::inline UI{
    //Dummy是可指定大小的空白控件
    export class Dummy:public Widget{
    public:
        Dummy()=default;
        Dummy(const glm::vec2& size);
        ~Dummy()=default;
    protected:
        void DrawImpl()override;
    public:
        glm::vec2 size;
    };
}

module : private;

namespace Engine::inline UI{
    Dummy::Dummy(const glm::vec2& size):size(size){

    }
    
    void Dummy::DrawImpl(){
        ImGui::Dummy(Converter::ToImVec2(size));
    }
}