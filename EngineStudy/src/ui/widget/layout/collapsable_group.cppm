export module collapsable_group;

import stl;
import imgui;
import group;
import event;

namespace Engine::inline UI{
    export class CollapsableGroup : public Group{
    public:
        CollapsableGroup()=default;
        ~CollapsableGroup()=default;
        CollapsableGroup(const std::u8string& name);
    protected:
        void DrawImpl() override;
    public:
        std::u8string name{};
        bool closable{false};
        bool isExpanded{false};
        Event<> CollapseEvent{};
        Event<> ExpandEvent{};
    };
}

module : private;

namespace Engine::inline UI{
    CollapsableGroup::CollapsableGroup(const std::u8string& name):name(name){

    }
    
    void CollapsableGroup::DrawImpl(){
        bool previousState=isExpanded;
        if(ImGui::CollapsingHeader(reinterpret_cast<const char*>(name.c_str()),closable?&isExpanded:nullptr)){
            DrawWidgets();
        }

        if(previousState!=isExpanded){
            if(isExpanded){
                ExpandEvent();
            }
            else{
                CollapseEvent();
            }
        }
    }
}