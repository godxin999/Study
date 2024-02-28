module;
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
module ui_manager;

import ui_style;
import stl;
import canvas;

namespace Engine::inline UI{
    UIManager::UIManager(GLFWwindow* window, UIStyle style,const std::string& glslVersion){
        ImGui::CreateContext();
        //设置窗口移动只能通过标题栏
        ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;
        EnableDocking(false);
        ApplyStyle(style);
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glslVersion.c_str());
    }
    UIManager::~UIManager(){
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
    void UIManager::ApplyStyle(UIStyle style){
        //用于设置自定义样式
        ImGuiStyle* Imstyle=&ImGui::GetStyle();
        switch(style){
            case UIStyle::IM_CLASSIC_STYLE:
                ImGui::StyleColorsClassic();
                break;
            case UIStyle::IM_DARK_STYLE:
                ImGui::StyleColorsDark();
                break;
            case UIStyle::IM_LIGHT_STYLE:
                ImGui::StyleColorsLight();
                break;
        }
    }
    bool UIManager::LoadFont(const std::string& name, const std::string& path, float size){
        if(m_Fonts.find(name)==m_Fonts.end()){
            ImFont* font=ImGui::GetIO().Fonts->AddFontFromFileTTF(path.c_str(),size);
            if(font){
                m_Fonts.insert({name,font});
                return true;
            }
        }
        return false;
    }
    bool UIManager::UnloadFont(const std::string& name){
        if(m_Fonts.find(name)!=m_Fonts.end()){
            m_Fonts.erase(name);
            return true;
        }
        return false;
    }
    bool UIManager::UseFont(const std::string& name){
        if(m_Fonts.find(name)!=m_Fonts.end()){
            ImGui::GetIO().FontDefault=m_Fonts[name];
            return true;
        }
        return false;
    }
    void UIManager::UseDefaultFont(){
        ImGui::GetIO().FontDefault=nullptr;
    }
    void UIManager::EnableEditorLayoutSave(bool enable){
        if(enable){
            ImGui::GetIO().IniFilename=m_LayoutSaveFilename.c_str();
        }
        else{
            ImGui::GetIO().IniFilename=nullptr;
        }
    }
    bool UIManager::IsEditorLayoutSaveEnabled()const{
        return ImGui::GetIO().IniFilename;
    }
    void UIManager::SetEditorLayoutSaveFilename(const std::string& filename){
        m_LayoutSaveFilename=filename;
    }
    void UIManager::SetEditorLayoutAutoSaveInterval(float interval){
        ImGui::GetIO().IniSavingRate=interval;
    }
    float UIManager::GetEditorLayoutAutoSaveInterval()const{
        return ImGui::GetIO().IniSavingRate;
    }
    void UIManager::EnableDocking(bool enable){
        m_DockingState=enable;
        if(enable){
            ImGui::GetIO().ConfigFlags|=ImGuiConfigFlags_DockingEnable;
        }
        else{
            ImGui::GetIO().ConfigFlags&=~ImGuiConfigFlags_DockingEnable;
        }
    }
    bool UIManager::IsDockingEnabled()const{
        return m_DockingState;
    }
    void UIManager::ResetLayout(const std::string& layout)const{
        ImGui::LoadIniSettingsFromDisk(layout.c_str());
    }
    void UIManager::SetCanvas(Canvas& canvas){
        RemoveCanvas();
        m_Canvas=&canvas;
    }
    void UIManager::RemoveCanvas(){
        m_Canvas=nullptr;
    }
    void UIManager::Render(){
        if(m_Canvas){
            m_Canvas->Draw();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }
    }
}