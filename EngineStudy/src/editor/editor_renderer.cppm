export module editor_renderer;

import glm;
import camera;

namespace Engine::inline editor{
    export class Context;
    export class EditorRenderer{
    public:
        EditorRenderer(Context& context);
        ~EditorRenderer()=default;
        
        void RenderUI();
        void RenderScene(const glm::vec3& cameraPosition,const Camera& camera);
    private:
        Context& m_context;
    };
}