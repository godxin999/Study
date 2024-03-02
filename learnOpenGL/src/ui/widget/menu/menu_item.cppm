export module menu_item;

import imgui;
import stl;
import event;
import data_widget;


namespace Engine::inline UI{
    export class MenuItem:public DataWidget<bool>{
    public:
        MenuItem(const std::string& name,const std::string& shortcut="",bool checkable=false,bool p_isChecked=false);
        ~MenuItem()=default; 
    protected:
        void DrawImpl()override;
    public:
        std::string name{""};
        std::string shortcut{""};
        bool checkable{false};
        bool isChecked{false};
        Event<> ClickEvent{};
        Event<bool> ValueChangeEvent{}; 
    };
}

module : private;

namespace Engine::inline UI{
    MenuItem::MenuItem(const std::string& name,const std::string& shortcut,bool checkable,bool p_isChecked):DataWidget(isChecked),name(name),shortcut(shortcut),checkable(checkable),isChecked(p_isChecked){
        
    }

    void MenuItem::DrawImpl(){
        bool previousValue=isChecked;
        if(ImGui::MenuItem((name+widgetID).c_str(),shortcut.c_str(),checkable?std::addressof(isChecked):nullptr,enable)){
            ClickEvent.Invoke();
        }
        if(previousValue!=isChecked){
            ValueChangeEvent.Invoke(isChecked);
            NotifyChange();
        }
    }
}