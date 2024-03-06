export module view;

import stl;
import glm;
import panel_window;
import image;
import buffer;
import camera;
import editor_renderer;
import service_locator;
import imgui;
import context;
import renderer;

namespace Engine::inline editor{
    export class View:public PanelWindow{
    public:
        View(const std::u8string& name,bool isOpened,const PanelWindowSettings& settings);
        ~View()=default;
        void Update(float deltaTime);
        void DrawImpl() override;
        void Render();
        Camera& GetCamera();
        const glm::vec3& GetGridColor() const;
        std::pair<uint16_t,uint16_t> GetSafeSize();
        void SetUBO();
    protected:
        virtual void RenderImpl()=0;
        void PrepareCamera();
    private:
        EditorRenderer& m_editorRenderer;
        Camera m_camera{};
        Image* m_image{nullptr};
        FrameBuffer m_frameBuffer{};
        glm::vec3 m_gridColor{0.176f, 0.176f, 0.176f};
    };
}

module : private;

namespace Engine::inline editor{
    View::View(const std::u8string& name,bool isOpened,const PanelWindowSettings& settings):PanelWindow(name,isOpened,settings),m_editorRenderer(ServiceLocator::Get<EditorRenderer>()){
        m_image=&CreateWidget<Image>(m_frameBuffer.GetRenderTextureID(),glm::vec2(0.f,0.f));
        scrollable=false;
    }

    void View::Update(float deltaTime){
		auto [width,height]=GetSafeSize();
		m_image->size=glm::vec2(static_cast<float>(width),static_cast<float>(height));
		m_frameBuffer.Resize(width,height);
    }

    void View::DrawImpl(){
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,ImVec2(0,0));
		PanelWindow::DrawImpl();
		ImGui::PopStyleVar();
    }

    void View::Render(){
        SetUBO();
        auto [width,height]=GetSafeSize();
        ServiceLocator::Get<Renderer>().SetViewPort(0,0,width,height);
        RenderImpl();
    }

    Camera& View::GetCamera(){
        return m_camera;
    }

    const glm::vec3& View::GetGridColor() const{
        return m_gridColor;
    }
    //返回忽略标题栏的窗口大小
    std::pair<uint16_t,uint16_t> View::GetSafeSize(){
        auto size=GetSize();
        size.y-=25;//25是标题栏的高度
        return {static_cast<uint16_t>(size.x),static_cast<uint16_t>(size.y)};
    }
	
    void View::SetUBO(){
        auto& context=Engine::ServiceLocator::Get<Engine::Context>();
        context.ubo->SetSubData(m_camera.GetViewMatrix(),0);
        context.ubo->SetSubData(m_camera.GetProjMatrix(),sizeof(glm::mat4));
        context.ubo->SetSubData(m_camera.GetPosition(),sizeof(glm::mat4)*2);
    }

    void View::PrepareCamera(){
		auto [width,height]=GetSafeSize();
		m_camera.UpdateCameraMatrices(width,height);
    }
}