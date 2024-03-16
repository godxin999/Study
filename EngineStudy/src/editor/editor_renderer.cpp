module editor_renderer;

import context;

namespace Engine::inline editor{
    EditorRenderer::EditorRenderer(Context& context):m_context(context){

    }
    
    void EditorRenderer::RenderUI(){
        m_context.uiManager->Render();
    }
}