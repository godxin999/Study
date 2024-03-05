module menu_bar;

import stl;
import panel_window;
import menu_list;
import menu_item;
import service_locator;
import input_manager;
import window_manager;
import log_manager;
import separator;

namespace Engine::inline editor{
    MenuBar::MenuBar(){
        CreateFileMenu();
        CreateWindowMenu();
    }
    
    void MenuBar::HandleShortcuts(float deltaTime){

    }

    void MenuBar::RegisterPanel(const std::u8string& name,PanelWindow& panel){
        //����panelwindow��Ӧ�Ĳ˵���
        auto& menuItem=m_windowMenu->CreateWidget<MenuItem>(name,u8"",true,true);
        //����˵���������ô�ͻᴥ��panelwindow�Ĵ򿪻�ر�
        menuItem.ValueChangeEvent+=[&panel](bool value){
            panel.SetOpenState(value);
        };
        m_panels.emplace(name,std::make_pair(std::ref(panel),std::ref(menuItem)));
    }
    
    void MenuBar::CreateFileMenu(){
        auto& fileMenu=CreateWidget<MenuList>(u8"�ļ�");
        fileMenu.CreateWidget<MenuItem>(u8"�½�",u8"Ctrl + N");
        fileMenu.CreateWidget<MenuItem>(u8"����",u8"Ctrl + S");
        fileMenu.CreateWidget<MenuItem>(u8"���Ϊ",u8"Ctrl + Shift + S");
        fileMenu.CreateWidget<MenuItem>(u8"�˳�",u8"Alt + F4").ClickEvent+=[](){
            ServiceLocator::Get<WindowManager>().SetShouldClose(true);
            ServiceLocator::Get<LogManager>().Log(LogLevel::info,"Exit The Editor");
        };
    }

    void MenuBar::CreateWindowMenu(){
        m_windowMenu=&CreateWidget<MenuList>(u8"����");
        m_windowMenu->CreateWidget<MenuItem>(u8"�ر�����").ClickEvent+=[this](){
            SetEveryWindowOpenState(false);
            ServiceLocator::Get<LogManager>().Log(LogLevel::info,"Close All PanelWindows");
        };
        m_windowMenu->CreateWidget<MenuItem>(u8"������").ClickEvent+=[this](){
            SetEveryWindowOpenState(true);
            ServiceLocator::Get<LogManager>().Log(LogLevel::info,"Open All PanelWindows");
        };
        //����һ���ָ���������Ĳ˵��ע���panelwindow��Ӧ�Ĳ˵���
        m_windowMenu->CreateWidget<Separator>();
        m_windowMenu->ClickEvent+=[this](){
            UpdateToggleableItems();
        };
    }

    void MenuBar::UpdateToggleableItems(){
        for(auto& [panelwindow,menuItem]:m_panels|std::views::values){
            menuItem.get().isChecked=panelwindow.get().IsOpened();
        }
    }

    void MenuBar::SetEveryWindowOpenState(bool state){
        for(auto& [panelwindow,menuItem]:m_panels|std::views::values){
            panelwindow.get().SetOpenState(state);
        }
    }
}