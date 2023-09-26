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
"#version 110\n"//����֧��OpenGL2.0�汾
"uniform mat4 u_mvp;\n"//uniform��������ͳһ����
"attribute vec3 a_pos;\n"//attribute�����������Ա�������ÿ��ִ��Shader����Ҫ����ֵ
"attribute vec4 a_color;\n"
"varying vec4 v_color;\n"//varying����������������������ڴӶ�����ɫ���������ݵ�Ƭ����ɫ��
"void main(){\n"//������ɫ���������겢���ݸ����ñ���gl_Position
"    gl_Position = u_mvp * vec4(a_pos,1.0);\n"
"    v_color = a_color;\n"
"}\n";
//���дӶ�����ɫ�������Ƭ����ɫ�������ݶ����ֵ
static const char* fragment_shader_text =
"#version 110\n"
"varying vec4 v_color;\n"//�Ӷ�����ɫ�����ݹ���������
"void main(){\n"//Ƭ����ɫ�����������ɫ
"    gl_FragColor = v_color;\n"
"}\n";

#endif // !DRAWTRIANGLE_SHADERSOURCE_H