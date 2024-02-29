module widget_container;
import stl;
import widget;

namespace Engine::inline UI{
    WidgetContainer::~WidgetContainer(){
        RemoveAllWidgets();
    }
    void WidgetContainer::RemoveWidget(Widget& widget){
        auto it=std::ranges::find_if(m_Widgets,[&widget](const auto& pair){
            return pair.first==&widget;
        });
        if(it!=m_Widgets.end()){
            if(it->second==MemoryMode::INTERNAL_MANAGEMENT){
                delete it->first;
            }
            m_Widgets.erase(it);
        }
    }
    void WidgetContainer::RemoveAllWidgets(){
        std::ranges::for_each(m_Widgets,[](const auto& pair){
            if(pair.second==MemoryMode::INTERNAL_MANAGEMENT){
                delete pair.first;
            }
        });
        m_Widgets.clear();
    }
    void WidgetContainer::ConsiderWidget(Widget& widget, bool manageMemory){
        m_Widgets.emplace_back(&widget, manageMemory ? MemoryMode::INTERNAL_MANAGEMENT : MemoryMode::EXTERNAL_MANAGEMENT);
        widget.SetParent(this);
    }
    void WidgetContainer::UnconsiderWidget(Widget& widget){
        auto it=std::ranges::find_if(m_Widgets,[&widget](const auto& pair){
            return pair.first==&widget;
        });
        if(it!=m_Widgets.end()){
            widget.SetParent(nullptr);
            m_Widgets.erase(it);
        }
    }
    void WidgetContainer::CollectGarbage(){
        auto [first,last]=std::ranges::remove_if(m_Widgets,[](const auto& pair){
            bool IsDestoryed=pair.first&&pair.first->IsDestroyed();
            if(IsDestoryed&&pair.second==MemoryMode::INTERNAL_MANAGEMENT){
                delete pair.first;
            }
            return IsDestoryed;
        });
        m_Widgets.erase(first,last);
    }
    void WidgetContainer::DrawWidgets(){
        CollectGarbage();
        if(m_IsReverseDraw){
            std::ranges::for_each(m_Widgets|std::views::reverse,&Widget::Draw,[](const auto& p){
                return p.first;
            });
        }
        else{
            std::ranges::for_each(m_Widgets,&Widget::Draw,[](const auto& p){
                return p.first;
            });
        }
    }
}