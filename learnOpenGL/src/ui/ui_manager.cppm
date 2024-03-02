module;
#include <GLFW/glfw3.h>
#include <imgui.h>
export module ui_manager;

import ui_style;
import stl;

namespace Engine::inline UI{
    export class Canvas;
    export class UIManager{
    public:
        UIManager(GLFWwindow* window, UIStyle style=UIStyle::IM_DARK_STYLE,const std::string& glslVersion="#version 150");
        ~UIManager();
        void ApplyStyle(UIStyle style);
        bool LoadFont(const std::string& name, const std::string& path, float size);
        bool UnloadFont(const std::string& name);
        bool UseFont(const std::string& name);
        void UseDefaultFont();
        void EnableEditorLayoutSave(bool enable);
        [[nodiscard]]bool IsEditorLayoutSaveEnabled()const;
        void SetEditorLayoutSaveFilename(const std::string& filename);
        void SetEditorLayoutAutoSaveInterval(float interval);
        float GetEditorLayoutAutoSaveInterval()const;
        void EnableDocking(bool enable);
        [[nodiscard]]bool IsDockingEnabled()const;
        void ResetLayout(const std::string& layout)const;
        void SetCanvas(Canvas& canvas);
        void RemoveCanvas();
        void Render();
    private:
        bool m_DockingState{false};
        Canvas* m_Canvas{nullptr};
        std::unordered_map<std::string,ImFont*> m_Fonts;
        std::string m_LayoutSaveFilename{"imgui.ini"};
    };
}