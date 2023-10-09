/*************************************************************************
	> File Name: ShaderSource.h
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/25 22:30:26
 ************************************************************************/
#pragma once

#ifndef SHADERSOURCE_H
#define SHADERSOURCE_H

static const char *vertex_shader_text =
	"#version 110\n"
	"uniform mat4 u_mvp;\n"
	"attribute vec3 a_pos;\n"
	"attribute vec4 a_color;\n"
	"attribute vec2 a_uv;\n"
	"varying vec4 v_color;\n"
	"varying vec2 v_uv;\n"
	"void main(){\n"
	"    gl_Position = u_mvp * vec4(a_pos,1.0);\n"
	"    v_color = a_color;\n"
	"	 v_uv = a_uv;\n"
	"}\n";

static const char *fragment_shader_text =
	"#version 110\n"
	"uniform sampler2D u_diffuse_texture;\n"
	"varying vec4 v_color;\n"
	"varying vec2 v_uv;\n"
	"void main(){\n"
	"    gl_FragColor = texture2D(u_diffuse_texture,v_uv);\n" // ����texture2D������������u_diffuse_texture�У���v_uvΪ���꣬ȡ����ɫֵ����ֵ��gl_FragColor
	"}\n";

#endif // !SHADERSOURCE_H