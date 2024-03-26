export module menu_list;

import group;
import stl;
import imgui;
import event;

namespace Engine::inline UI{
    export class MenuList:public Group{
    public:
        MenuList(const std::u8string& name,bool isLocked=false);
        ~MenuList()=default;
    protected:
        void DrawImpl()override;
    public:
        std::u8string name{};
        bool isLocked{false};
        Event<> ClickEvent{};
    private:
        bool m_isOpened{false};
    };
}

module : private;

namespace Engine::inline UI{
    MenuList::MenuList(const std::u8string& name,bool isLocked):name(name),isLocked(isLocked){

    }

    void MenuList::DrawImpl(){
        if(ImGui::BeginMenu(reinterpret_cast<const char*>(name.c_str()),!isLocked)){
            //成功打开菜单但是m_isOpened为false，说明发生了点击事件
            if(!m_isOpened){
                ClickEvent();
                m_isOpened=true;
            }
            DrawWidgets();
            ImGui::EndMenu();
        }
        else{
            m_isOpened=false;
        }
    }
}