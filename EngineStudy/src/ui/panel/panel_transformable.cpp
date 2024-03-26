module panel_transformable;

import glm;
import alignment;
import panel;
import converter;
import imgui;

namespace Engine::inline UI{
    PanelTransformable::PanelTransformable(
        const glm::vec2& p_defaultPosition, 
        const glm::vec2& p_defaultSize, 
        HorizontalAlignment p_defaultHorizontalAlignment, 
        VerticalAlignment p_defaultVerticalAlignment,
        bool p_isIgnoreConfigFile):
        defaultPosition(p_defaultPosition),
        defaultSize(p_defaultSize),
        defaultHorizontalAlignment(p_defaultHorizontalAlignment),
        defaultVerticalAlignment(p_defaultVerticalAlignment),
        isIgnoreConfigFile(p_isIgnoreConfigFile){
            
    }

    void PanelTransformable::SetPostion(const glm::vec2& p_position){
        position=p_position;
        isPositionChanged=true;
    }

    void PanelTransformable::SetSize(const glm::vec2& p_size){
        size=p_size;
        isSizeChanged=true;
    }

    void PanelTransformable::SetAlignment(HorizontalAlignment p_horizontalAlignment, VerticalAlignment p_verticalAlignment){
        horizontalAlignment=p_horizontalAlignment;
        verticalAlignment=p_verticalAlignment;
        isAlignmentChanged=true;
    }

    const glm::vec2& PanelTransformable::GetPosition() const{
        return position;
    }

    const glm::vec2& PanelTransformable::GetSize() const{
        return size;
    }

    HorizontalAlignment PanelTransformable::GetHorizontalAlignment() const{
        return horizontalAlignment;
    }

    VerticalAlignment PanelTransformable::GetVerticalAlignment() const{
        return verticalAlignment;
    }

    void PanelTransformable::Update(){
        if(firstFrame){
            firstFrame=false;
        }
        else[[likely]]{
            UpdateSize();
            UpdatePosition();
        }
    }

    glm::vec2 PanelTransformable::CalculatePositionAlignmentOffset(bool defaultValue){
        glm::vec2 result(0.f);
        switch(defaultValue?defaultHorizontalAlignment:horizontalAlignment){
            case HorizontalAlignment::CENTER:
                result.x-=size.x/2.f;
                break;
            case HorizontalAlignment::RIGHT:
                result.x-=size.x;
                break;
            default:
                break;
        }
        switch(defaultValue?defaultVerticalAlignment:verticalAlignment){
            case VerticalAlignment::CENTER:
                result.y-=size.y/2.f;
                break;
            case VerticalAlignment::BOTTOM:
                result.y-=size.y;
                break;
            default:
                break;
        }
        return result;
    }
    
    void PanelTransformable::UpdatePosition(){
        if(defaultPosition.x!=-1.f&&defaultPosition.y!=-1.f){
            glm::vec2 offset=CalculatePositionAlignmentOffset(true)+defaultPosition;
            ImGui::SetWindowPos(Converter::ToImVec2(offset),isIgnoreConfigFile?ImGuiCond_Once:ImGuiCond_FirstUseEver);
        }
        if(isPositionChanged||isAlignmentChanged){
            glm::vec2 offset=CalculatePositionAlignmentOffset()+position;
            ImGui::SetWindowPos(Converter::ToImVec2(offset), ImGuiCond_Always);
            isPositionChanged=false;
            isAlignmentChanged=false;
        }
        position=Converter::ToVec2(ImGui::GetWindowPos());
    }
    
    void PanelTransformable::UpdateSize(){
        //如果不是自适应大小
        if(!autoSize&&isSizeChanged){
            ImGui::SetWindowSize(Converter::ToImVec2(size), ImGuiCond_Always);
            isSizeChanged=false;
        }
        size=Converter::ToVec2(ImGui::GetWindowSize());
    }
}