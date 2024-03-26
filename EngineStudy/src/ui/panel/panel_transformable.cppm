export module panel_transformable;

import glm;
import alignment;
import panel;

namespace Engine::inline UI{
    export class PanelTransformable:public Panel{
    public:
        PanelTransformable(
            const glm::vec2& p_defaultPosition=glm::vec2(-1.f), 
            const glm::vec2& p_defaultSize=glm::vec2(-1.f), 
            HorizontalAlignment p_defaultHorizontalAlignment=HorizontalAlignment::LEFT, 
            VerticalAlignment p_defaultVerticalAlignment=VerticalAlignment::TOP,
            bool p_isIgnoreConfigFile=false
        );
        virtual ~PanelTransformable()=default;
        void SetPostion(const glm::vec2& position);
        void SetSize(const glm::vec2& size);
        void SetAlignment(HorizontalAlignment horizontalAlignment,VerticalAlignment verticalAlignment);
        const glm::vec2& GetPosition() const;
        const glm::vec2& GetSize() const;
        HorizontalAlignment GetHorizontalAlignment() const;
        VerticalAlignment GetVerticalAlignment() const;
    protected:
        void Update();
    private:
        glm::vec2 CalculatePositionAlignmentOffset(bool defaultValue=false);
        void UpdatePosition();
        void UpdateSize();
    public:
        bool autoSize{true};
    protected:
        glm::vec2 defaultPosition;
        glm::vec2 defaultSize;
        HorizontalAlignment defaultHorizontalAlignment;
        VerticalAlignment defaultVerticalAlignment;
        bool isIgnoreConfigFile;

        glm::vec2 position{0.f};
        glm::vec2 size{0.f};
        HorizontalAlignment horizontalAlignment{HorizontalAlignment::LEFT};
        VerticalAlignment verticalAlignment{VerticalAlignment::TOP};
        bool isPositionChanged{false};
        bool isSizeChanged{false};
        bool isAlignmentChanged{false};
        bool firstFrame{true};
    };
}