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
	return 0;
}


