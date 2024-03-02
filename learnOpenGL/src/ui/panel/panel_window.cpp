module;
#include <imgui_internal.h>
module panel_window;

import panel_transformable;
import event;
import stl;
import glm;
import converter;

namespace Engine::inline UI{
    PanelWindow::PanelWindow(const std::string& name,bool isOpened,const PanelWindowSettings& settings):
        name(name),
        closable(settings.closable),
        resizable(settings.resizable),
        movable(settings.movable),
        dockable(settings.dockable),
        scrollable(settings.scrollable),
        hideBackground(settings.hideBackground),
        forceHorizontalScrollbar(settings.forceHorizontalScrollbar),
        forceVerticalScrollbar(settings.forceVerticalScrollbar),
        allowHorizontalScrollbar(settings.allowHorizontalScrollbar),
        bringToFrontOnFocus(settings.bringToFrontOnFocus),
        collapsable(settings.collapsable),
        allowInputs(settings.allowInputs),
        titleBar(settings.titleBar),
        m_IsOpened(isOpened){
        autoSize=settings.autoSize;
    }

    void PanelWindow::Open(){
        if(!m_IsOpened){
            m_IsOpened=true;
            OpenEvent.Invoke();        
        }
    }

    void PanelWindow::Close(){
        if(m_IsOpened){
            m_IsOpened=false;
            CloseEvent.Invoke();
        }
    }

    void PanelWindow::Focus(){
        ImGui::SetWindowFocus((name+panelID).c_str());
    }

    void PanelWindow::SetOpenState(bool isOpened){
        if(isOpened!=m_IsOpened){
            m_IsOpened=isOpened;
            if(m_IsOpened){
                OpenEvent.Invoke();
            }
            else{
                CloseEvent.Invoke();
            }
        }
    }

    bool PanelWindow::IsOpened() const{
        return m_IsOpened;
    }

    bool PanelWindow::IsHovered() const{
        return m_IsHovered;
    }

    bool PanelWindow::IsFocused() const{
        return m_IsFocused;
    }

    bool PanelWindow::IsAppearing() const{
        if(auto window=ImGui::FindWindowByName((name+panelID).c_str())){
            return window->Appearing;
        }
        else{
            return false;
        }
    }

    void PanelWindow::ScrollToTop(){
        m_MustScrollToTop=true;
    }

    void PanelWindow::ScrollToBottom(){
        m_MustScrollToBottom=true;
    }

    bool PanelWindow::IsScrollToTop() const{
        return m_ScrolledToTop;
    }

    bool PanelWindow::IsScrollToBottom() const{
        return m_ScrolledToBottom;
    }

    void PanelWindow::DrawImpl(){
        if(m_IsOpened){
            int windowFlags=ImGuiWindowFlags_None;
            if(!resizable)windowFlags|=ImGuiWindowFlags_NoResize;
            if(!movable)windowFlags|=ImGuiWindowFlags_NoMove;
            if(!dockable)windowFlags|=ImGuiWindowFlags_NoDocking;
            if(hideBackground)windowFlags|=ImGuiWindowFlags_NoBackground;
            if(forceHorizontalScrollbar)windowFlags|=ImGuiWindowFlags_AlwaysHorizontalScrollbar;
            if(forceVerticalScrollbar)windowFlags|=ImGuiWindowFlags_AlwaysVerticalScrollbar;
            if(allowHorizontalScrollbar)windowFlags|=ImGuiWindowFlags_HorizontalScrollbar;
            if(!bringToFrontOnFocus)windowFlags|=ImGuiWindowFlags_NoBringToFrontOnFocus;
            if(!collapsable)windowFlags|=ImGuiWindowFlags_NoCollapse;
            if(!allowInputs)windowFlags|=ImGuiWindowFlags_NoInputs;
            if(!titleBar)windowFlags|=ImGuiWindowFlags_NoTitleBar;
            if(!scrollable)windowFlags|=ImGuiWindowFlags_NoScrollWithMouse|ImGuiWindowFlags_NoScrollbar;

            ImGui::SetNextWindowSizeConstraints(Converter::ToImVec2(minSize),Converter::ToImVec2(maxSize));

            if(ImGui::Begin((name+panelID).c_str(),(closable?&m_IsOpened:nullptr),windowFlags)){
                m_IsHovered=ImGui::IsWindowHovered();
                m_IsFocused=ImGui::IsWindowFocused();

                auto scrollY=ImGui::GetScrollY();

                m_ScrolledToTop=(scrollY==ImGui::GetScrollMaxY());
                m_ScrolledToBottom=(scrollY==0.f);

                Update();

                if(m_MustScrollToTop){
                    ImGui::SetScrollY(0.f);
                    m_MustScrollToTop=false;
                }
                if(m_MustScrollToBottom){
                    ImGui::SetScrollY(ImGui::GetScrollMaxY());
                    m_MustScrollToBottom=false;
                }
                DrawWidgets();
            }
            ImGui::End();
            
            if(!m_IsOpened){
                CloseEvent.Invoke();
            }
        }
    }
}