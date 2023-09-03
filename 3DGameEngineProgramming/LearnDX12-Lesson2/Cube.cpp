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


Cube::Cube(const std::wstring& name, int width, int height, bool vSync) :
	Game(name, width, height, vSync),
	m_ScissorRect(CD3DX12_RECT(0, 0, LONG_MAX, LONG_MAX)),//��ʼ��ΪLONG_MAX��ȷ���ü����θ�����������
	m_Viewport(CD3DX12_VIEWPORT(0.0f, 0.0f, static_cast<float> (width), static_cast<float>(height))),//�ӿھ����ڹ�դ���׶�ָ�������ڸ��߹�դ������ν������NDCת����������ɫ���������Ļ�ռ�
	m_Fov(45.0),//������Ĵ�ֱ�ӳ�����ʼΪ45��
	m_ContentLoaded(false) {

}

bool Cube::LoadContent(){
	auto device = Application::Get().GetDevice();
	auto commandQueue = Application::Get().GetCommandQueue(D3D12_COMMAND_LIST_TYPE_COPY);
	auto commandList = commandQueue->GetCommandList();

	//�ϴ����㻺����
	ComPtr<ID3D12Resource> intermediateVertexBuffer;//���ڽ�CPU���㻺������ת����GPU����ʱ���㻺����
	UpdateBufferResource(commandList, &m_VertexBuffer, &intermediateVertexBuffer, _countof(g_Vertices), sizeof(VertexPosColor), g_Vertices);
	//�������㻺������ͼ
	m_VertexBufferView.BufferLocation = m_VertexBuffer->GetGPUVirtualAddress();
	m_VertexBufferView.SizeInBytes = sizeof(g_Vertices);//���㻺�����Ĵ�С
	m_VertexBufferView.StrideInBytes = sizeof(VertexPosColor);//ÿ������Ĵ�С
	//�ϴ�����������
	ComPtr<ID3D12Resource> intermediateIndexBuffer;
	UpdateBufferResource(commandList, &m_IndexBuffer, &intermediateIndexBuffer, _countof(g_Indices), sizeof(WORD), g_Indices);
	//����������������ͼ
	m_IndexBufferView.BufferLocation = m_IndexBuffer->GetGPUVirtualAddress();
	m_IndexBufferView.Format = DXGI_FORMAT_R16_UINT;//��ʾ�����������ڵ�ÿ��ֵ����16λ�޷�����
	m_IndexBufferView.SizeInBytes = sizeof(g_Indices);
	//����DSV��������
	m_d3d12DSVDescriptorHeap = Application::Get().CreateDescriptorHeap(1, D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	//������ɫ��
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
		subresourceData.RowPitch = bufferSize;//��Դ���о�(��λΪ�ֽ�)
		subresourceData.SlicePitch = subresourceData.RowPitch;//��Դ�����(��λΪ�ֽ�)

		UpdateSubresources(commandList.Get(), *pDestinationResource, *pIntermediateResource, 0, 0, 1, &subresourceData);
	}
}
