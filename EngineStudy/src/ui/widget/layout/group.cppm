export module group;

import stl;
import widget;
import widget_container;

namespace Engine::inline UI{
    export class Group:public WidgetContainer,public Widget{
    public:
        Group()=default;
        ~Group()=default;
    };
}