#include <fstream>
#include <rttr/registration>
#include "MeshFilter.h"
#include "../utils/Application.h"

RTTR_REGISTRATION
{
	rttr::registration::class_<MeshFilter>("MeshFilter")
		.constructor<>()(rttr::policy::ctor::as_raw_ptr);
}


void MeshFilter::LoadMesh(std::string mesh_file_path) {
	std::ifstream input_file_stream(Application::assets_path() + mesh_file_path, std::ios::in | std::ios::binary);

	MeshFileHeader mesh_file_header;
	input_file_stream.read((char*)&mesh_file_header, sizeof(MeshFileHeader));

	mesh_ = new Mesh();
	mesh_->vertex_num_ = mesh_file_header.vertex_num_;
	mesh_->vertex_index_num_ = mesh_file_header.vertex_index_num_;

	mesh_->vertex_data_ = (Vertex*)malloc(mesh_file_header.vertex_num_ * sizeof(Vertex));
	input_file_stream.read((char*)mesh_->vertex_data_, mesh_file_header.vertex_num_ * sizeof(Vertex));

	mesh_->vertex_index_data_ = (unsigned short*)malloc(mesh_file_header.vertex_index_num_ * sizeof(unsigned short));
	input_file_stream.read((char*)mesh_->vertex_index_data_, mesh_file_header.vertex_index_num_ * sizeof(unsigned short));

	input_file_stream.close();
}

void MeshFilter::CreateMesh(std::vector<Vertex>& vertex_data, std::vector<unsigned short>& vertex_index_data) {
	if (mesh_) {
		delete mesh_;
	}
	mesh_ = new Mesh();
	mesh_->vertex_num_ = vertex_data.size();
	mesh_->vertex_index_num_ = vertex_index_data.size();

	mesh_->vertex_data_ = (Vertex*)malloc(mesh_->vertex_num_ * sizeof(Vertex));
	memcpy(mesh_->vertex_data_, &vertex_data[0], mesh_->vertex_num_ * sizeof(Vertex));

	mesh_->vertex_index_data_ = (unsigned short*)malloc(mesh_->vertex_index_num_ * sizeof(unsigned short));
	memcpy(mesh_->vertex_index_data_, &vertex_index_data[0], mesh_->vertex_index_num_ * sizeof(unsigned short));
}

MeshFilter::~MeshFilter() {
	delete mesh_;
}
