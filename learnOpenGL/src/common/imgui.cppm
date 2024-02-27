module;
#include <imgui.h>
#include <imgui_internal.h>
export module imgui;

//export ImGuiViewport;
//export ImGuiID;
//export ImVec2;
//export ImGuiWindowFlags_;
//export ImGuiStyleVar_;
//export ImGuiDockNodeFlags_;

export namespace ImGui{
    using ImGui::SameLine;
	using ImGui::NewFrame;
	using ImGui::GetMainViewport;
	using ImGui::SetNextWindowPos;
	using ImGui::SetNextWindowSize;
	using ImGui::SetNextWindowViewport;
	using ImGui::PushStyleVar;
	using ImGui::Begin;
	using ImGui::GetID;
	using ImGui::DockSpace;
	using ImGui::SetWindowPos;
	using ImGui::GetIO;
	using ImGui::SetWindowSize;
	using ImGui::End;
    using ImGui::PopStyleVar;
	using ImGui::Render;

}