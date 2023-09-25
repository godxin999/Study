/*************************************************************************
	> File Name: ShaderSource.h
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/25 22:30:26
 ************************************************************************/
#pragma once

#ifndef DRAWTRIANGLE_SHADERSOURCE_H
#define DRAWTRIANGLE_SHADERSOURCE_H

static const char* vertex_shader_text =
"#version 110\n"
"uniform mat4 u_mvp;\n"
"attribute vec3 a_pos;\n"
"attribute vec4 a_color;\n"
"varying vec4 v_color;\n"
"void main(){\n"
"    gl_Position = u_mvp * vec4(a_pos,1.0);\n"
"    v_color = a_color;\n"
"}\n";

static const char* fragment_shader_text =
"#version 110\n"
"varying vec4 v_color;\n"
"void main(){\n"
"    gl_FragColor = v_color;\n"
"}\n";

#endif // !DRAWTRIANGLE_SHADERSOURCE_H