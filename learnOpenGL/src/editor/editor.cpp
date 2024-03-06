module editor;

import stl;
import canvas;
import menu_bar;
import panel_manager;
import editor_renderer;
import context;
import device;
import service_locator;
import renderer;

namespace Engine::inline editor{
    Editor::Editor(Context& context):m_context(context),m_editorRenderer(context),m_panelManager(m_canvas){
        ServiceLocator::Register<EditorRenderer>(m_editorRenderer);
        ServiceLocator::Register<Renderer>(*m_context.renderer);
        SetupUI();
    }
    
    Editor::~Editor(){

    }
    
    void Editor::PreUpdate(){
        m_context.device->PollEvents();
        m_context.renderer->SetClearColor(0.f,0.f,0.f);
        m_context.renderer->Clear();
        m_context.renderer->ClearFrameInfo();
    }
    
    void Editor::Update(float deltaTime){
        HandleGlobalShortcuts();
        UpdateCurrentEditorMode(deltaTime);
        PrepareRendering(deltaTime);
        RenderViews(deltaTime);
        UpdateEditorPanels(deltaTime);
        RenderEditorUI();
    }
    
    void Editor::PostUpdate(){
        m_context.windowManager->SwapBuffers();
        m_context.inputManager->ClearStates();
    }
    
    void Editor::SetupUI(){
        m_panelManager.CreatePanel<MenuBar>(u8"Menu Bar");
        m_canvas.MakeDockSpace(true);
        m_context.uiManager->SetCanvas(m_canvas);
    }
    
    void Editor::HandleGlobalShortcuts(){

    }
    
    void Editor::UpdateCurrentEditorMode(float deltaTime){

    }
    
    void Editor::UpdateEditorPanels(float deltaTime){

    }
    
    void Editor::PrepareRendering(float deltaTime){

    }
    
    void Editor::RenderViews(float deltaTime){

    }
    
    void Editor::RenderEditorUI(){
        m_editorRenderer.RenderUI();
    }
    
    void Editor::UpdatePlayMode(float deltaTime){

    }
    
    void Editor::UpdateEditMode(float deltaTime){

    }
}