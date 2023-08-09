#include "..\include\directx\d3dx12.h"
#include <sdkddkver.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tchar.h>
#include <wrl.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>
#include <d3d12.h>
#include <d3d12shader.h>
#include <d3dcompiler.h>
#if defined(_DEBUG)
#include <dxgidebug.h>
#endif
using namespace Microsoft;
using namespace Microsoft::WRL;
using namespace DirectX;

#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"d3dcompiler.lib")

#define WND_CLASS_NAME _T("Window Class")
#define WND_TITLE _T("Triangle")

#define THROW_IF_FAILED(hr) if(FAILED(hr)){throw COMException(hr,"Error",__FILE__,__LINE__);}

#include <exception>
#include <cstring>
#include <sstream>
#include <strsafe.h>

//异常类
class COMException :public std::exception {
public:
	COMException(HRESULT hr, const std::string& message = "", const std::string& file = "", int line = 0) :m_hrError(hr), m_message(message), m_file(file), m_line(line) {}
	HRESULT Error() const {
		return m_hrError;
	}
	virtual const char* what()const noexcept override {
		return m_message.c_str();
	}
	std::string File()const {
		return m_file;
	}
	int Line()const {
		return m_line;
	}
private:
	const HRESULT m_hrError;
	const std::string m_message;
	const std::string m_file;
	const int m_line;
};

//顶点结构体
struct VERTEX {
	XMFLOAT3 pos;
	XMFLOAT4 color;
};

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
	const UINT nFrameBackBufCount = 3u;
	int iWidth = 1024;
	int iHeight = 768;
	UINT nFrameIndex = 0;

	DXGI_FORMAT emRenderTargetFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	const float faClearColor[] = { 0.0f,0.2f,0.4f,1.0f };

	UINT nDXGIFactoryFlags = 0U;
	UINT nRTVDescriptorSize = 0U;

	HWND hWnd = nullptr;
	MSG msg = {};

	float fAspectRatio = 3.0f;

	D3D12_VERTEX_BUFFER_VIEW stVertexBufferView = {};

	UINT64 n64FenceValue = 0ui64;
	HANDLE hFenceEvent = nullptr;

	CD3DX12_VIEWPORT stViewPort(0.0f, 0.0f, static_cast<float>(iWidth), static_cast<float>(iHeight), D3D12_MIN_DEPTH, D3D12_MAX_DEPTH);
	CD3DX12_RECT stScissorRect(0, 0, static_cast<LONG>(iWidth), static_cast<LONG>(iHeight));

	ComPtr<IDXGIFactory5> pIDXGIFactory5;
	ComPtr<IDXGIAdapter1> pIAdapter;
	ComPtr<ID3D12Device4> pID3DDevice;
	ComPtr<ID3D12CommandQueue> pICommandQueue;
	ComPtr<IDXGISwapChain1> pISwapChain1;
	ComPtr<IDXGISwapChain3> pISwapChain3;
	ComPtr<ID3D12DescriptorHeap> pIRTVHeap;
	ComPtr<ID3D12Resource> pIARenderTarget[nFrameBackBufCount];
	ComPtr<ID3D12CommandAllocator> pICommandAllocator;
	ComPtr<ID3D12RootSignature> pIRootSignature;
	ComPtr<ID3D12PipelineState> pIPipelineState;
	ComPtr<ID3D12GraphicsCommandList> pICommandList;
	ComPtr<ID3D12Resource> pIVertexBuffer;
	ComPtr<ID3D12Fence> pIFence;


	try {
		//创建窗口
		{
			WNDCLASSEX wcex = {};

			wcex.cbSize = sizeof(WNDCLASSEX);
			wcex.style = CS_GLOBALCLASS;
			wcex.lpfnWndProc = WndProc;
			wcex.cbClsExtra = 0;
			wcex.cbWndExtra = 0;
			wcex.hInstance = hInstance;
			wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
			wcex.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
			wcex.lpszClassName = WND_CLASS_NAME;

			RegisterClassEx(&wcex);

			DWORD dwWndStyle = WS_OVERLAPPED | WS_SYSMENU;
			RECT rtWnd = { 0,0,iWidth,iHeight };
			AdjustWindowRect(&rtWnd, dwWndStyle, FALSE);

			//计算窗口居中的屏幕坐标
			INT posX = (GetSystemMetrics(SM_CXSCREEN) - rtWnd.right - rtWnd.left) / 2;
			INT posY = (GetSystemMetrics(SM_CYSCREEN) - rtWnd.bottom - rtWnd.top) / 2;

			hWnd = CreateWindowW(
				WND_CLASS_NAME,
				WND_TITLE,
				dwWndStyle,
				posX,
				posY,
				rtWnd.right - rtWnd.left,
				rtWnd.bottom - rtWnd.top,
				nullptr,
				nullptr,
				hInstance,
				nullptr
			);

			if (!hWnd) {
				return FALSE;
			}
			ShowWindow(hWnd, nCmdShow);
			UpdateWindow(hWnd);
		}
		//打开显示子系统的调试支持
		{
#if defined(_DEBUG)
			ComPtr<ID3D12Debug> debugController;
			if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
			{
				debugController->EnableDebugLayer();
				// 打开附加的调试支持
				nDXGIFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
			}
#endif
		}
		//创建DXGI Factory对象
		{
			CreateDXGIFactory2(nDXGIFactoryFlags, IID_PPV_ARGS(&pIDXGIFactory5));//创建工厂
			THROW_IF_FAILED(pIDXGIFactory5->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER));//关闭alt+enter切换全屏
		}
		//枚举适配器，并选择合适的适配器来创建3D设备对象
		{
			DXGI_ADAPTER_DESC1	desc = {};
			for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != pIDXGIFactory5->EnumAdapters1(adapterIndex, &pIAdapter); ++adapterIndex) {
				pIAdapter->GetDesc1(&desc);
				if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
					continue;//软件虚拟适配器，跳过
				}
				//检查适配器对D3D的支持级别，返回接口的参数被值为nullptr，这样就不会创建设备，只是检查支持级别
				if (SUCCEEDED(D3D12CreateDevice(pIAdapter.Get(), D3D_FEATURE_LEVEL_12_1, _uuidof(ID3D12Device), nullptr))) {
					break;
				}
			}
			//创建D3D12.1设备
			THROW_IF_FAILED(D3D12CreateDevice(pIAdapter.Get(), D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&pID3DDevice)));

			TCHAR pszWndTitle[MAX_PATH] = {};
			THROW_IF_FAILED(pIAdapter->GetDesc1(&desc));
			::GetWindowText(hWnd, pszWndTitle, MAX_PATH);
			StringCchPrintf(pszWndTitle
				, MAX_PATH
				, _T("%s (GPU:%s)")
				, pszWndTitle
				, desc.Description);
			::SetWindowText(hWnd, pszWndTitle);
		}
		//创建3D图形命令队列
		{
			D3D12_COMMAND_QUEUE_DESC queueDesc = {};
			queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
			THROW_IF_FAILED(pID3DDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&pICommandQueue)));
		}
		//创建交换链
		{
			DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
			swapChainDesc.BufferCount = nFrameBackBufCount;
			swapChainDesc.Width = iWidth;
			swapChainDesc.Height = iHeight;
			swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			swapChainDesc.SampleDesc.Count = 1;

			THROW_IF_FAILED(pIDXGIFactory5->CreateSwapChainForHwnd(
				pICommandQueue.Get(),
				hWnd,
				&swapChainDesc,
				nullptr,
				nullptr,
				&pISwapChain1
			));

			THROW_IF_FAILED(pISwapChain1.As(&pISwapChain3));//使用As方法将低版本的SwapChain接口转换为高版本的SwapChain接口
			nFrameIndex = pISwapChain3->GetCurrentBackBufferIndex();//调用高版本接口的GetCurrentBackBufferIndex方法获取当前后台缓冲区索引

			//创建描述符堆
			D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
			rtvHeapDesc.NumDescriptors = nFrameBackBufCount;
			rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

			THROW_IF_FAILED(pID3DDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&pIRTVHeap)));
			nRTVDescriptorSize = pID3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);//获取描述符大小

			//创建RTV描述符
			CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(pIRTVHeap->GetCPUDescriptorHandleForHeapStart());//管理描述符堆的当前元素指针位置
			for (UINT i = 0; i < nFrameBackBufCount; ++i) {
				THROW_IF_FAILED(pISwapChain3->GetBuffer(i, IID_PPV_ARGS(&pIARenderTarget[i])));
				pID3DDevice->CreateRenderTargetView(pIARenderTarget[i].Get(), nullptr, rtvHandle);
				rtvHandle.Offset(1, nRTVDescriptorSize);
			}
		}
		//创建根签名
		{
			CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
			rootSignatureDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
			ComPtr<ID3DBlob> pISignature;
			ComPtr<ID3DBlob> pIError;

			THROW_IF_FAILED(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pISignature, &pIError));
			THROW_IF_FAILED(pID3DDevice->CreateRootSignature(0, pISignature->GetBufferPointer(), pISignature->GetBufferSize(), IID_PPV_ARGS(&pIRootSignature)));
		}
		//创建命令列表分配器和图形命令列表
		{
			THROW_IF_FAILED(pID3DDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&pICommandAllocator)));
			THROW_IF_FAILED(pID3DDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, pICommandAllocator.Get(), pIPipelineState.Get(), IID_PPV_ARGS(&pICommandList)));
		}
		//编译Shader
		{
			ComPtr<ID3DBlob> pIVertexShader;
			ComPtr<ID3DBlob> pIPixelShader;
#if defined(_DEBUG)
			UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
			UINT compileFlags = 0;
#endif // defined(_DEBUG)
			TCHAR pszShaderFileName[] = _T("C:\\Users\\admin\\Desktop\\Study\\D3D12\\1-D3D12Triangle\\Shaders\\shaders.hlsl");
			THROW_IF_FAILED(D3DCompileFromFile(pszShaderFileName, nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &pIVertexShader, nullptr));
			THROW_IF_FAILED(D3DCompileFromFile(pszShaderFileName, nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &pIPixelShader, nullptr));

			D3D12_INPUT_ELEMENT_DESC inputElementDescs[] = {
				{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
				{ "COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,12,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 }
			};

			//创建渲染管线
			D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
			psoDesc.InputLayout = { inputElementDescs,_countof(inputElementDescs) };
			psoDesc.pRootSignature = pIRootSignature.Get();
			psoDesc.VS = CD3DX12_SHADER_BYTECODE(pIVertexShader.Get());
			psoDesc.PS = CD3DX12_SHADER_BYTECODE(pIPixelShader.Get());
			psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
			psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
			psoDesc.DepthStencilState.DepthEnable = FALSE;
			psoDesc.DepthStencilState.StencilEnable = FALSE;
			psoDesc.SampleMask = UINT_MAX;
			psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
			psoDesc.NumRenderTargets = 1;
			psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
			psoDesc.SampleDesc.Count = 1;

			THROW_IF_FAILED(pID3DDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pIPipelineState)));
		}
		//创建顶点缓冲
		{
			VERTEX triangleVertices[] =
			{
				{{0.0f,0.25f * fAspectRatio,0.0f},{1.0f,0.0f,0.0f,1.0f}},
				{{0.25f * fAspectRatio,-0.25f * fAspectRatio,0.0f},{0.0f,1.0f,0.0f,1.0f}},
				{{-0.25f * fAspectRatio,-0.25f * fAspectRatio,0.0f},{0.0f,0.0f,1.0f,1.0f}}
			};

			const UINT nVertexBufferSize = sizeof(triangleVertices);

			D3D12_HEAP_PROPERTIES stHeapProp = { D3D12_HEAP_TYPE_UPLOAD };

			D3D12_RESOURCE_DESC stResourceDesc = {};
			stResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
			stResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
			stResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
			stResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
			stResourceDesc.Width = nVertexBufferSize;
			stResourceDesc.Height = 1;
			stResourceDesc.DepthOrArraySize = 1;
			stResourceDesc.MipLevels = 1;
			stResourceDesc.SampleDesc.Count = 1;
			stResourceDesc.SampleDesc.Quality = 0;

			THROW_IF_FAILED(pID3DDevice->CreateCommittedResource(
				&stHeapProp,
				D3D12_HEAP_FLAG_NONE,
				&stResourceDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&pIVertexBuffer)));

			UINT8* pVertexDataBegin = nullptr;
			CD3DX12_RANGE readRange(0, 0);
			THROW_IF_FAILED(pIVertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin)));
			memcpy(pVertexDataBegin, triangleVertices, sizeof(triangleVertices));

			pIVertexBuffer->Unmap(0, nullptr);

			stVertexBufferView.BufferLocation = pIVertexBuffer->GetGPUVirtualAddress();
			stVertexBufferView.StrideInBytes = sizeof(VERTEX);
			stVertexBufferView.SizeInBytes = nVertexBufferSize;
		}
		//创建围栏对象用于同步
		{
			THROW_IF_FAILED(pID3DDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&pIFence)));
			n64FenceValue = 1;
			//创建一个Event同步对象，用于等待围栏事件通知
			hFenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
			if (hFenceEvent == nullptr) {
				THROW_IF_FAILED(HRESULT_FROM_WIN32(GetLastError()));
			}
		}
		//填充资源屏障
		D3D12_RESOURCE_BARRIER stBeginResourceBarrier = {};
		stBeginResourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		stBeginResourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		stBeginResourceBarrier.Transition.pResource = pIARenderTarget[nFrameIndex].Get();
		stBeginResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		stBeginResourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		stBeginResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

		D3D12_RESOURCE_BARRIER stEndResourceBarrier = {};
		stEndResourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		stEndResourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		stEndResourceBarrier.Transition.pResource = pIARenderTarget[nFrameIndex].Get();
		stEndResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		stEndResourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		stEndResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

		D3D12_CPU_DESCRIPTOR_HANDLE stRTVHandle = pIRTVHeap->GetCPUDescriptorHandleForHeapStart();
		DWORD dwRet = 0;
		BOOL bExit = FALSE;

		THROW_IF_FAILED(pICommandList->Close());
		SetEvent(hFenceEvent);
		//开启消息循环
		while (!bExit) {
			dwRet = ::MsgWaitForMultipleObjects(1, &hFenceEvent, FALSE, INFINITE, QS_ALLINPUT);
			switch (dwRet - WAIT_OBJECT_0) {
			case 0:
			{
				//获取新的后缓冲序号，因为Present整整完成时后缓冲的序号就更新了
				nFrameIndex = pISwapChain3->GetCurrentBackBufferIndex();
				THROW_IF_FAILED(pICommandAllocator->Reset());
				THROW_IF_FAILED(pICommandList->Reset(pICommandAllocator.Get(), pIPipelineState.Get()));
				//开始记录命令
				pICommandList->SetGraphicsRootSignature(pIRootSignature.Get());
				pICommandList->SetPipelineState(pIPipelineState.Get());
				pICommandList->RSSetViewports(1, &stViewPort);
				pICommandList->RSSetScissorRects(1, &stScissorRect);
				//通过资源屏障判定后缓冲已经切换完毕可以开始渲染了
				stBeginResourceBarrier.Transition.pResource = pIARenderTarget[nFrameIndex].Get();
				pICommandList->ResourceBarrier(1, &stBeginResourceBarrier);

				stRTVHandle = pIRTVHeap->GetCPUDescriptorHandleForHeapStart();
				stRTVHandle.ptr += nFrameIndex * nRTVDescriptorSize;
				//设置渲染目标
				pICommandList->OMSetRenderTargets(1, &stRTVHandle, FALSE, nullptr);
				//继续记录命令，并真正开始新一帧的渲染
				pICommandList->ClearRenderTargetView(stRTVHandle, faClearColor, 0, nullptr);
				pICommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
				pICommandList->IASetVertexBuffers(0, 1, &stVertexBufferView);
				//Draw Call
				pICommandList->DrawInstanced(3, 1, 0, 0);
				//资源屏障，用于确定渲染已经结束可以提交画面去显示了
				stEndResourceBarrier.Transition.pResource = pIARenderTarget[nFrameIndex].Get();
				pICommandList->ResourceBarrier(1, &stEndResourceBarrier);
				//关闭命令列表，可以开始执行了
				THROW_IF_FAILED(pICommandList->Close());
				//执行命令列表
				ID3D12CommandList* ppCommandLists[] = { pICommandList.Get() };
				pICommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
				//提交画面
				THROW_IF_FAILED(pISwapChain3->Present(1, 0));
				//开始同步GPU和CPU的执行，线记录围栏标记值
				const UINT64 n64CurrentFenceValue = n64FenceValue;
				THROW_IF_FAILED(pICommandQueue->Signal(pIFence.Get(), n64CurrentFenceValue));
				n64FenceValue++;
				THROW_IF_FAILED(pIFence->SetEventOnCompletion(n64CurrentFenceValue, hFenceEvent));
			}
			break;
			case 1:
			{//处理消息
				while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
					if (WM_QUIT != msg.message) {
						::TranslateMessage(&msg);
						::DispatchMessage(&msg);
					}
					else {
						bExit = TRUE;
					}
				}
			}
			break;
			case WAIT_TIMEOUT:
			{

			}
			break;
			default:
				break;
			}
		}

	}
	catch (const COMException& e) {
		std::basic_stringstream<TCHAR> ss;
		ss << _T("Error message") << std::endl;
		ss << _T("File: ") << (e.File().c_str()) << std::endl;
		ss << _T("Line: ") << e.Line();
		MessageBox(NULL, ss.str().c_str(), _T("Error"), MB_OK | MB_ICONERROR);
	}
	return 0;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}
