module editor_renderer;

import context;

namespace Engine::inline editor{
    EditorRenderer::EditorRenderer(Context& context):m_Context(context){

    }
    
    void EditorRenderer::RenderUI(){
        m_Context.uiManager->Render();
    }
}