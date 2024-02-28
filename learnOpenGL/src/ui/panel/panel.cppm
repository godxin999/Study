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

module : private;

namespace Engine::inline UI{
    Panel::Panel(){
        m_PanelID=PanelCounter++;
    }
    void Panel::Draw(){
        if(IsEnabled){
            DrawImpl();
        }
    }
    uint64_t Panel::GetPanelID() const{
        return m_PanelID;
    }
}