module widget_container;
import stl;
import widget;

namespace Engine::inline UI{
    WidgetContainer::~WidgetContainer(){
        RemoveAllWidgets();
    }
    void WidgetContainer::RemoveWidget(Widget& widget){
        auto it=std::ranges::find_if(m_widgets,[&widget](const auto& pair){
            return pair.first==&widget;
        });
        if(it!=m_widgets.end()){
            if(it->second==MemoryMode::INTERNAL_MANAGEMENT){
                delete it->first;
            }
            m_widgets.erase(it);
        }
    }
    void WidgetContainer::RemoveAllWidgets(){
        std::ranges::for_each(m_widgets,[](const auto& pair){
            if(pair.second==MemoryMode::INTERNAL_MANAGEMENT){
                delete pair.first;
            }
        });
        m_widgets.clear();
    }
    void WidgetContainer::ConsiderWidget(Widget& widget, bool manageMemory){
        m_widgets.emplace_back(&widget, manageMemory ? MemoryMode::INTERNAL_MANAGEMENT : MemoryMode::EXTERNAL_MANAGEMENT);
        widget.SetParent(this);
    }
    void WidgetContainer::UnconsiderWidget(Widget& widget){
        auto it=std::ranges::find_if(m_widgets,[&widget](const auto& pair){
            return pair.first==&widget;
        });
        if(it!=m_widgets.end()){
            widget.SetParent(nullptr);
            m_widgets.erase(it);
        }
    }
    void WidgetContainer::CollectGarbage(){
        auto [first,last]=std::ranges::remove_if(m_widgets,[](const auto& pair){
            bool IsDestoryed=pair.first&&pair.first->IsDestroyed();
            if(IsDestoryed&&pair.second==MemoryMode::INTERNAL_MANAGEMENT){
                delete pair.first;
            }
            return IsDestoryed;
        });
        m_widgets.erase(first,last);
    }
    void WidgetContainer::DrawWidgets(){
        CollectGarbage();
        if(m_isReverseDraw){
            std::ranges::for_each(m_widgets|std::views::reverse,&Widget::Draw,[](const auto& p){
                return p.first;
            });
        }
        else{
            std::ranges::for_each(m_widgets,&Widget::Draw,[](const auto& p){
                return p.first;
            });
        }
    }
}