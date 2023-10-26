#include "MeshRenderer.h"
#include <glad/gl.h>
#include "Material.h"
#include "MeshFilter.h"
#include "texture2d.h"
#include "Shader.h"

#include <ranges>

void MeshRenderer::SetMaterial(Material* material) {
	material_ = material;
}

void MeshRenderer::SetMeshFilter(MeshFilter* mesh_filter) {
	mesh_filter_ = mesh_filter;
}

void MeshRenderer::SetMVP(glm::mat4 mvp) {
	mvp_ = mvp;
}

void MeshRenderer::Render() {
	GLuint gl_program_id = material_->shader()->gl_program_id();
	if (vertex_array_object_ == 0) {
		//��ȡshader������ID
		GLint vpos_location = glGetAttribLocation(gl_program_id, "a_pos");
		GLint vcol_location = glGetAttribLocation(gl_program_id, "a_color");
		GLint a_uv_location = glGetAttribLocation(gl_program_id, "a_uv");

		//��������������
		glGenBuffers(1, &vertex_buffer_object_);
		//ָ��Ϊ���㻺��������
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_);
		//�ϴ��������ݵ�����������
		glBufferData(GL_ARRAY_BUFFER, mesh_filter_->mesh()->vertex_num * sizeof(MeshFilter::Vertex), mesh_filter_->mesh()->vertex_data, GL_STATIC_DRAW);

		//��������������
		glGenBuffers(1, &element_buffer_object_);
		//ָ��Ϊ��������������
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object_);
		//�ϴ��������ݵ�����������
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh_filter_->mesh()->vertex_index_num * sizeof(unsigned short), mesh_filter_->mesh()->vertex_index_data, GL_STATIC_DRAW);

		glGenVertexArrays(1, &vertex_array_object_);

		//����VAO
		glBindVertexArray(vertex_array_object_);
		{
			//ָ����ǰʹ�õ�VBO
			glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_);
			//��λ�������붥����ɫ������(a_pos)���й���
			glVertexAttribPointer(vpos_location, 3, GL_FLOAT, false, sizeof(MeshFilter::Vertex), 0);
			//����ɫ�����붥����ɫ������(a_color)���й���
			glVertexAttribPointer(vcol_location, 4, GL_FLOAT, false, sizeof(MeshFilter::Vertex), (void*)(sizeof(float) * 3));
			//���������������붥����ɫ������(a_uv)���й���
			glVertexAttribPointer(a_uv_location, 2, GL_FLOAT, false, sizeof(MeshFilter::Vertex), (void*)(sizeof(float) * (3 + 4)));

			glEnableVertexAttribArray(vpos_location);
			glEnableVertexAttribArray(vcol_location);
			glEnableVertexAttribArray(a_uv_location);

			//ָ����ǰʹ�õ�EBO
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object_);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);//�����������
	}
	//ָ��GPU����
	glUseProgram(gl_program_id);
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);//���������޳�

		//�ϴ�mvp����
		glUniformMatrix4fv(glGetUniformLocation(gl_program_id, "u_mvp"), 1, GL_FALSE, &mvp_[0][0]);

		auto textures = material_->textures();

		//��ͼ����
		for (auto&& [index, texture] : std::views::enumerate(textures)) {
			GLint u_texture_location = glGetUniformLocation(gl_program_id, texture.first.c_str());
			glActiveTexture(GL_TEXTURE0 + index);//��������Ԫ
			glBindTexture(GL_TEXTURE_2D, texture.second->gl_texture_id());//�����ص��������󶨵�����Ԫ
			glUniform1i(u_texture_location, index);//����Shader���������Ԫ�ж�������
		}

		glBindVertexArray(vertex_array_object_);
		{
			glDrawElements(GL_TRIANGLES, mesh_filter_->mesh()->vertex_index_num, GL_UNSIGNED_SHORT, 0);
		}
		glBindVertexArray(0);//�����������İ�
	}

}