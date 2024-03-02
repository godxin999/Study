export module tree_node;

import imgui;
import stl;
import event;
import data_widget;
import widget_container;
import pluginable;


namespace Engine::inline UI{
    export class TreeNode : public WidgetContainer, public DataWidget<std::string>{
    public:
        TreeNode()=default;
        TreeNode(const std::string& name="",bool isClickArrowToOpen=false);
        void Close();
        void Open();
        [[nodiscard]]bool IsOpened()const;
    protected:
        void DrawImpl()override;
    public:
        bool IsSelected{false};
        bool IsLeafNode{false};

        Event<> ClickEvent{};
        Event<> DoubleClickEvent{};
        Event<> OpenEvent{};
        Event<> CloseEvent{};
    private:
        bool m_IsClickArrowToOpen{false};
        bool m_IsOpened{false};
        bool m_IsShouldOpen{false};
        bool m_IsShouldClose{false};
    };
}

module : private;

namespace Engine::inline UI{
    TreeNode::TreeNode(const std::string& name,bool isClickArrowToOpen):DataWidget(name),m_IsClickArrowToOpen(isClickArrowToOpen){
        m_IsAutoExecutePlugins=false;
    }

    void TreeNode::DrawImpl(){
        bool previousIsOpened=m_IsOpened;
        
        if(m_IsShouldOpen){
            ImGui::SetNextItemOpen(true);
            m_IsShouldOpen=false;
        }
        else if(m_IsShouldClose){
            ImGui::SetNextItemOpen(false);
            m_IsShouldClose=false;
        }

        int flags=ImGuiTreeNodeFlags_None;
        //设置是否点击箭头展开
        if(m_IsClickArrowToOpen) flags|=ImGuiTreeNodeFlags_OpenOnArrow;
        if(IsLeafNode) flags|=ImGuiTreeNodeFlags_Leaf;
        if(IsSelected) flags|=ImGuiTreeNodeFlags_Selected;

        bool opened=ImGui::TreeNodeEx((data+m_WidgetID).c_str(),flags);
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
            m_IsOpened=true;
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
            m_IsOpened=false;
            ExecutePlugins();
        }
    }

    void TreeNode::Close(){
        m_IsShouldClose=true;
        m_IsShouldOpen=false;
    }

    void TreeNode::Open(){
        m_IsShouldOpen=true;
        m_IsShouldClose=false;
    }

    bool TreeNode::IsOpened()const{
        return m_IsOpened;
    }
}