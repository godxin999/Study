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

static VertexPosColor g_Vertices[8] = {
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
static WORD g_Indicies[36] = {
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

bool Cube::LoadContent() {
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
	UpdateBufferResource(commandList, &m_IndexBuffer, &intermediateIndexBuffer, _countof(g_Indicies), sizeof(WORD), g_Indicies);
	//����������������ͼ
	m_IndexBufferView.BufferLocation = m_IndexBuffer->GetGPUVirtualAddress();
	m_IndexBufferView.Format = DXGI_FORMAT_R16_UINT;//��ʾ�����������ڵ�ÿ��ֵ����16λ�޷�����
	m_IndexBufferView.SizeInBytes = sizeof(g_Indicies);
	//����DSV��������
	m_d3d12DSVDescriptorHeap = Application::Get().CreateDescriptorHeap(1, D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	//������ɫ��
	ComPtr<ID3DBlob> vertexShaderBlob;
	ThrowIfFailed(D3DReadFileToBlob(L"VertexShader.cso", &vertexShaderBlob));
	ComPtr<ID3DBlob> pixelShaderBlob;
	ThrowIfFailed(D3DReadFileToBlob(L"PixelShader.cso", &pixelShaderBlob));
	//�����������벼��
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},//ʹ��D3D12_APPEND_ALIGNED_ELEMENT���Զ�����Ԫ��
		{"COLOR",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0}
	};
	//��ѯ��ǩ�������֧�ְ汾
	D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData = {};
	featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;
	if (FAILED(device->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &featureData, sizeof(featureData)))) {//����Ƿ�֧��1.1�汾�ĸ�ǩ��
		featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
	}
	//�����ǩ����־
	D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;

	CD3DX12_ROOT_PARAMETER1 rootParameters[1];//����32λ������������������ɫ��ʹ��
	rootParameters[0].InitAsConstants(sizeof(XMMATRIX) / 4, 0, 0, D3D12_SHADER_VISIBILITY_VERTEX);//XMMATRIX����16��32λ��������ÿ��������ռ4�ֽڣ�����32λ����������Ϊ sizeof(XMMATRIX) / 4�����ı�־��ʾ��������ɫ���ɼ�
	//������ǩ������
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDescription;
	rootSignatureDescription.Init_1_1(_countof(rootParameters), rootParameters, 0, nullptr, rootSignatureFlags);
	//���л���ǩ��
	ComPtr<ID3DBlob> rootSignatureBlob;
	ComPtr<ID3DBlob> errorBlob;
	ThrowIfFailed(D3DX12SerializeVersionedRootSignature(&rootSignatureDescription, featureData.HighestVersion, &rootSignatureBlob, &errorBlob));
	//������ǩ��
	ThrowIfFailed(device->CreateRootSignature(0, rootSignatureBlob->GetBufferPointer(), rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(&m_RootSignature)));

	struct PipelineStateStream {
		CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE pRootSignature;
		CD3DX12_PIPELINE_STATE_STREAM_INPUT_LAYOUT InputLayout;
		CD3DX12_PIPELINE_STATE_STREAM_PRIMITIVE_TOPOLOGY PrimitiveTopologyType;
		CD3DX12_PIPELINE_STATE_STREAM_VS VS;
		CD3DX12_PIPELINE_STATE_STREAM_PS PS;
		CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL_FORMAT DSVFormat;
		CD3DX12_PIPELINE_STATE_STREAM_RENDER_TARGET_FORMATS RTVFormats;
	}pipelineStateStream;
	//������ȾĿ�������͸�ʽ
	D3D12_RT_FORMAT_ARRAY rtvFormats = {};
	rtvFormats.NumRenderTargets = 1;
	rtvFormats.RTFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	//����PSO����
	pipelineStateStream.pRootSignature = m_RootSignature.Get();
	pipelineStateStream.InputLayout = { inputLayout,_countof(inputLayout) };
	pipelineStateStream.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	pipelineStateStream.VS = CD3DX12_SHADER_BYTECODE(vertexShaderBlob.Get());
	pipelineStateStream.PS = CD3DX12_SHADER_BYTECODE(pixelShaderBlob.Get());
	pipelineStateStream.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	pipelineStateStream.RTVFormats = rtvFormats;
	//����PSO����
	D3D12_PIPELINE_STATE_STREAM_DESC pipelineStateStreamDesc = {
		sizeof(PipelineStateStream),&pipelineStateStream
	};
	ThrowIfFailed(device->CreatePipelineState(&pipelineStateStreamDesc, IID_PPV_ARGS(&m_PipelineState)));

	//�����������ִ�������б���ȷ������Ⱦ֮ǰ�������������Ͷ��㻺�����ϴ���GPU
	auto fenceValue = commandQueue->ExecuteCommandList(commandList);
	commandQueue->WaitForFenceValue(fenceValue);

	m_ContentLoaded = true;

	//����/������Ȼ�����
	ResizeDepthBuffer(GetClientWidth(), GetClientHeight());

	return true;
}

void Cube::UnloadContent() {
	m_ContentLoaded = false;
}

void Cube::OnUpdate(UpdateEventArgs& e) {
	//����֡�ʲ�������������
	static uint64_t frameCount = 0;
	static double elapsedTime = 0.0;

	Game::OnUpdate(e);

	elapsedTime += e.ElapsedTime;
	frameCount++;

	if (elapsedTime > 1.0) {
		wchar_t buffer[512];
		double fps = frameCount / elapsedTime;
		swprintf_s(buffer, 512, L"FPS: %f\n", fps);
		OutputDebugString(buffer);

		frameCount = 0;
		elapsedTime = 0.0;
	}

	//����ģ�;���
	float angle = static_cast<float>(e.TotalTime * 90.0);//��ת�Ƕ�
	const XMVECTOR rotationAxis = XMVectorSet(0, 1, 1, 0);//��ת��
	m_ModelMatrix = XMMatrixRotationAxis(rotationAxis, XMConvertToRadians(angle));//�����ת����
	//������ͼ����
	const XMVECTOR eyePosition = XMVectorSet(0, 0, -10, 1);
	const XMVECTOR focusPoint = XMVectorSet(0, 0, 0, 1);
	const XMVECTOR upDirection = XMVectorSet(0, 1, 0, 0);
	m_ViewMatrix = XMMatrixLookAtLH(eyePosition, focusPoint, upDirection);//������ϵ�»�ȡ��ͼ����
	//����ͶӰ����
	float aspectRatio = GetClientWidth() / static_cast<float>(GetClientHeight());
	m_ProjectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(m_Fov), aspectRatio, 0.1f, 100.0f);//������ϵ�»�ȡͶӰ����

}

void Cube::OnRender(RenderEventArgs& e) {
	Game::OnRender(e);

	auto commandQueue = Application::Get().GetCommandQueue(D3D12_COMMAND_LIST_TYPE_DIRECT);
	auto commandList = commandQueue->GetCommandList();

	UINT currentBackBufferIndex = m_pWindow->GetCurrentBackBufferIndex();

	auto backBuffer = m_pWindow->GetCurrentBackBuffer();
	auto rtv = m_pWindow->GetCurrentRenderTargetView();
	auto dsv = m_d3d12DSVDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

	//�����ȾĿ��
	{
		TransitionResource(commandList, backBuffer, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		FLOAT clearColor[] = { 0.4f,0.6f,0.9f,1.0f };

		ClearRTV(commandList, rtv, clearColor);
		ClearDepth(commandList, dsv);
	}
	//���ùܵ�״̬�͸�ǩ��
	commandList->SetPipelineState(m_PipelineState.Get());//��PSO����Ⱦ��ˮ�߰�
	commandList->SetGraphicsRootSignature(m_RootSignature.Get());//��ʹ��PSO�����Ĺ����о��Ѿ������˸�ǩ���������ڰ��κ���Դ֮ǰ������ʾ���ø�ǩ��

	//����IA
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);//����ͼԪ����
	commandList->IASetVertexBuffers(0, 1, &m_VertexBufferView);//�����㻺�����󶨵�����װ����
	commandList->IASetIndexBuffer(&m_IndexBufferView);//�������������󶨵�����װ����

	//����RS
	commandList->RSSetViewports(1, &m_Viewport);//�����ӿ�
	commandList->RSSetScissorRects(1, &m_ScissorRect);//���òü�����

	//����OM
	commandList->OMSetRenderTargets(1, &rtv, FALSE, &dsv);//����ȾĿ����ͼ�����ģ����ͼ�󶨵�����ϲ��׶�

	//���¸�����
	XMMATRIX mvpMatrix = XMMatrixMultiply(m_ModelMatrix, m_ViewMatrix);
	mvpMatrix = XMMatrixMultiply(mvpMatrix, m_ProjectionMatrix);
	commandList->SetGraphicsRoot32BitConstants(0, sizeof(XMMATRIX) / 4, &mvpMatrix, 0);//��ģ����ͼͶӰ���󴫵ݸ���ɫ��

	//Draw
	commandList->DrawIndexedInstanced(_countof(g_Indicies), 1, 0, 0, 0);

	//Present
	{
		TransitionResource(commandList, backBuffer, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		m_fenceValues[currentBackBufferIndex] = commandQueue->ExecuteCommandList(commandList);

		currentBackBufferIndex = m_pWindow->Present();

		commandQueue->WaitForFenceValue(m_fenceValues[currentBackBufferIndex]);
	}
}

void Cube::OnKeyPressed(KeyEventArgs& e) {
	Game::OnKeyPressed(e);

	switch (e.Key)
	{
	case KeyCode::Escape:
		Application::Get().Quit(0);
		break;
	case KeyCode::Enter:
		if (e.Alt) 
		{
	case KeyCode::F11:
		m_pWindow->ToggleFullScreen();
		break;
		}
	case KeyCode::V:
		m_pWindow->ToggleVSync();
		break;
	}
}

void Cube::OnMouseWheel(MouseWheelEventArgs& e) {
	m_Fov -= e.WheelDelta;
	m_Fov = std::clamp(m_Fov, 12.0f, 90.0f);

	wchar_t buffer[256];
	swprintf_s(buffer, 256, L"FoV: %f\n", m_Fov);
	OutputDebugString(buffer);
}

void Cube::TransitionResource(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> commandList, Microsoft::WRL::ComPtr<ID3D12Resource> resource, D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState) {
	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		resource.Get(),
		beforeState,
		afterState
	);
	commandList->ResourceBarrier(1, &barrier);//������Դ����
}

void Cube::ClearRTV(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> commandList, D3D12_CPU_DESCRIPTOR_HANDLE rtv, FLOAT* clearColor) {
	commandList->ClearRenderTargetView(rtv, clearColor, 0, nullptr);
}

void Cube::ClearDepth(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> commandList, D3D12_CPU_DESCRIPTOR_HANDLE dsv, FLOAT depth) {
	commandList->ClearDepthStencilView(dsv, D3D12_CLEAR_FLAG_DEPTH, depth, 0, 0, nullptr);
}

void Cube::UpdateBufferResource(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> commandList, ID3D12Resource** pDestinationResource, ID3D12Resource** pIntermediateResource, size_t numElements, size_t elementSize, const void* bufferData, D3D12_RESOURCE_FLAGS flags) {
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

void Cube::ResizeDepthBuffer(int width, int height) {

	if (m_ContentLoaded) {

		//Ϊ�˷�ֹ��Ȼ��������������������ִ�е������б����ã���Ҫ�Ƚ��������ˢ��
		Application::Get().Flush();

		width = std::max(1, width);
		height = std::max(1, height);

		auto device = Application::Get().GetDevice();

		//������Ȼ�����
		D3D12_CLEAR_VALUE optimizedClearValue = {};
		optimizedClearValue.Format = DXGI_FORMAT_D32_FLOAT;
		optimizedClearValue.DepthStencil = { 1.0f,0 };

		ThrowIfFailed(device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, width, height, 1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL),
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&optimizedClearValue,
			IID_PPV_ARGS(&m_DepthBuffer)
		));
		//�������/ģ����ͼ
		D3D12_DEPTH_STENCIL_VIEW_DESC dsv = {};
		dsv.Format = DXGI_FORMAT_D32_FLOAT;
		dsv.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsv.Texture2D.MipSlice = 0;
		dsv.Flags = D3D12_DSV_FLAG_NONE;

		device->CreateDepthStencilView(m_DepthBuffer.Get(), &dsv, m_d3d12DSVDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
	}
}

void Cube::OnResize(ResizeEventArgs& e) {
	if (e.Width != GetClientWidth() || e.Height != GetClientHeight()) {
		Game::OnResize(e);

		m_Viewport = CD3DX12_VIEWPORT(0.0f, 0.0f, static_cast<float>(e.Width), static_cast<float>(e.Height));//�����ӿ�

		ResizeDepthBuffer(e.Width, e.Height);
	}
}