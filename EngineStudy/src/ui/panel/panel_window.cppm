export module panel_window;

import panel_transformable;
import event;
import stl;
import glm;

namespace Engine::inline UI{
    export struct PanelWindowSettings{
        bool closable{false};
        bool resizable{true};
        bool movable{true};
        bool dockable{false};//是否可停靠
        bool scrollable{true};
        bool hideBackground{false};
        bool forceHorizontalScrollbar{false};
        bool forceVerticalScrollbar{false};
        bool allowHorizontalScrollbar{false};
        bool bringToFrontOnFocus{true};//获得焦点时是否置顶
        bool collapsable{false};//是否可折叠
        bool allowInputs{true};
        bool titleBar{true};
        bool autoSize{false};
    };
    export class PanelWindow:public PanelTransformable{
    public:
        PanelWindow(
            const std::u8string& name=u8"",
            bool isOpened=true,
            const PanelWindowSettings& settings=PanelWindowSettings{}
        );
        virtual ~PanelWindow()=default;
        void Open();
        void Close();
        void Focus();
        void SetOpenState(bool isOpened);
        [[nodiscard]]bool IsOpened() const;
        [[nodiscard]]bool IsHovered() const;
        [[nodiscard]]bool IsFocused() const;
        [[nodiscard]]bool IsAppearing() const;
        void ScrollToTop();
        void ScrollToBottom();
        [[nodiscard]]bool IsScrollToTop() const;
        [[nodiscard]]bool IsScrollToBottom() const;
    protected:
        void DrawImpl() override;
    public:
        std::u8string name{};

        glm::vec2 minSize{0.f};
        glm::vec2 maxSize{10000.f};

        bool closable{false};
        bool resizable{true};
        bool movable{true};
        bool dockable{false};
        bool scrollable{true};
        bool hideBackground{false};
        bool forceHorizontalScrollbar{false};
        bool forceVerticalScrollbar{false};
        bool allowHorizontalScrollbar{false};
        bool bringToFrontOnFocus{true};
        bool collapsable{false};
        bool allowInputs{true};
        bool titleBar{true};

        Event<> OpenEvent{};
        Event<> CloseEvent{};
    private:
        bool m_isOpened{true};
        bool m_isHovered{false};
        bool m_isFocused{false};
        bool m_mustScrollToBottom{false};
        bool m_mustScrollToTop{false};
        bool m_scrolledToBottom{false};
        bool m_scrolledToTop{false};
    };
}