#include "UIImage.h"
#include "..\component\GameObject.h"
#include "..\renderer\MeshRenderer.h"
#include "..\renderer\Material.h"
#include "..\renderer\texture2d.h"
#include "..\renderer\MeshFilter.h"
#include <rttr/registration>

RTTR_REGISTRATION
{
	rttr::registration::class_<UIImage>("UIImage")
			.constructor<>()(rttr::policy::ctor::as_raw_ptr);
}

void UIImage::Awake() {
	Component::Awake();
}

void UIImage::Update() {
	Component::Update();
	if (!texture2D_)return;
	MeshFilter* mesh_filter=dynamic_cast<MeshFilter*>(game_object()->GetComponent("MeshFilter"));
	if (!mesh_filter) {
		//����MeshFilter
		std::vector<MeshFilter::Vertex> vertex_vector = {
			{ {0.f, 0.0f, 0.0f}, {1.0f,1.0f,1.0f,1.0f},   {0.f, 0.f} },
			{ {texture2D_->width(), 0.0f, 0.0f}, {1.0f,1.0f,1.0f,1.0f},   {1.f, 0.f} },
			{ {texture2D_->width(),  texture2D_->height(), 0.0f}, {1.0f,1.0f,1.0f,1.0f},   {1.f, 1.f} },
			{ {0.f,  texture2D_->height(), 0.0f}, {1.0f,1.0f,1.0f,1.0f},   {0.f, 1.f} }
		};
		std::vector<unsigned short> index_vector = {
				0,1,2,
				0,2,3
		};
		auto mesh_filter = dynamic_cast<MeshFilter*>(game_object()->AddComponent("MeshFilter"));
		mesh_filter->CreateMesh(vertex_vector, index_vector);
		//����Material
		auto material = new Material();
		material->Parse("material/ui_image.mat");
		material->SetTexture("u_diffuse_texture", texture2D_);
		//����MeshRenderer
		auto mesh_renderer = dynamic_cast<MeshRenderer*>(game_object()->AddComponent("MeshRenderer"));
		mesh_renderer->SetMaterial(material);
	}
}