module editor;

import stl;
import canvas;
import menu_bar;
import scene_view;
import panel_manager;
import editor_renderer;
import context;
import device;
import service_locator;
import renderer;
import panel_window;

namespace Engine::inline editor{
    Editor::Editor(Context& context):m_context(context),m_editorRenderer(context),m_panelManager(m_canvas){
        ServiceLocator::Register<EditorRenderer>(m_editorRenderer);
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
        PanelWindowSettings settings;
        settings.closable=true;
        settings.collapsable=true;
        settings.dockable=true;

        m_panelManager.CreatePanel<MenuBar>(u8"Menu Bar");
        m_panelManager.CreatePanel<SceneView>(u8"Scene View",true,settings);
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