export module canvas;

import drawable;
import stl;
import panel;

namespace Engine::inline UI{
    export class Canvas: public Drawable{
    public:
        void Draw() override;
        void AddPanel(Panel& panel);
        void RemovePanel(Panel& panel);
        void RemoveAllPanels();
        void MakeDockSpace(bool state);
        [[nodiscard]]bool IsDockSpace() const;
    private:
        std::vector<std::reference_wrapper<Panel>> m_Panels;
        bool m_IsDockSpace{false};
    };
}