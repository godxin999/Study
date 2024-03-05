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
        bool LoadFont(const std::u8string& name, const std::u8string& path, float size);
        bool UnloadFont(const std::u8string& name);
        bool UseFont(const std::u8string& name);
        void UseDefaultFont();
        void EnableEditorLayoutSave(bool enable);
        [[nodiscard]]bool IsEditorLayoutSaveEnabled()const;
        void SetEditorLayoutSaveFilename(const std::u8string& filename);
        void SetEditorLayoutAutoSaveInterval(float interval);
        float GetEditorLayoutAutoSaveInterval()const;
        void EnableDocking(bool enable);
        [[nodiscard]]bool IsDockingEnabled()const;
        void ResetLayout(const std::u8string& layout)const;
        void SetCanvas(Canvas& canvas);
        void RemoveCanvas();
        void Render();
    private:
        bool m_dockingState{false};
        Canvas* m_canvas{nullptr};
        std::unordered_map<std::u8string,ImFont*> m_fonts;
        std::u8string m_layoutSaveFilename{u8"imgui.ini"};
    };
}