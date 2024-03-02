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
        bool enable{true};
    protected:
        std::string panelID{""};
    private:
        inline static uint64_t panelCounter{0};
    };
}

module : private;

namespace Engine::inline UI{
    Panel::Panel(){
        panelID=std::to_string(panelCounter++);
    }
    void Panel::Draw(){
        if(enable){
            DrawImpl();
        }
    }
    const std::string& Panel::GetPanelID() const{
        return panelID;
    }
}