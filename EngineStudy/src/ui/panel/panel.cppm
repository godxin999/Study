export module panel;

import drawable;
import widget_container;

namespace Engine::inline UI{
    export class Panel: public Drawable,public WidgetContainer{
    public:
        Panel();
        ~Panel()=default;
        void Draw() override;
        const std::u8string& GetPanelID() const;
    protected:
        virtual void DrawImpl() = 0;
    public:
        bool enable{true};
    protected:
        std::u8string panelID{};
    private:
        inline static uint64_t panelCounter{0};
    };
}

module : private;

namespace Engine::inline UI{
    Panel::Panel(){
        panelID=reinterpret_cast<const char8_t*>(std::to_string(panelCounter++).c_str());
    }
    void Panel::Draw(){
        if(enable){
            DrawImpl();
        }
    }
    const std::u8string& Panel::GetPanelID() const{
        return panelID;
    }
}