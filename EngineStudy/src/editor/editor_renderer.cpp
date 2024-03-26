module editor_renderer;

import context;
import glm;
import camera;

namespace Engine::inline editor{
    EditorRenderer::EditorRenderer(Context& context):m_context(context){

    }
    
    void EditorRenderer::RenderUI(){
        m_context.uiManager->Render();
    }

    void EditorRenderer::RenderScene(const glm::vec3& cameraPosition,const Camera& camera){
        m_context.lightSSBO->Bind(0);
        //m_context.renderer->RenderScene(*m_context.sceneManager->GetCurrentScene(),cameraPosition,camera);
        m_context.lightSSBO->Unbind();
    }
}