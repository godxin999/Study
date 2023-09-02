#include "pch.h"
#include "Cube.h"
#include "Window.h"
#include "Application.h"
#include "CommandQueue.h"
#include "Helpers.h"

using namespace Microsoft::WRL;
using namespace DirectX;

struct VertexPosColor {
	XMFLOAT3 Position;
	XMFLOAT3 Color;
};

static VertexPosColor g_Vertices[8]={
	{XMFLOAT3{-1.0f,-1.0f,-1.0f},XMFLOAT3{0.0f,0.0f,0.0f}},
	{XMFLOAT3{-1.0f,1.0f,-1.0f},XMFLOAT3{0.0f,1.0f,0.0f}},
	{XMFLOAT3{1.0f,1.0f,-1.0f},XMFLOAT3{1.0f,1.0f,0.0f}},
	{XMFLOAT3{1.0f,-1.0f,-1.0f},XMFLOAT3{1.0f,0.0f,0.0f}},
	{XMFLOAT3{-1.0f,-1.0f,1.0f},XMFLOAT3{0.0f,0.0f,1.0f}},
	{XMFLOAT3{-1.0f,1.0f,1.0f},XMFLOAT3{0.0f,1.0f,1.0f}},
	{XMFLOAT3{1.0f,1.0f,1.0f},XMFLOAT3{1.0f,1.0f,1.0f}},
	{XMFLOAT3{1.0f,-1.0f,1.0f},XMFLOAT3{1.0f,0.0f,1.0f}}
};

 /*
 �����εĻ���˳������ȷ�����滹�Ǳ���
 ����˳��ȡ����ͶӰ�ü��ռ��е������εķ��߷���
 �����εķ��߻��������������ߵĲ��
 DirectX���������εĻ���˳��Ĭ��Ϊ˳ʱ��(�������εĻ���˳��Ϊ��ʱ��) 
 */
static WORD g_Indices[36] = {
	0,1,2,
	0,2,3,
	4,6,5,
	4,7,6,
	4,5,1,
	4,1,0,
	3,2,6,
	3,6,7,
	1,5,6,
	1,6,2,
	4,0,3,
	4,3,7
};
