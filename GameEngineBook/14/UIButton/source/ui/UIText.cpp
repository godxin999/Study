#include "UIText.h"
#include "component/GameObject.h"
#include "renderer/Font.h"
#include "renderer/Material.h"
#include "renderer/MeshFilter.h"
#include "renderer/MeshRenderer.h"
#include "renderer/texture2d.h"
#include "render_device/RenderDevice.h"
#include "utils/Debug.h"
#include <vector>
#include <ranges>
#include <rttr/registration>


RTTR_REGISTRATION{
	rttr::registration::class_<UIText>("UIText")
			.constructor<>()(rttr::policy::ctor::as_raw_ptr);
}

void UIText::SetText(std::string text) {
	if (text_ == text)return;
	text_ = text;
	dirty_ = true;
}

void UIText::Update() {
	if (!font_ || !font_->font_texture())return;

	MeshFilter* mesh_filter = dynamic_cast<MeshFilter*>(game_object()->GetComponent("MeshFilter"));
	if (!mesh_filter) {
		mesh_filter = dynamic_cast<MeshFilter*>(game_object()->AddComponent("MeshFilter"));

		auto material = new Material();
		material->Parse("material/ui_text.mat");

		auto mesh_renderer = dynamic_cast<MeshRenderer*>(game_object()->AddComponent("MeshRenderer"));
		mesh_renderer->SetMaterial(material);

		material->SetTexture("u_diffuse_texture", font_->font_texture());
	}

	if (dirty_) {
		std::vector<Font::Character*> character_vec = font_->LoadStr(text_);
		//遍历每个字符进行绘制
		std::vector<MeshFilter::Vertex> vertex_vector;
		std::vector<unsigned short> index_vector(character_vec.size() * 6);
		int x = 0;
		std::vector<unsigned short> triangle_index = { 0,1,2,0,2,3 };
		//合并Mesh
		for (auto&& [i, vec] : std::views::enumerate(character_vec)) {
			unsigned short width = (vec->right_bottom_x_ - vec->left_top_x_) * font_->font_texture()->width();
			unsigned short height = (vec->right_bottom_y_ - vec->left_top_y_) * font_->font_texture()->height();
			//因为FreeType生成的bitmap是上下颠倒的，所以这里UV坐标也要做对应翻转，将左上角作为零点
			vertex_vector.insert(vertex_vector.end(), {
				{{x,0.f,0.f},color_,{vec->left_top_x_,vec->right_bottom_y_}},
				{{x + width,0.f,0.f},color_,{vec->right_bottom_x_,vec->right_bottom_y_}},
				{{x + width,height,0.f},color_,{vec->right_bottom_x_,vec->left_top_y_}},
				{{x,height,0.f},color_,{vec->left_top_x_,vec->left_top_y_}}
				});
			x += width;
			for (int j = 0; j < triangle_index.size(); ++j) {
				index_vector[i * triangle_index.size() + j] = triangle_index[j] + i * 4;
			}
		}
		mesh_filter->CreateMesh(vertex_vector, index_vector);
	}
}

void UIText::OnPreRender() {
	Component::OnPreRender();
}

void UIText::OnPostRender() {
	Component::OnPostRender();
}
