module widget;

import drawable;
import stl;
import pluginable;
import imgui;

namespace Engine::inline UI{
    Widget::Widget(){
        m_WidgetID=WidgetCounter++;
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
}
