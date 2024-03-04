export module widget;
import drawable;
import stl;
import pluginable;
import imgui;

namespace Engine::inline UI{
    export class WidgetContainer;
    export class Widget : public Drawable, public Pluginable{
    public:
        Widget();
        virtual ~Widget()=default;
        void Draw() override;
        void LinkTo(const Widget& widget){
            widgetID=widget.widgetID;
        }
        void Destroy(){
            m_IsDestroyed=true;
        }
        bool IsDestroyed() const{
            return m_IsDestroyed;
        }
        WidgetContainer* GetParent() const{
            return parent;
        }
        void SetParent(WidgetContainer* p_parent){
            parent=p_parent;
        }
        bool HasParent() const{
            return parent;
        }
    protected:
        virtual void DrawImpl()=0;
    public:
        bool enable{true};
        bool isLineBreak{true};
    protected:
        std::u8string widgetID{};
        WidgetContainer* parent{nullptr};
        bool isAutoExecutePlugins{true};
    private:
        inline static uint64_t WidgetCounter{0};
        bool m_IsDestroyed{false};
    };
}

module : private;

namespace Engine::inline UI{
    Widget::Widget(){
        //imgui会隐藏##开头的名称
        widgetID=u8"##"+std::u8string(reinterpret_cast<const char8_t*>(std::to_string(WidgetCounter++).c_str()));
    }
    
    void Widget::Draw(){
        if(enable){
            DrawImpl();

            if(isAutoExecutePlugins){
                ExecutePlugins();
            }

            if(!isLineBreak){
                ImGui::SameLine();
            }
        }
    }
}