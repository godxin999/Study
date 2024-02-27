export module panel;

import drawable;
import widget_container;

namespace Engine::inline UI{
    export class Panel: public Drawable,public WidgetContainer{
    public:
        Panel();
        ~Panel()=default;
        void Draw() override;
        uint64_t GetPanelID() const;
    protected:
        virtual void DrawImpl() = 0;
    public:
        bool IsEnabled{true};
    protected:
        uint64_t m_PanelID{};
    private:
        inline static uint64_t PanelCounter{};
    };
}