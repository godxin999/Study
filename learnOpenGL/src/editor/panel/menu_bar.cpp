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

    void MenuBar::RegisterPanel(const std::string& name,PanelWindow& panel){
        //����panelwindow��Ӧ�Ĳ˵���
        auto& menuItem=m_WindowMenu->CreateWidget<MenuItem>(name,"",true,true);
        //����˵���������ô�ͻᴥ��panelwindow�Ĵ򿪻�ر�
        menuItem.ValueChangeEvent+=[&panel](bool value){
            panel.SetOpenState(value);
        };
        m_Panels.emplace(name,std::make_pair(std::ref(panel),std::ref(menuItem)));
    }
    
    void MenuBar::CreateFileMenu(){
        auto& fileMenu=CreateWidget<MenuList>("�ļ�");
        fileMenu.CreateWidget<MenuItem>("�½�","Ctrl + N");
        fileMenu.CreateWidget<MenuItem>("����","Ctrl + S");
        fileMenu.CreateWidget<MenuItem>("���Ϊ","Ctrl + Shift + S");
        fileMenu.CreateWidget<MenuItem>("�˳�","Alt + F4").ClickEvent+=[](){
            ServiceLocator::Get<WindowManager>().SetShouldClose(true);
            ServiceLocator::Get<LogManager>().Log(LogLevel::info,"Exit The Editor");
        };
    }

    void MenuBar::CreateWindowMenu(){
        m_WindowMenu=&CreateWidget<MenuList>("����");
        m_WindowMenu->CreateWidget<MenuItem>("�ر�����").ClickEvent+=[this](){
            SetEveryWindowOpenState(false);
            ServiceLocator::Get<LogManager>().Log(LogLevel::info,"Close All PanelWindows");
        };
        m_WindowMenu->CreateWidget<MenuItem>("������").ClickEvent+=[this](){
            SetEveryWindowOpenState(true);
            ServiceLocator::Get<LogManager>().Log(LogLevel::info,"Open All PanelWindows");
        };
        //����һ���ָ���������Ĳ˵��ע���panelwindow��Ӧ�Ĳ˵���
        m_WindowMenu->CreateWidget<Separator>();
        m_WindowMenu->ClickEvent+=[this](){
            UpdateToggleableItems();
        };
    }

    void MenuBar::UpdateToggleableItems(){
        for(auto& [panelwindow,menuItem]:m_Panels|std::views::values){
            menuItem.get().isChecked=panelwindow.get().IsOpened();
        }
    }

    void MenuBar::SetEveryWindowOpenState(bool state){
        for(auto& [panelwindow,menuItem]:m_Panels|std::views::values){
            panelwindow.get().SetOpenState(state);
        }
    }
}