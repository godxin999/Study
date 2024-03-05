export module tree_node;

import imgui;
import stl;
import event;
import data_widget;
import widget_container;
import pluginable;


namespace Engine::inline UI{
    export class TreeNode : public WidgetContainer, public DataWidget<std::u8string>{
    public:
        TreeNode()=default;
        TreeNode(const std::u8string& p_name=u8"",bool isClickArrowToOpen=false);
        void Close();
        void Open();
        [[nodiscard]]bool IsOpened()const;
    protected:
        void DrawImpl()override;
    public:
        std::u8string name{};
        bool isSelected{false};
        bool isLeafNode{false};

        Event<> ClickEvent{};
        Event<> DoubleClickEvent{};
        Event<> OpenEvent{};
        Event<> CloseEvent{};
    private:
        bool m_isClickArrowToOpen{false};
        bool m_isOpened{false};
        bool m_isShouldOpen{false};
        bool m_isShouldClose{false};
    };
}

module : private;

namespace Engine::inline UI{
    TreeNode::TreeNode(const std::u8string& p_name,bool isClickArrowToOpen):DataWidget(name),name(p_name),m_isClickArrowToOpen(isClickArrowToOpen){
        isAutoExecutePlugins=false;
    }

    void TreeNode::DrawImpl(){
        bool previousIsOpened=m_isOpened;
        
        if(m_isShouldOpen){
            ImGui::SetNextItemOpen(true);
            m_isShouldOpen=false;
        }
        else if(m_isShouldClose){
            ImGui::SetNextItemOpen(false);
            m_isShouldClose=false;
        }

        int flags=ImGuiTreeNodeFlags_None;
        //设置是否点击箭头展开
        if(m_isClickArrowToOpen) flags|=ImGuiTreeNodeFlags_OpenOnArrow;
        if(isLeafNode) flags|=ImGuiTreeNodeFlags_Leaf;
        if(isSelected) flags|=ImGuiTreeNodeFlags_Selected;

        bool opened=ImGui::TreeNodeEx(reinterpret_cast<const char*>((data+widgetID).c_str()),flags);
        //计算是否点击到标签
        if(ImGui::IsItemClicked()&&(ImGui::GetMousePos().x-ImGui::GetItemRectMin().x)>ImGui::GetTreeNodeToLabelSpacing()){
            //检测是否为左键
            if(ImGui::IsMouseDoubleClicked(0)){
                DoubleClickEvent.Invoke();
            }
            else{
                ClickEvent.Invoke();
            }
        }

        if(opened){
            if(!previousIsOpened){
                OpenEvent.Invoke();
            }
            m_isOpened=true;
            //手动执行插件
            ExecutePlugins();
            DrawWidgets();
            //关闭树节点
            ImGui::TreePop();
        }
        else{
            if(previousIsOpened){
                CloseEvent.Invoke();
            }
            m_isOpened=false;
            ExecutePlugins();
        }
    }

    void TreeNode::Close(){
        m_isShouldClose=true;
        m_isShouldOpen=false;
    }

    void TreeNode::Open(){
        m_isShouldOpen=true;
        m_isShouldClose=false;
    }

    bool TreeNode::IsOpened()const{
        return m_isOpened;
    }
}