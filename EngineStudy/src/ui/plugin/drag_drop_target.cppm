export module drag_drop_target;

import plugin;
import stl;
import event;
import imgui;

namespace Engine::inline UI{
    export template <typename T>
    class DragDropTarget:public Plugin{
    public:
        DragDropTarget(const std::u8string& identifier);
        void Execute() override;
        [nodiscard]bool isHovered()const;
    public:
        std::u8string identifier;
        Event<T> DataReceivedEvent;
        Event<> HoverStartEvent;
        Event<> HoverEndEvent;
        bool showYellowRect{true};
    private:
        bool m_isHovered;
    };
}

module : private;

namespace Engine::inline UI{
    template <typename T>
    DragDropTarget<T>::DragDropTarget(const std::u8string& identifier):identifier(identifier){
        
    }

    template <typename T>
    void DragDropTarget<T>::Execute(){
        if(ImGui::BeginDragDropTarget()){
            if(!m_isHovered){
                HoverStartEvent();
            }
            m_isHovered = true;
            ImGuiDragDropFlags flags = 0;
            if(showYellowRect){
                flags|=ImGuiDragDropFlags_AcceptNoDrawDefaultRect;//拖拽时不显示默认黄色矩形
            }
            if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(reinterpret_cast<const char*>(identifier.c_str()),flags)){
                DataReceivedEvent(*reinterpret_cast<T*>(payload->Data));
            }
            ImGui::EndDragDropTarget();
        }
        else{
            if(m_isHovered){
                HoverEndEvent();
            }
            m_isHovered = false;
        }
    }

    template <typename T>
    bool DragDropTarget<T>::isHovered()const{
        return m_isHovered;
    }
}