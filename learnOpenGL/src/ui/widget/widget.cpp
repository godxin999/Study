module widget;

import drawable;
import stl;
import pluginable;
import imgui;

namespace Engine::inline UI{
    Widget::Widget(){
        m_WidgetID=m_WidgetCounter++;
    }
    void Widget::Draw(){
        if(m_IsEnabled){
            DrawImpl();

            if(m_IsAutoExecutePlugins){
                ExecutePlugins();
            }

            if(m_IsLineBreak){
                ImGui::SameLine();
            }
        }
    }
    WidgetContainer* Widget::GetParent() const{
        return m_Parent;
    }
    void Widget::SetParent(WidgetContainer* parent){
        m_Parent=parent;
    }
}
