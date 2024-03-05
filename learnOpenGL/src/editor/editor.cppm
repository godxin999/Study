export module editor;

import stl;
import canvas;
import panel_manager;
import editor_renderer;

namespace Engine::inline editor{
    export class Context;
    export class Editor{
    public:
        Editor(Context& context);
        ~Editor();
        void PreUpdate();
        void Update(float deltaTime);
        void PostUpdate();
    private:
        void SetupUI();
        void HandleGlobalShortcuts();
        void UpdateCurrentEditorMode(float deltaTime);
        void UpdateEditorPanels(float deltaTime);
        void PrepareRendering(float deltaTime);
        void RenderViews(float deltaTime);
        void RenderEditorUI();
        void UpdatePlayMode(float deltaTime);
        void UpdateEditMode(float deltaTime);
        uint64_t m_elapsedFrames{};
        Context& m_context;
        Canvas m_canvas;
        PanelManager m_panelManager;
        EditorRenderer m_editorRenderer;
    };
};
