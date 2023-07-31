#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"D3D12.lib")
#pragma comment(lib,"dxgi.lib")
#include <iostream>
#include <comdef.h>
#include <windows.h>
#include <wrl.h>
#include <dxgi1_4.h>
#include <d3d12.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>
#include <string>
#include <memory>
#include <algorithm>
#include <vector>
#include <array>
#include <unordered_map>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <cassert>
using namespace std;
using namespace DirectX;
using Microsoft::WRL::ComPtr;

inline void ThrowIfFailed(HRESULT hr) {
	if (FAILED(hr)) {
		throw std::exception();
	}
}


int main() {
	//创建设备
#if defined(DEBUG) || defined(_DEBUG)//如果处于调试模式则启用调试层
	{
		ComPtr<ID3D12Debug> debugController;
		ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
		debugController->EnableDebugLayer();
	}
#endif;
	ComPtr<IDXGIFactory4> mdxgiFactory;//创建工厂
	ComPtr<ID3D12Device> md3dDevice;
	ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&mdxgiFactory)));
	HRESULT hardwareResult = D3D12CreateDevice(nullptr,//使用默认硬件适配器，创建设备
		D3D_FEATURE_LEVEL_11_0,//最少支持d3d11
		IID_PPV_ARGS(&md3dDevice));

	if (FAILED(hardwareResult)) {//如果创建硬件设备失败
		ComPtr<IDXGIAdapter> pWarpAdapter;//使用WARP适配器
		ThrowIfFailed(mdxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&pWarpAdapter)));
		ThrowIfFailed(D3D12CreateDevice(
			pWarpAdapter.Get(),
			D3D_FEATURE_LEVEL_11_0,
			IID_PPV_ARGS(&md3dDevice)
		));
	}
	//获取并打印适配器信息
	ComPtr<IDXGIAdapter> pA;
	for (UINT i = 0; mdxgiFactory->EnumAdapters(i, &pA) != DXGI_ERROR_NOT_FOUND; ++i) {
		DXGI_ADAPTER_DESC desc;
		pA->GetDesc(&desc);
		wcout << L"Adapter " << i << endl;
		wcout << L"Description: " << desc.Description << endl;
		wcout << L"Vendor ID: " << desc.VendorId << endl;
		wcout << L"Device ID: " << desc.DeviceId << endl;
	}
	//创建围栏并获取描述符的大小
	ComPtr<ID3D12Fence> mFence;
	ThrowIfFailed(md3dDevice->CreateFence(
		0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence)
	));
	//提前缓存描述符的大小，使用时直接引用
	auto mRtvDescriptorSize = md3dDevice->GetDescriptorHandleIncrementSize(
		D3D12_DESCRIPTOR_HEAP_TYPE_RTV
	);
	auto mDsvDescriptorSize = md3dDevice->GetDescriptorHandleIncrementSize(
		D3D12_DESCRIPTOR_HEAP_TYPE_DSV
	);
	auto mCbvUavDescriptorSize = md3dDevice->GetDescriptorHandleIncrementSize(
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
	);
	cout << mRtvDescriptorSize << " " << mDsvDescriptorSize << " " << mCbvUavDescriptorSize << endl;
	//检测对4x MSAA质量级别的支持
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;
	//填充结构体
	msQualityLevels.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	msQualityLevels.SampleCount = 4;
	msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	msQualityLevels.NumQualityLevels = 0;
	ThrowIfFailed(md3dDevice->CheckFeatureSupport(
		D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
		&msQualityLevels,
		sizeof(msQualityLevels)
	));
	auto m4xMsaaQuality = msQualityLevels.NumQualityLevels;
	cout << "m4xMsaaQuality: " << m4xMsaaQuality << endl;
	assert(m4xMsaaQuality > 0 && "Unexpected MSAA quality level.");
	//创建命令队列和命令列表
	ComPtr<ID3D12CommandQueue> mCmdQueue;
	ComPtr<ID3D12CommandAllocator> mCmdListAlloc;
	ComPtr<ID3D12GraphicsCommandList> mCmdList;
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};//描述队列类型和标志的结构体
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	ThrowIfFailed(md3dDevice->CreateCommandQueue(//创建命令队列
		&queueDesc, IID_PPV_ARGS(&mCmdQueue)
	));
	ThrowIfFailed(md3dDevice->CreateCommandAllocator(//创建命令分配器
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(mCmdListAlloc.GetAddressOf())
	));
	ThrowIfFailed(md3dDevice->CreateCommandList(//创建图形命令列表
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		mCmdListAlloc.Get(),//关联命令分配器
		nullptr,//初始化流水线状态对象
		IID_PPV_ARGS(mCmdList.GetAddressOf())
	));
	mCmdList->Close();//关闭命令列表，因为在初次引用命令列表时需要对其进行重置，而在调用重置方法之前又需要将其先关闭
	//描述并创建交换链
	ComPtr<IDXGISwapChain> mSwapChain;
	DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	mSwapChain.Reset();
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = 800;
	sd.BufferDesc.Height = 600;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = mBackBufferFormat;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = m4xMsaaQuality ? 4 : 1;
	sd.SampleDesc.Quality = m4xMsaaQuality ? (m4xMsaaQuality - 1) : 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 2;
	//sd.OutputWindow=whd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	ThrowIfFailed(mdxgiFactory->CreateSwapChain(
		mCmdQueue.Get(),
		&sd,
		mSwapChain.GetAddressOf()
	));
	
	return 0;
}


