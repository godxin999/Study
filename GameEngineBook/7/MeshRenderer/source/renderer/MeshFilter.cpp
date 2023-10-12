#include <fstream>
#include "MeshFilter.h"
#include "../utils/Application.h"

MeshFilter::MeshFilter() :mesh_(nullptr) {

}

void MeshFilter::LoadMesh(std::string mesh_file_path) {
	std::ifstream input_file_stream(Application::assets_path()+mesh_file_path, std::ios::in | std::ios::binary);
	MeshFileHeader mesh_file_header;
	input_file_stream.read((char*)&mesh_file_header, sizeof(MeshFileHeader));
	unsigned char* vertex_data= new unsigned char[mesh_file_header.vertex_num * sizeof(Vertex)];
	input_file_stream.read((char*)vertex_data, mesh_file_header.vertex_num * sizeof(Vertex));
	unsigned short* vertex_index_data = new unsigned short[mesh_file_header.vertex_index_num];
	input_file_stream.read((char*)vertex_index_data, mesh_file_header.vertex_index_num * sizeof(unsigned short));
	input_file_stream.close();

	mesh_ = new Mesh();
	mesh_->vertex_num= mesh_file_header.vertex_num;
	mesh_->vertex_index_num = mesh_file_header.vertex_index_num;
	mesh_->vertex_data = (Vertex*)vertex_data;
	mesh_->vertex_index_data = vertex_index_data;
}

MeshFilter::~MeshFilter() {
	delete mesh_;
	mesh_ = nullptr;
}
