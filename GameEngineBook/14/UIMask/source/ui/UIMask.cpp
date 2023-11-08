#include "UIMask.h"
#include "render_device/RenderDevice.h"
#include "renderer/Material.h"
#include "renderer/MeshRenderer.h"
#include "renderer/MeshFilter.h"
#include "component/GameObject.h"
#include "renderer/texture2d.h"
#include <glad/glad.h>
#include <vector>
#include <rttr/registration>

RTTR_REGISTRATION
{
	rttr::registration::class_<UIMask>("UIMask")
			.constructor<>()(rttr::policy::ctor::as_raw_ptr);
}

void UIMask::OnEnable() {

}

void UIMask::Update() {
	Component::Update();
	if (!texture2d_)return;
	MeshFilter* mesh_filter = dynamic_cast<MeshFilter*>(game_object()->GetComponent("MeshFilter"));
	if (!mesh_filter) {
		std::vector<MeshFilter::Vertex> vertex_vector = {
			{ {0.f, 0.0f, 0.0f}, {1.0f,1.0f,1.0f,1.0f},   {0.f, 0.f} },
			{ {texture2d_->width(), 0.0f, 0.0f}, {1.0f,1.0f,1.0f,1.0f},   {1.f, 0.f} },
			{ {texture2d_->width(),  texture2d_->height(), 0.0f}, {1.0f,1.0f,1.0f,1.0f},   {1.f, 1.f} },
			{ {0.f,  texture2d_->height(), 0.0f}, {1.0f,1.0f,1.0f,1.0f},   {0.f, 1.f} }
		};
		std::vector<unsigned short> index_vector = {
				0,1,2,
				0,2,3
		};
		mesh_filter = dynamic_cast<MeshFilter*>(game_object()->AddComponent("MeshFilter"));
		mesh_filter->CreateMesh(vertex_vector, index_vector);

		auto material = new Material();
		material->Parse("material/ui_mask.mat");
		material->SetTexture("u_diffuse_texture", texture2d_);

		auto mesh_renderer=dynamic_cast<MeshRenderer*>(game_object()->AddComponent("MeshRenderer"));
		mesh_renderer->SetMaterial(material);
	}
}

void UIMask::OnPreRender() {
	Component::OnPreRender();
	RenderDevice::instance()->Enable(RenderDevice::RenderState::STENCIL_TEST);
	glClearStencil(0);//设置默认模板值为0
	glStencilFunc(GL_NEVER, 0x0, 0xFF);//设置模板测试函数，永远不通过
	glStencilOp(GL_INCR, GL_INCR, GL_INCR);
}

void UIMask::OnPostRender() {
	Component::OnPostRender();
}

void UIMask::OnDisable() {
	RenderDevice::instance()->Disable(RenderDevice::RenderState::STENCIL_TEST);
}