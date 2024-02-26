export module widget;
import drawable;
import stl;
import pluginable;

namespace Engine::inline UI{
    export class WidgetContainer;
    export class Widget : public Drawable, public Pluginable{
    public:
        Widget();
        virtual ~Widget()=default;
        void Draw() override;
        void LinkTo(const Widget& widget){
            m_WidgetID=widget.m_WidgetID;
        }
        void Destroy(){
            m_IsDestroyed=true;
        }
        bool IsDestroyed() const{
            return m_IsDestroyed;
        }
        void SetParent(WidgetContainer* parent);
        WidgetContainer* GetParent() const;
        bool HasParent() const{
            return m_Parent;
        }
    protected:
        virtual void DrawImpl()=0;
    public:
        bool m_IsEnabled{true};
        bool m_IsLineBreak{true};
    protected:
        WidgetContainer* m_Parent{nullptr};
        uint64_t m_WidgetID{0};
        bool m_IsAutoExecutePlugins{true};
    private:
        inline static uint64_t m_WidgetCounter{0};
        bool m_IsDestroyed{false};
    };
}
