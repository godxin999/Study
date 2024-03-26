export module drag_drop_source;

import plugin;
import stl;
import event;
import imgui;

namespace Engine::inline UI{
    export template <typename T>
    class DragDropSource:public Plugin{
    public:
        DragDropSource(const std::u8string& identifier,const std::u8string& tooltip,T data);
        void Execute() override;
        [nodiscard]bool isDragged()const;
    public:
        std::u8string identifier;
        std::u8string tooltip;
        T data;
        Event<> DragStartEvent;
        Event<> DragEndEvent;
        bool hasTooltip{true};
    private:
        bool m_isDragged;
    };
}

module : private;

namespace Engine::inline UI{
    template <typename T>
    DragDropSource<T>::DragDropSource(const std::u8string& identifier,const std::u8string& tooltip,T data):identifier(identifier),tooltip(tooltip),data(data){

    }

    template <typename T>
    void DragDropSource<T>::Execute(){
        ImGuiDragDropFlags flags = 0;
        flags|=ImGuiDragDropFlags_SourceNoDisableHover;//当拖拽源被拖拽时依然显示悬停效果
        flags|=ImGuiDragDropFlags_SourceNoHoldToOpenOthers;//当拖拽源被拖拽时不允许打开其他树节点
        if(!hasTooltip){
            flags|=ImGuiDragDropFlags_SourceNoPreviewTooltip;//当拖拽源被拖拽时不显示预览提示
        }
        if(ImGui::BeginDragDropSource(flags)){
            if(!m_isDragged){
                DragStartEvent();
            }   
            m_isDragged = true;
            if(!(flags&ImGuiDragDropFlags_SourceNoPreviewTooltip)){
                ImGui::Text(reinterpret_cast<const char*>(tooltip.c_str()));
            }
            ImGui::SetDragDropPayload(reinterpret_cast<const char*>(identifier.c_str()),&data,sizeof(T));
            ImGui::EndDragDropSource();
        }
        else{
            if(m_isDragged){
                DragEndEvent();
            }
            m_isDragged = false;
        }
    }

    template <typename T>
    bool DragDropSource<T>::isDragged()const{
        return m_isDragged;
    }
}