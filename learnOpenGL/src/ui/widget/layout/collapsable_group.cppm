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
        CollapsableGroup(const std::string& name);
    protected:
        void DrawImpl() override;
    public:
        std::string name{""};
        bool closable{false};
        bool isExpanded{false};
        Event<> collapseEvent{};
        Event<> expandEvent{};
    };
}

module : private;

namespace Engine::inline UI{
    CollapsableGroup::CollapsableGroup(const std::string& name):name(name){

    }
    void CollapsableGroup::DrawImpl(){
        bool previousState=isExpanded;
        if(ImGui::CollapsingHeader(name.c_str(),closable?&isExpanded:nullptr)){
            DrawWidgets();
        }

        if(previousState!=isExpanded){
            if(isExpanded){
                expandEvent.Invoke();
            }
            else{
                collapseEvent.Invoke();
            }
        }
    }
}