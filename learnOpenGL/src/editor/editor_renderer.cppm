export module editor_renderer;


namespace Engine::inline editor{
    export class Context;
    export class EditorRenderer{
    public:
        EditorRenderer(Context& context);
        ~EditorRenderer()=default;
        void RenderUI();
    private:
        Context& m_Context;
    };
}