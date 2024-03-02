module;
#include <imgui.h>
#include <imgui_internal.h>
export module imgui;

export ImGuiViewport;
export ImGuiID;
export ImVec2;
export ImGuiWindowFlags_;
export ImGuiStyleVar_;
export ImGuiDockNodeFlags_;
export ImFont;
export ImGuiTreeNodeFlags_;

export namespace ImGui{
    using ImGui::SameLine;
	using ImGui::NewFrame;
	using ImGui::GetMainViewport;
	using ImGui::SetNextWindowPos;
	using ImGui::SetNextWindowSize;
	using ImGui::SetNextWindowViewport;
	using ImGui::PushStyleVar;
	using ImGui::Begin;
	using ImGui::End;
	using ImGui::GetID;
	using ImGui::DockSpace;
	using ImGui::SetWindowPos;
	using ImGui::SetWindowSize;
	using ImGui::GetIO;
    using ImGui::PopStyleVar;
	using ImGui::Render;
    using ImGui::CreateContext;
    using ImGui::DestroyContext;
    using ImGui::GetStyle;
    using ImGui::StyleColorsClassic;
    using ImGui::StyleColorsDark;
    using ImGui::StyleColorsLight;
    using ImGui::LoadIniSettingsFromDisk;
    using ImGui::GetDrawData;
    using ImGui::GetWindowPos;
    using ImGui::GetWindowSize;
    using ImGui::BeginMainMenuBar;
    using ImGui::EndMainMenuBar;
    using ImGui::SetWindowFocus;
    using ImGui::Columns;
    using ImGui::NextColumn;
    using ImGui::SetColumnWidth;
    using ImGui::Dummy;
    using ImGui::NewLine;
    using ImGui::Spacing;
    using ImGui::CollapsingHeader;
    using ImGui::TreeNodeEx;
    using ImGui::SetNextItemOpen;
    using ImGui::IsItemClicked;
    using ImGui::GetMousePos;
    using ImGui::GetItemRectMin;
    using ImGui::GetTreeNodeToLabelSpacing;
    using ImGui::IsMouseDoubleClicked;
    using ImGui::TreePop;
    using ImGui::BeginMenu;
    using ImGui::EndMenu;
    using ImGui::MenuItem;
}