export module image;

import glm;
import widget;
import imgui;
import converter;

namespace Engine::inline UI{
    export class Image:public Widget{
    public:
        Image(uint32_t p_textureID,const glm::vec2& size);
        ~Image()=default;
    protected:
        void DrawImpl()override;
    public:
        uint32_t textureID;
        glm::vec2 size;
    };
}

module : private;

namespace Engine::inline UI{
    Image::Image(uint32_t p_textureID,const glm::vec2& p_size):textureID(p_textureID),size(p_size){
        
    }

    void Image::DrawImpl(){
        //因为OpenGL的左下角是(0,0)，而ImGui的左上角是(0,0)，所以要把y轴翻转
        ImGui::Image(reinterpret_cast<ImTextureID>(textureID),Converter::ToImVec2(size),ImVec2(0,1),ImVec2(1,0));
    }
}
