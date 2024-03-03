export module menu_bar;

import stl;
import panel_window;
import panel_menu_bar;
import menu_list;
import menu_item;

namespace Engine::inline editor{
    export class MenuBar:public PanelMenuBar{
    public:
        using panelMap=std::unordered_map<std::string,std::pair<std::reference_wrapper<PanelWindow>,std::reference_wrapper<MenuItem>>>;
        MenuBar();
        ~MenuBar()=default;
        void HandleShortcuts(float deltaTime);
        void RegisterPanel(const std::string& name,PanelWindow& panel);
    private:
        void CreateFileMenu();
        void CreateWindowMenu();

        void UpdateToggleableItems();
        void SetEveryWindowOpenState(bool state);
    private:
        panelMap m_Panels{};
        MenuList* m_WindowMenu{nullptr};
    };
}