export module panel;

import drawable;
import widget_container;

namespace Engine::inline UI{
    export class Panel: public Drawable,public WidgetContainer{
    public:
        Panel();
        ~Panel()=default;
        void Draw() override;
        const std::string& GetPanelID() const;
    protected:
        virtual void DrawImpl() = 0;
    public:
        bool IsEnabled{true};
    protected:
        std::string m_PanelID{""};
    private:
        inline static uint64_t PanelCounter{0};
    };
}

module : private;

namespace Engine::inline UI{
    Panel::Panel(){
        m_PanelID=std::to_string(PanelCounter++);
    }
    void Panel::Draw(){
        if(IsEnabled){
            DrawImpl();
        }
    }
    const std::string& Panel::GetPanelID() const{
        return m_PanelID;
    }
}