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
        bool autoSize{false};
    };
    export class PanelWindow:public PanelTransformable{
    public:
        PanelWindow()=default;
        PanelWindow(const std::string& name="",bool isOpened=true,const PanelWindowSettings& settings=PanelWindowSettings{});
        ~PanelWindow()=default;
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
        std::string name{""};

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
        bool m_IsOpened;
        bool m_IsHovered;
        bool m_IsFocused;
        bool m_MustScrollToBottom{false};
        bool m_MustScrollToTop{false};
        bool m_ScrolledToBottom{false};
        bool m_ScrolledToTop{false};
    };
}