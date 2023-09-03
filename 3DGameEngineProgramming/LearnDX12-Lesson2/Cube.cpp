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


Cube::Cube(const std::wstring& name, int width, int height, bool vSync) :
	Game(name, width, height, vSync),
	m_ScissorRect(CD3DX12_RECT(0, 0, LONG_MAX, LONG_MAX)),//初始化为LONG_MAX，确保裁剪矩形覆盖整个窗口
	m_Viewport(CD3DX12_VIEWPORT(0.0f, 0.0f, static_cast<float> (width), static_cast<float>(height))),//视口矩形在光栅化阶段指定，用于告诉光栅化器如何将顶点从NDC转换到像素着色器所需的屏幕空间
	m_Fov(45.0),//摄像机的垂直视场，初始为45°
	m_ContentLoaded(false) {

}

bool Cube::LoadContent(){
	auto device = Application::Get().GetDevice();
	auto commandQueue = Application::Get().GetCommandQueue(D3D12_COMMAND_LIST_TYPE_COPY);
	auto commandList = commandQueue->GetCommandList();

	//上传顶点缓冲区
	ComPtr<ID3D12Resource> intermediateVertexBuffer;//用于将CPU顶点缓冲数据转换到GPU的临时顶点缓冲区
	UpdateBufferResource(commandList, &m_VertexBuffer, &intermediateVertexBuffer, _countof(g_Vertices), sizeof(VertexPosColor), g_Vertices);
	//创建顶点缓冲区视图
	m_VertexBufferView.BufferLocation = m_VertexBuffer->GetGPUVirtualAddress();
	m_VertexBufferView.SizeInBytes = sizeof(g_Vertices);//顶点缓冲区的大小
	m_VertexBufferView.StrideInBytes = sizeof(VertexPosColor);//每个顶点的大小
	//上传索引缓冲区
	ComPtr<ID3D12Resource> intermediateIndexBuffer;
	UpdateBufferResource(commandList, &m_IndexBuffer, &intermediateIndexBuffer, _countof(g_Indices), sizeof(WORD), g_Indices);
	//创建索引缓冲区视图
	m_IndexBufferView.BufferLocation = m_IndexBuffer->GetGPUVirtualAddress();
	m_IndexBufferView.Format = DXGI_FORMAT_R16_UINT;//表示索引缓冲区内的每个值都是16位无符号数
	m_IndexBufferView.SizeInBytes = sizeof(g_Indices);
	//创建DSV描述符堆
	m_d3d12DSVDescriptorHeap = Application::Get().CreateDescriptorHeap(1, D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	//加载着色器
	ComPtr<ID3DBlob> vertexShaderBlob;
	ThrowIfFailed(D3DReadFileToBlob(L"VertexShader.cso", &vertexShaderBlob));
	ComPtr<ID3DBlob> pixelShaderBlob;
	ThrowIfFailed(D3DReadFileToBlob(L"PixelShader.cso", &pixelShaderBlob));



	return true;
}

void Cube::UpdateBufferResource(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> commandList, ID3D12Resource** pDestinationResource, ID3D12Resource** pIntermediateResource, size_t numElements, size_t elementSize, const void* bufferData, D3D12_RESOURCE_FLAGS flags){
	auto device = Application::Get().GetDevice();
	size_t bufferSize = numElements * elementSize;
	ThrowIfFailed(device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(bufferSize, flags),
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(pDestinationResource)
	));

	if (bufferData) {
		ThrowIfFailed(device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(bufferSize),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(pIntermediateResource)
		));
		D3D12_SUBRESOURCE_DATA subresourceData = {};
		subresourceData.pData = bufferData;
		subresourceData.RowPitch = bufferSize;//资源的行距(单位为字节)
		subresourceData.SlicePitch = subresourceData.RowPitch;//资源的深度(单位为字节)

		UpdateSubresources(commandList.Get(), *pDestinationResource, *pIntermediateResource, 0, 0, 1, &subresourceData);
	}
}
