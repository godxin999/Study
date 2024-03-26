export module menu_item;

import imgui;
import stl;
import event;
import data_widget;


namespace Engine::inline UI{
    export class MenuItem:public DataWidget<bool>{
    public:
        MenuItem(const std::u8string& name,const std::u8string& shortcut=u8"",bool checkable=false,bool p_isChecked=false);
        ~MenuItem()=default; 
    protected:
        void DrawImpl()override;
    public:
        std::u8string name{};
        std::u8string shortcut{};
        bool checkable{false};
        bool isChecked{false};
        Event<> ClickEvent{};
        Event<bool> ValueChangeEvent{}; 
    };
}

module : private;

namespace Engine::inline UI{
    MenuItem::MenuItem(const std::u8string& name,const std::u8string& shortcut,bool checkable,bool p_isChecked):DataWidget(isChecked),name(name),shortcut(shortcut),checkable(checkable),isChecked(p_isChecked){
        
    }

    void MenuItem::DrawImpl(){
        bool previousValue=isChecked;
        if(ImGui::MenuItem(reinterpret_cast<const char*>((name+widgetID).c_str()),reinterpret_cast<const char*>(shortcut.c_str()),checkable?std::addressof(isChecked):nullptr,enable)){
            ClickEvent();
        }
        if(previousValue!=isChecked){
            ValueChangeEvent(isChecked);
            NotifyChange();
        }
    }
}