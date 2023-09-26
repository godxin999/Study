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
"#version 110\n"//至少支持OpenGL2.0版本
"uniform mat4 u_mvp;\n"//uniform用于修饰统一变量
"attribute vec3 a_pos;\n"//attribute用于修饰属性变量，其每次执行Shader都需要被赋值
"attribute vec4 a_color;\n"
"varying vec4 v_color;\n"//varying用于修饰输出变量，其用于从顶点着色器传递数据到片段着色器
"void main(){\n"//顶点着色器计算坐标并传递给内置变量gl_Position
"    gl_Position = u_mvp * vec4(a_pos,1.0);\n"
"    v_color = a_color;\n"
"}\n";
//所有从顶点着色器输出到片段着色器的数据都会插值
static const char* fragment_shader_text =
"#version 110\n"
"varying vec4 v_color;\n"//从顶点着色器传递过来的数据
"void main(){\n"//片段着色器用于输出颜色
"    gl_FragColor = v_color;\n"
"}\n";

#endif // !DRAWTRIANGLE_SHADERSOURCE_H