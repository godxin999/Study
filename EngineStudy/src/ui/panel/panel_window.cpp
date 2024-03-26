module panel_window;

import panel_transformable;
import event;
import stl;
import glm;
import converter;
import imgui;

namespace Engine::inline UI{
    PanelWindow::PanelWindow(const std::u8string& name,bool isOpened,const PanelWindowSettings& settings):
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
        m_isOpened(isOpened){
        autoSize=settings.autoSize;
    }

    void PanelWindow::Open(){
        if(!m_isOpened){
            m_isOpened=true;
            OpenEvent();        
        }
    }

    void PanelWindow::Close(){
        if(m_isOpened){
            m_isOpened=false;
            CloseEvent();
        }
    }

    void PanelWindow::Focus(){
        ImGui::SetWindowFocus(reinterpret_cast<const char*>((name+panelID).c_str()));
    }

    void PanelWindow::SetOpenState(bool isOpened){
        if(isOpened!=m_isOpened){
            m_isOpened=isOpened;
            if(m_isOpened){
                OpenEvent();
            }
            else{
                CloseEvent();
            }
        }
    }

    bool PanelWindow::IsOpened() const{
        return m_isOpened;
    }

    bool PanelWindow::IsHovered() const{
        return m_isHovered;
    }

    bool PanelWindow::IsFocused() const{
        return m_isFocused;
    }

    bool PanelWindow::IsAppearing() const{
        if(auto window=ImGui::FindWindowByName(reinterpret_cast<const char*>((name+panelID).c_str()))){
            return window->Appearing;
        }
        else{
            return false;
        }
    }

    void PanelWindow::ScrollToTop(){
        m_mustScrollToTop=true;
    }

    void PanelWindow::ScrollToBottom(){
        m_mustScrollToBottom=true;
    }

    bool PanelWindow::IsScrollToTop() const{
        return m_scrolledToTop;
    }

    bool PanelWindow::IsScrollToBottom() const{
        return m_scrolledToBottom;
    }

    void PanelWindow::DrawImpl(){
        if(m_isOpened){
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

            if(ImGui::Begin(reinterpret_cast<const char*>((name+panelID).c_str()),(closable?&m_isOpened:nullptr),windowFlags)){
                m_isHovered=ImGui::IsWindowHovered();
                m_isFocused=ImGui::IsWindowFocused();

                auto scrollY=ImGui::GetScrollY();

                m_scrolledToTop=(scrollY==ImGui::GetScrollMaxY());
                m_scrolledToBottom=(scrollY==0.f);

                Update();

                if(m_mustScrollToTop){
                    ImGui::SetScrollY(0.f);
                    m_mustScrollToTop=false;
                }
                if(m_mustScrollToBottom){
                    ImGui::SetScrollY(ImGui::GetScrollMaxY());
                    m_mustScrollToBottom=false;
                }
                DrawWidgets();
            }
            ImGui::End();
            
            if(!m_isOpened){
                CloseEvent();
            }
        }
    }
}