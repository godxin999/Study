module panel;

namespace Engine::inline UI{
    Panel::Panel(){
        m_PanelID=PanelCounter++;
    }
    void Panel::Draw(){
        if(IsEnabled){
            DrawImpl();
        }
    }
    uint64_t Panel::GetPanelID() const{
        return m_PanelID;
    }
}