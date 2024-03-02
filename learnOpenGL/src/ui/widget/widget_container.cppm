export module widget_container;
import stl;
import widget;

namespace Engine::inline UI{
    export class WidgetContainer{
    public:
        enum class MemoryMode{
            INTERNAL_MANAGEMENT,
            EXTERNAL_MANAGEMENT
        };
    public:
        WidgetContainer()=default;
        virtual ~WidgetContainer();
        void RemoveWidget(Widget& widget);
        void RemoveAllWidgets();
        void ConsiderWidget(Widget& widget, bool manageMemory = true);
        void UnconsiderWidget(Widget& widget);
        void CollectGarbage();
        void DrawWidgets();
        void ReverseDrawOrder(bool reverse = true){
            m_IsReverseDraw=reverse;
        }
        template <typename T, typename... Args>
        T& CreateWidget(Args&&... args){
            m_Widgets.emplace_back(new T(std::forward<Args>(args)...), MemoryMode::INTERNAL_MANAGEMENT);
            T& widget=*(dynamic_cast<T*>(m_Widgets.back().first));
            widget.SetParent(this);
            return widget;
        }
        std::vector<std::pair<Widget*, MemoryMode>>& GetWidgets(){
            return m_Widgets;
        }
    protected:
        std::vector<std::pair<Widget*, MemoryMode>> m_Widgets{};
        bool m_IsReverseDraw{false};
    };
}