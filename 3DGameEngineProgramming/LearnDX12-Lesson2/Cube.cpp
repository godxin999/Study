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
 三角形的环绕顺序用于确定正面还是背面
 环绕顺序取决于投影裁剪空间中的三角形的法线方向
 三角形的法线基于三角形两条边的叉积
 DirectX中正面多边形的环绕顺序默认为顺时针(背面多边形的环绕顺序为逆时针) 
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
