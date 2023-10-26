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
		//获取shader的属性ID
		GLint vpos_location = glGetAttribLocation(gl_program_id, "a_pos");
		GLint vcol_location = glGetAttribLocation(gl_program_id, "a_color");
		GLint a_uv_location = glGetAttribLocation(gl_program_id, "a_uv");

		//创建缓冲区对象
		glGenBuffers(1, &vertex_buffer_object_);
		//指定为顶点缓冲区对象
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_);
		//上传顶点数据到缓冲区对象
		glBufferData(GL_ARRAY_BUFFER, mesh_filter_->mesh()->vertex_num * sizeof(MeshFilter::Vertex), mesh_filter_->mesh()->vertex_data, GL_STATIC_DRAW);

		//创建缓冲区对象
		glGenBuffers(1, &element_buffer_object_);
		//指定为索引缓冲区对象
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object_);
		//上传索引数据到缓冲区对象
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh_filter_->mesh()->vertex_index_num * sizeof(unsigned short), mesh_filter_->mesh()->vertex_index_data, GL_STATIC_DRAW);

		glGenVertexArrays(1, &vertex_array_object_);

		//设置VAO
		glBindVertexArray(vertex_array_object_);
		{
			//指定当前使用的VBO
			glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_);
			//将位置数据与顶点着色器属性(a_pos)进行关联
			glVertexAttribPointer(vpos_location, 3, GL_FLOAT, false, sizeof(MeshFilter::Vertex), 0);
			//将颜色数据与顶点着色器属性(a_color)进行关联
			glVertexAttribPointer(vcol_location, 4, GL_FLOAT, false, sizeof(MeshFilter::Vertex), (void*)(sizeof(float) * 3));
			//将纹理坐标数据与顶点着色器属性(a_uv)进行关联
			glVertexAttribPointer(a_uv_location, 2, GL_FLOAT, false, sizeof(MeshFilter::Vertex), (void*)(sizeof(float) * (3 + 4)));

			glEnableVertexAttribArray(vpos_location);
			glEnableVertexAttribArray(vcol_location);
			glEnableVertexAttribArray(a_uv_location);

			//指定当前使用的EBO
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object_);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);//解除缓冲区绑定
	}
	//指定GPU程序
	glUseProgram(gl_program_id);
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);//开启背面剔除

		//上传mvp矩阵
		glUniformMatrix4fv(glGetUniformLocation(gl_program_id, "u_mvp"), 1, GL_FALSE, &mvp_[0][0]);

		auto textures = material_->textures();

		//贴图设置
		for (auto&& [index, texture] : std::views::enumerate(textures)) {
			GLint u_texture_location = glGetUniformLocation(gl_program_id, texture.first.c_str());
			glActiveTexture(GL_TEXTURE0 + index);//激活纹理单元
			glBindTexture(GL_TEXTURE_2D, texture.second->gl_texture_id());//将加载的纹理句柄绑定到纹理单元
			glUniform1i(u_texture_location, index);//设置Shader程序从纹理单元中读出数据
		}

		glBindVertexArray(vertex_array_object_);
		{
			glDrawElements(GL_TRIANGLES, mesh_filter_->mesh()->vertex_index_num, GL_UNSIGNED_SHORT, 0);
		}
		glBindVertexArray(0);//解除顶点数组的绑定
	}

}