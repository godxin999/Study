module editor;

import stl;
import canvas;
import panel_manager;
import editor_renderer;
import context;
import device;

namespace Engine::inline editor{
    Editor::Editor(Context& context):m_Context(context),m_EditorRenderer(context),m_PanelManager(m_Canvas){
        SetupUI();
    }
    
    Editor::~Editor(){

    }
    
    void Editor::PreUpdate(){
        m_Context.device->PollEvents();
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
        m_Context.windowManager->SwapBuffers();
        m_Context.inputManager->ClearStates();
    }
    
    void Editor::SetupUI(){

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
        m_EditorRenderer.RenderUI();
    }
    
    void Editor::UpdatePlayMode(float deltaTime){

    }
    
    void Editor::UpdateEditMode(float deltaTime){

    }
}