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
	std::ifstream input_file_stream(Application::assets_path()+mesh_file_path, std::ios::in | std::ios::binary);
	
	MeshFileHeader mesh_file_header;
	input_file_stream.read((char*)&mesh_file_header, sizeof(MeshFileHeader));
	
	mesh_ = new Mesh();
	mesh_->vertex_num= mesh_file_header.vertex_num;
	mesh_->vertex_index_num = mesh_file_header.vertex_index_num;
	
	mesh_->vertex_data = (Vertex*)malloc(mesh_file_header.vertex_num * sizeof(Vertex));
	input_file_stream.read((char*)mesh_->vertex_data, mesh_file_header.vertex_num * sizeof(Vertex));
	
	mesh_->vertex_index_data = (unsigned short*)malloc(mesh_file_header.vertex_index_num*sizeof(unsigned short));
	input_file_stream.read((char*)mesh_->vertex_index_data, mesh_file_header.vertex_index_num * sizeof(unsigned short));
	
	input_file_stream.close();
}

MeshFilter::~MeshFilter() {
	free(mesh_->vertex_data);
	free(mesh_->vertex_index_data);
	delete mesh_;
}
