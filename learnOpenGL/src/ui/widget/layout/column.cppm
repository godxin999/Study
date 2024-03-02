export module column;

import group;
import imgui;
import stl;

namespace Engine::inline UI{
    export template <size_t N>
    class Column:public Group{
    public:
        Column();
        ~Column()=default;
    protected:
        void DrawImpl() override;
    public:
        std::array<float,N> widths;
    };
}

module : private;

namespace Engine::inline UI{
    template <size_t N>
    Column<N>::Column(){
        widths.fill(-1.f);
    }

    template <size_t N>
    void Column<N>::DrawImpl(){
        //列数量，名称，是否保留上次的设置
        ImGui::Columns(static_cast<int>(N),("##"+widgetID).c_str(),false);
        
        CollectGarbage();
        
        for(auto&& [index,widget] : m_Widgets|std::views::enumerate){
            widget.first->Draw();
            if(widths[index]!=-1.f){
                //设置列宽
                ImGui::SetColumnWidth(index,widths[index]);
            }
            //移动上下文到下一列
            ImGui::NextColumn();
        }
        //将列数量重置为1
        ImGui::Columns(1);
    }
}