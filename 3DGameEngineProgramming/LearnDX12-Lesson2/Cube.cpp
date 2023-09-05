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
三角形的环绕顺序用于确定正面还是背面
环绕顺序取决于投影裁剪空间中的三角形的法线方向
三角形的法线基于三角形两条边的叉积
DirectX中正面多边形的环绕顺序默认为顺时针(背面多边形的环绕顺序为逆时针)
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
	m_ScissorRect(CD3DX12_RECT(0, 0, LONG_MAX, LONG_MAX)),//初始化为LONG_MAX，确保裁剪矩形覆盖整个窗口
	m_Viewport(CD3DX12_VIEWPORT(0.0f, 0.0f, static_cast<float> (width), static_cast<float>(height))),//视口矩形在光栅化阶段指定，用于告诉光栅化器如何将顶点从NDC转换到像素着色器所需的屏幕空间
	m_Fov(45.0),//摄像机的垂直视场，初始为45°
	m_ContentLoaded(false) {

}

bool Cube::LoadContent() {
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
	UpdateBufferResource(commandList, &m_IndexBuffer, &intermediateIndexBuffer, _countof(g_Indicies), sizeof(WORD), g_Indicies);
	//创建索引缓冲区视图
	m_IndexBufferView.BufferLocation = m_IndexBuffer->GetGPUVirtualAddress();
	m_IndexBufferView.Format = DXGI_FORMAT_R16_UINT;//表示索引缓冲区内的每个值都是16位无符号数
	m_IndexBufferView.SizeInBytes = sizeof(g_Indicies);
	//创建DSV描述符堆
	m_d3d12DSVDescriptorHeap = Application::Get().CreateDescriptorHeap(1, D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	//加载着色器
	ComPtr<ID3DBlob> vertexShaderBlob;
	ThrowIfFailed(D3DReadFileToBlob(L"VertexShader.cso", &vertexShaderBlob));
	ComPtr<ID3DBlob> pixelShaderBlob;
	ThrowIfFailed(D3DReadFileToBlob(L"PixelShader.cso", &pixelShaderBlob));
	//创建顶点输入布局
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},//使用D3D12_APPEND_ALIGNED_ELEMENT宏自动对齐元素
		{"COLOR",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0}
	};
	//查询根签名的最高支持版本
	D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData = {};
	featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;
	if (FAILED(device->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &featureData, sizeof(featureData)))) {//检查是否支持1.1版本的根签名
		featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
	}
	//定义根签名标志
	D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;

	CD3DX12_ROOT_PARAMETER1 rootParameters[1];//单个32位常量根参数供顶点着色器使用
	rootParameters[0].InitAsConstants(sizeof(XMMATRIX) / 4, 0, 0, D3D12_SHADER_VISIBILITY_VERTEX);//XMMATRIX包含16个32位浮点数，每个浮点数占4字节，所以32位常量的数量为 sizeof(XMMATRIX) / 4，最后的标志表示仅顶点着色器可见
	//创建根签名描述
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDescription;
	rootSignatureDescription.Init_1_1(_countof(rootParameters), rootParameters, 0, nullptr, rootSignatureFlags);
	//序列化根签名
	ComPtr<ID3DBlob> rootSignatureBlob;
	ComPtr<ID3DBlob> errorBlob;
	ThrowIfFailed(D3DX12SerializeVersionedRootSignature(&rootSignatureDescription, featureData.HighestVersion, &rootSignatureBlob, &errorBlob));
	//创建根签名
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
	//定义渲染目标数量和格式
	D3D12_RT_FORMAT_ARRAY rtvFormats = {};
	rtvFormats.NumRenderTargets = 1;
	rtvFormats.RTFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	//描述PSO对象
	pipelineStateStream.pRootSignature = m_RootSignature.Get();
	pipelineStateStream.InputLayout = { inputLayout,_countof(inputLayout) };
	pipelineStateStream.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	pipelineStateStream.VS = CD3DX12_SHADER_BYTECODE(vertexShaderBlob.Get());
	pipelineStateStream.PS = CD3DX12_SHADER_BYTECODE(pixelShaderBlob.Get());
	pipelineStateStream.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	pipelineStateStream.RTVFormats = rtvFormats;
	//创建PSO对象
	D3D12_PIPELINE_STATE_STREAM_DESC pipelineStateStreamDesc = {
		sizeof(PipelineStateStream),&pipelineStateStream
	};
	ThrowIfFailed(device->CreatePipelineState(&pipelineStateStreamDesc, IID_PPV_ARGS(&m_PipelineState)));

	//在命令队列中执行命令列表，以确保在渲染之前将索引缓冲区和顶点缓冲区上传到GPU
	auto fenceValue = commandQueue->ExecuteCommandList(commandList);
	commandQueue->WaitForFenceValue(fenceValue);

	m_ContentLoaded = true;

	//创建/创建深度缓冲区
	ResizeDepthBuffer(GetClientWidth(), GetClientHeight());

	return true;
}

void Cube::UnloadContent() {
	m_ContentLoaded = false;
}

void Cube::OnUpdate(UpdateEventArgs& e) {
	//计算帧率并输出到调试输出
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

	//更新模型矩阵
	float angle = static_cast<float>(e.TotalTime * 90.0);//旋转角度
	const XMVECTOR rotationAxis = XMVectorSet(0, 1, 1, 0);//旋转轴
	m_ModelMatrix = XMMatrixRotationAxis(rotationAxis, XMConvertToRadians(angle));//获得旋转矩阵
	//更新视图矩阵
	const XMVECTOR eyePosition = XMVectorSet(0, 0, -10, 1);
	const XMVECTOR focusPoint = XMVectorSet(0, 0, 0, 1);
	const XMVECTOR upDirection = XMVectorSet(0, 1, 0, 0);
	m_ViewMatrix = XMMatrixLookAtLH(eyePosition, focusPoint, upDirection);//在左手系下获取视图矩阵
	//更新投影矩阵
	float aspectRatio = GetClientWidth() / static_cast<float>(GetClientHeight());
	m_ProjectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(m_Fov), aspectRatio, 0.1f, 100.0f);//在左手系下获取投影矩阵

}

void Cube::OnRender(RenderEventArgs& e) {
	Game::OnRender(e);

	auto commandQueue = Application::Get().GetCommandQueue(D3D12_COMMAND_LIST_TYPE_DIRECT);
	auto commandList = commandQueue->GetCommandList();

	UINT currentBackBufferIndex = m_pWindow->GetCurrentBackBufferIndex();

	auto backBuffer = m_pWindow->GetCurrentBackBuffer();
	auto rtv = m_pWindow->GetCurrentRenderTargetView();
	auto dsv = m_d3d12DSVDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

	//清除渲染目标
	{
		TransitionResource(commandList, backBuffer, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		FLOAT clearColor[] = { 0.4f,0.6f,0.9f,1.0f };

		ClearRTV(commandList, rtv, clearColor);
		ClearDepth(commandList, dsv);
	}
	//设置管道状态和根签名
	commandList->SetPipelineState(m_PipelineState.Get());//将PSO和渲染流水线绑定
	commandList->SetGraphicsRootSignature(m_RootSignature.Get());//即使在PSO创建的过程中就已经设置了根签名，但是在绑定任何资源之前必须显示设置根签名

	//设置IA
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);//设置图元类型
	commandList->IASetVertexBuffers(0, 1, &m_VertexBufferView);//将顶点缓冲区绑定到输入装配器
	commandList->IASetIndexBuffer(&m_IndexBufferView);//将索引缓冲区绑定到输入装配器

	//设置RS
	commandList->RSSetViewports(1, &m_Viewport);//设置视口
	commandList->RSSetScissorRects(1, &m_ScissorRect);//设置裁剪矩形

	//设置OM
	commandList->OMSetRenderTargets(1, &rtv, FALSE, &dsv);//将渲染目标视图和深度模板视图绑定到输出合并阶段

	//更新根参数
	XMMATRIX mvpMatrix = XMMatrixMultiply(m_ModelMatrix, m_ViewMatrix);
	mvpMatrix = XMMatrixMultiply(mvpMatrix, m_ProjectionMatrix);
	commandList->SetGraphicsRoot32BitConstants(0, sizeof(XMMATRIX) / 4, &mvpMatrix, 0);//将模型视图投影矩阵传递给着色器

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
	commandList->ResourceBarrier(1, &barrier);//插入资源屏障
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
		subresourceData.RowPitch = bufferSize;//资源的行距(单位为字节)
		subresourceData.SlicePitch = subresourceData.RowPitch;//资源的深度(单位为字节)

		UpdateSubresources(commandList.Get(), *pDestinationResource, *pIntermediateResource, 0, 0, 1, &subresourceData);
	}
}

void Cube::ResizeDepthBuffer(int width, int height) {

	if (m_ContentLoaded) {

		//为了防止深度缓冲区被命令队列上正在执行的命令列表引用，需要先将命令队列刷新
		Application::Get().Flush();

		width = std::max(1, width);
		height = std::max(1, height);

		auto device = Application::Get().GetDevice();

		//创建深度缓冲区
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
		//更新深度/模板视图
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

		m_Viewport = CD3DX12_VIEWPORT(0.0f, 0.0f, static_cast<float>(e.Width), static_cast<float>(e.Height));//更新视口

		ResizeDepthBuffer(e.Width, e.Height);
	}
}