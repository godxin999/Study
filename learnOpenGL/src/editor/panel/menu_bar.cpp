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
        //创建panelwindow对应的菜单项
        auto& menuItem=m_WindowMenu->CreateWidget<MenuItem>(name,"",true,true);
        //如果菜单项被点击，那么就会触发panelwindow的打开或关闭
        menuItem.ValueChangeEvent+=[&panel](bool value){
            panel.SetOpenState(value);
        };
        m_Panels.emplace(name,std::make_pair(std::ref(panel),std::ref(menuItem)));
    }
    
    void MenuBar::CreateFileMenu(){
        auto& fileMenu=CreateWidget<MenuList>("文件");
        fileMenu.CreateWidget<MenuItem>("新建","Ctrl + N");
        fileMenu.CreateWidget<MenuItem>("保存","Ctrl + S");
        fileMenu.CreateWidget<MenuItem>("另存为","Ctrl + Shift + S");
        fileMenu.CreateWidget<MenuItem>("退出","Alt + F4").ClickEvent+=[](){
            ServiceLocator::Get<WindowManager>().SetShouldClose(true);
            ServiceLocator::Get<LogManager>().Log(LogLevel::info,"Exit The Editor");
        };
    }

    void MenuBar::CreateWindowMenu(){
        m_WindowMenu=&CreateWidget<MenuList>("窗口");
        m_WindowMenu->CreateWidget<MenuItem>("关闭所有").ClickEvent+=[this](){
            SetEveryWindowOpenState(false);
            ServiceLocator::Get<LogManager>().Log(LogLevel::info,"Close All PanelWindows");
        };
        m_WindowMenu->CreateWidget<MenuItem>("打开所有").ClickEvent+=[this](){
            SetEveryWindowOpenState(true);
            ServiceLocator::Get<LogManager>().Log(LogLevel::info,"Open All PanelWindows");
        };
        //创建一个分隔符，下面的菜单项即注册的panelwindow对应的菜单项
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