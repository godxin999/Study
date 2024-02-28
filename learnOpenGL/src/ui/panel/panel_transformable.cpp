module;
#include <imgui.h>
module panel_transformable;

import glm;
import alignment;
import panel;
import converter;

namespace Engine::inline UI{
    PanelTransformable::PanelTransformable(
        const glm::vec2& defaultPosition, 
        const glm::vec2& defaultSize, 
        const HorizontalAlignment& defaultHorizontalAlignment, 
        const VerticalAlignment& defaultVerticalAlignment,
        bool IsIgnoreConfigFile):
        m_DefaultPosition(defaultPosition),
        m_DefaultSize(defaultSize),
        m_DefaultHorizontalAlignment(defaultHorizontalAlignment),
        m_DefaultVerticalAlignment(defaultVerticalAlignment),
        m_IsIgnoreConfigFile(IsIgnoreConfigFile){
            
    }
    void PanelTransformable::SetPostion(const glm::vec2& position){
        m_Position=position;
        m_IsPositionChanged=true;
    }
    void PanelTransformable::SetSize(const glm::vec2& size){
        m_Size=size;
        m_IsSizeChanged=true;
    }
    void PanelTransformable::SetAlignment(HorizontalAlignment horizontalAlignment, VerticalAlignment verticalAlignment){
        m_HorizontalAlignment=horizontalAlignment;
        m_VerticalAlignment=verticalAlignment;
        m_IsAlignmentChanged=true;
    }
    const glm::vec2& PanelTransformable::GetPosition() const{
        return m_Position;
    }
    const glm::vec2& PanelTransformable::GetSize() const{
        return m_Size;
    }
    HorizontalAlignment PanelTransformable::GetHorizontalAlignment() const{
        return m_HorizontalAlignment;
    }
    VerticalAlignment PanelTransformable::GetVerticalAlignment() const{
        return m_VerticalAlignment;
    }
    void PanelTransformable::Update(){
        if(m_FirstFrame){
            m_FirstFrame=false;
        }
        else[[likely]]{
            UpdateSize();
            UpdatePosition();
        }
    }
    glm::vec2 PanelTransformable::CalculatePositionAlignmentOffset(bool defaultValue){
        glm::vec2 result(0.f);
        switch(defaultValue?m_DefaultHorizontalAlignment:m_HorizontalAlignment){
            case HorizontalAlignment::CENTER:
                result.x-=m_Size.x/2.f;
                break;
            case HorizontalAlignment::RIGHT:
                result.x-=m_Size.x;
                break;
            default:
                break;
        }
        switch(defaultValue?m_DefaultVerticalAlignment:m_VerticalAlignment){
            case VerticalAlignment::CENTER:
                result.y-=m_Size.y/2.f;
                break;
            case VerticalAlignment::BOTTOM:
                result.y-=m_Size.y;
                break;
            default:
                break;
        }
        return result;
    }
    void PanelTransformable::UpdatePosition(){
        if(m_DefaultPosition.x!=-1.f&&m_DefaultPosition.y!=-1.f){
            glm::vec2 offset=CalculatePositionAlignmentOffset(true)+m_DefaultPosition;
            ImGui::SetWindowPos(Converter::ToImVec2(offset),m_IsIgnoreConfigFile?ImGuiCond_Once:ImGuiCond_FirstUseEver);
        }
        if(m_IsPositionChanged||m_IsAlignmentChanged){
            glm::vec2 offset=CalculatePositionAlignmentOffset()+m_Position;
            ImGui::SetWindowPos(Converter::ToImVec2(offset), ImGuiCond_Always);
            m_IsPositionChanged=false;
            m_IsAlignmentChanged=false;
        }
        m_Position=Converter::ToVec2(ImGui::GetWindowPos());
    }
    void PanelTransformable::UpdateSize(){
        //如果不是自适应大小
        if(!autoSize&&m_IsSizeChanged){
            ImGui::SetWindowSize(Converter::ToImVec2(m_Size), ImGuiCond_Always);
            m_IsSizeChanged=false;
        }
        m_Size=Converter::ToVec2(ImGui::GetWindowSize());
    }
}