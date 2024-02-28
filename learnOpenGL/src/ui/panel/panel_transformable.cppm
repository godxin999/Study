export module panel_transformable;

import glm;
import alignment;
import panel;

namespace Engine::inline UI{
    export class PanelTransformable:public Panel{
    public:
        PanelTransformable(
            const glm::vec2& defaultPosition=glm::vec2(-1.f), 
            const glm::vec2& defaultSize=glm::vec2(-1.f), 
            const HorizontalAlignment& defaultHorizontalAlignment=HorizontalAlignment::LEFT, 
            const VerticalAlignment& defaultVerticalAlignment=VerticalAlignment::TOP,
            bool IsIgnoreConfigFile=false
        );
        ~PanelTransformable()=default;
        void SetPostion(const glm::vec2& position);
        void SetSize(const glm::vec2& size);
        void SetAlignment(HorizontalAlignment horizontalAlignment,VerticalAlignment verticalAlignment);
        const glm::vec2& GetPosition() const;
        const glm::vec2& GetSize() const;
        HorizontalAlignment GetHorizontalAlignment() const;
        VerticalAlignment GetVerticalAlignment() const;
    protected:
        void Update();
        virtual void DrawImpl()=0;
    private:
        glm::vec2 CalculatePositionAlignmentOffset(bool defaultValue=false);
        void UpdatePosition();
        void UpdateSize();
    public:
        bool autoSize{true};
    protected:
        glm::vec2 m_DefaultPosition;
        glm::vec2 m_DefaultSize;
        HorizontalAlignment m_DefaultHorizontalAlignment;
        VerticalAlignment m_DefaultVerticalAlignment;
        bool m_IsIgnoreConfigFile;

        glm::vec2 m_Position{0.f};
        glm::vec2 m_Size{0.f};
        HorizontalAlignment m_HorizontalAlignment{HorizontalAlignment::LEFT};
        VerticalAlignment m_VerticalAlignment{VerticalAlignment::TOP};
        bool m_IsPositionChanged{false};
        bool m_IsSizeChanged{false};
        bool m_IsAlignmentChanged{false};
        bool m_FirstFrame{true};
    };
}