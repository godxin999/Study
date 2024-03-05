module;
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
module canvas;

import stl;

namespace Engine::inline UI{
    void Canvas::AddPanel(Panel& panel){
        m_panels.push_back(std::ref(panel));
    }
    void Canvas::RemovePanel(Panel& panel){
        auto [first,last]=std::ranges::remove_if(m_panels,[&panel](auto& p){
            return &p.get()==&panel;
        });
        m_panels.erase(first,last);
    }
    void Canvas::RemoveAllPanels(){
        m_panels.clear();
    }
    void Canvas::MakeDockSpace(bool state){
        m_isDockSpace=state;
    }
    bool Canvas::IsDockSpace() const{
        return m_isDockSpace;
    }
    void Canvas::Draw(){
        if(!m_panels.empty()){
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            if(m_isDockSpace){
                ImGuiViewport* viewport=ImGui::GetMainViewport();
                ImGui::SetNextWindowPos(viewport->Pos);
                ImGui::SetNextWindowSize(viewport->Size);
                ImGui::SetNextWindowViewport(viewport->ID);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding,0.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize,0.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,ImVec2(0.0f,0.0f));
                
                ImGui::Begin("DockSpace",nullptr,
                ImGuiWindowFlags_NoTitleBar|
                ImGuiWindowFlags_NoCollapse|
                ImGuiWindowFlags_NoResize|
                ImGuiWindowFlags_NoMove|
                ImGuiWindowFlags_NoBringToFrontOnFocus|
                ImGuiWindowFlags_MenuBar|
                ImGuiWindowFlags_NoDocking);

                ImGuiID dockSpaceID=ImGui::GetID("DockSpace");
                ImGui::DockSpace(dockSpaceID,ImVec2(0.0f,0.0f),ImGuiDockNodeFlags_PassthruCentralNode);
                ImGui::SetWindowPos(ImVec2(0.0f,0.0f));
                ImGui::SetWindowSize(ImGui::GetIO().DisplaySize);
                ImGui::End();
                ImGui::PopStyleVar(3);
            }
            for(const auto& panel:m_panels){
                panel.get().Draw();
            }

            ImGui::Render();
        }
    }
}