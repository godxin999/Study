//#include "..\include\directx\d3dx12.h"
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
//#include "..\WindowsCommons\d3dx12.h"
using namespace Microsoft;
using namespace Microsoft::WRL;
using namespace DirectX;

#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"d3dcompiler.lib")

#define WND_CLASS_NAME _T("Window Class")
#define WND_TITLE _T("Texture Cube")
#define GRS_THROW_IF_FAILED(hr) THROW_IF_FAILED(hr)
#define GRS_UPPER_DIV(A,B) UPPER_DIV(A,B)
#define GRS_UPPER(A,B) ALIGN_UP(A,B)
#define THROW_IF_FAILED(hr) if(FAILED(hr)){throw COMException(hr,"Error",__FILE__,__LINE__);}
//上取整除法
#define UPPER_DIV(A,B) ((UINT)(((A)+((B)-1))/(B)))
//向上边界对齐
#define ALIGN_UP(A, B) ((UINT)(((A) + ((B) - 1)) & ~((B) - 1)))


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

//WIC接口头文件
#include <wincodec.h>

//WIC格式和DXGI格式转换的结构体
struct WICTranslate {
	GUID                wic;
	DXGI_FORMAT         format;
};

static WICTranslate g_WICFormats[] = {
	//WIC格式与DXGI像素格式的对应表，该表中的格式为被支持的格式
	{ GUID_WICPixelFormat128bppRGBAFloat,       DXGI_FORMAT_R32G32B32A32_FLOAT },

	{ GUID_WICPixelFormat64bppRGBAHalf,         DXGI_FORMAT_R16G16B16A16_FLOAT },
	{ GUID_WICPixelFormat64bppRGBA,             DXGI_FORMAT_R16G16B16A16_UNORM },

	{ GUID_WICPixelFormat32bppRGBA,             DXGI_FORMAT_R8G8B8A8_UNORM },
	{ GUID_WICPixelFormat32bppBGRA,             DXGI_FORMAT_B8G8R8A8_UNORM }, // DXGI 1.1
	{ GUID_WICPixelFormat32bppBGR,              DXGI_FORMAT_B8G8R8X8_UNORM }, // DXGI 1.1

	{ GUID_WICPixelFormat32bppRGBA1010102XR,    DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM }, // DXGI 1.1
	{ GUID_WICPixelFormat32bppRGBA1010102,      DXGI_FORMAT_R10G10B10A2_UNORM },

	{ GUID_WICPixelFormat16bppBGRA5551,         DXGI_FORMAT_B5G5R5A1_UNORM },
	{ GUID_WICPixelFormat16bppBGR565,           DXGI_FORMAT_B5G6R5_UNORM },

	{ GUID_WICPixelFormat32bppGrayFloat,        DXGI_FORMAT_R32_FLOAT },
	{ GUID_WICPixelFormat16bppGrayHalf,         DXGI_FORMAT_R16_FLOAT },
	{ GUID_WICPixelFormat16bppGray,             DXGI_FORMAT_R16_UNORM },
	{ GUID_WICPixelFormat8bppGray,              DXGI_FORMAT_R8_UNORM },

	{ GUID_WICPixelFormat8bppAlpha,             DXGI_FORMAT_A8_UNORM },
};

// WIC 像素格式转换表.
struct WICConvert {
	GUID source;
	GUID target;
};

static WICConvert g_WICConvert[] = {
	//需要把前者转换为后者才可以转换为DXGI格式像素,目标格式一定是最接近的被支持的格式
	{ GUID_WICPixelFormatBlackWhite,            GUID_WICPixelFormat8bppGray }, // DXGI_FORMAT_R8_UNORM

	{ GUID_WICPixelFormat1bppIndexed,           GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM
	{ GUID_WICPixelFormat2bppIndexed,           GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM
	{ GUID_WICPixelFormat4bppIndexed,           GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM
	{ GUID_WICPixelFormat8bppIndexed,           GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM

	{ GUID_WICPixelFormat2bppGray,              GUID_WICPixelFormat8bppGray }, // DXGI_FORMAT_R8_UNORM
	{ GUID_WICPixelFormat4bppGray,              GUID_WICPixelFormat8bppGray }, // DXGI_FORMAT_R8_UNORM

	{ GUID_WICPixelFormat16bppGrayFixedPoint,   GUID_WICPixelFormat16bppGrayHalf }, // DXGI_FORMAT_R16_FLOAT
	{ GUID_WICPixelFormat32bppGrayFixedPoint,   GUID_WICPixelFormat32bppGrayFloat }, // DXGI_FORMAT_R32_FLOAT

	{ GUID_WICPixelFormat16bppBGR555,           GUID_WICPixelFormat16bppBGRA5551 }, // DXGI_FORMAT_B5G5R5A1_UNORM

	{ GUID_WICPixelFormat32bppBGR101010,        GUID_WICPixelFormat32bppRGBA1010102 }, // DXGI_FORMAT_R10G10B10A2_UNORM

	{ GUID_WICPixelFormat24bppBGR,              GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM
	{ GUID_WICPixelFormat24bppRGB,              GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM
	{ GUID_WICPixelFormat32bppPBGRA,            GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM
	{ GUID_WICPixelFormat32bppPRGBA,            GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM

	{ GUID_WICPixelFormat48bppRGB,              GUID_WICPixelFormat64bppRGBA }, // DXGI_FORMAT_R16G16B16A16_UNORM
	{ GUID_WICPixelFormat48bppBGR,              GUID_WICPixelFormat64bppRGBA }, // DXGI_FORMAT_R16G16B16A16_UNORM
	{ GUID_WICPixelFormat64bppBGRA,             GUID_WICPixelFormat64bppRGBA }, // DXGI_FORMAT_R16G16B16A16_UNORM
	{ GUID_WICPixelFormat64bppPRGBA,            GUID_WICPixelFormat64bppRGBA }, // DXGI_FORMAT_R16G16B16A16_UNORM
	{ GUID_WICPixelFormat64bppPBGRA,            GUID_WICPixelFormat64bppRGBA }, // DXGI_FORMAT_R16G16B16A16_UNORM

	{ GUID_WICPixelFormat48bppRGBFixedPoint,    GUID_WICPixelFormat64bppRGBAHalf }, // DXGI_FORMAT_R16G16B16A16_FLOAT
	{ GUID_WICPixelFormat48bppBGRFixedPoint,    GUID_WICPixelFormat64bppRGBAHalf }, // DXGI_FORMAT_R16G16B16A16_FLOAT
	{ GUID_WICPixelFormat64bppRGBAFixedPoint,   GUID_WICPixelFormat64bppRGBAHalf }, // DXGI_FORMAT_R16G16B16A16_FLOAT
	{ GUID_WICPixelFormat64bppBGRAFixedPoint,   GUID_WICPixelFormat64bppRGBAHalf }, // DXGI_FORMAT_R16G16B16A16_FLOAT
	{ GUID_WICPixelFormat64bppRGBFixedPoint,    GUID_WICPixelFormat64bppRGBAHalf }, // DXGI_FORMAT_R16G16B16A16_FLOAT
	{ GUID_WICPixelFormat48bppRGBHalf,          GUID_WICPixelFormat64bppRGBAHalf }, // DXGI_FORMAT_R16G16B16A16_FLOAT
	{ GUID_WICPixelFormat64bppRGBHalf,          GUID_WICPixelFormat64bppRGBAHalf }, // DXGI_FORMAT_R16G16B16A16_FLOAT

	{ GUID_WICPixelFormat128bppPRGBAFloat,      GUID_WICPixelFormat128bppRGBAFloat }, // DXGI_FORMAT_R32G32B32A32_FLOAT
	{ GUID_WICPixelFormat128bppRGBFloat,        GUID_WICPixelFormat128bppRGBAFloat }, // DXGI_FORMAT_R32G32B32A32_FLOAT
	{ GUID_WICPixelFormat128bppRGBAFixedPoint,  GUID_WICPixelFormat128bppRGBAFloat }, // DXGI_FORMAT_R32G32B32A32_FLOAT
	{ GUID_WICPixelFormat128bppRGBFixedPoint,   GUID_WICPixelFormat128bppRGBAFloat }, // DXGI_FORMAT_R32G32B32A32_FLOAT
	{ GUID_WICPixelFormat32bppRGBE,             GUID_WICPixelFormat128bppRGBAFloat }, // DXGI_FORMAT_R32G32B32A32_FLOAT

	{ GUID_WICPixelFormat32bppCMYK,             GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM
	{ GUID_WICPixelFormat64bppCMYK,             GUID_WICPixelFormat64bppRGBA }, // DXGI_FORMAT_R16G16B16A16_UNORM
	{ GUID_WICPixelFormat40bppCMYKAlpha,        GUID_WICPixelFormat64bppRGBA }, // DXGI_FORMAT_R16G16B16A16_UNORM
	{ GUID_WICPixelFormat80bppCMYKAlpha,        GUID_WICPixelFormat64bppRGBA }, // DXGI_FORMAT_R16G16B16A16_UNORM

	{ GUID_WICPixelFormat32bppRGB,              GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM
	{ GUID_WICPixelFormat64bppRGB,              GUID_WICPixelFormat64bppRGBA }, // DXGI_FORMAT_R16G16B16A16_UNORM
	{ GUID_WICPixelFormat64bppPRGBAHalf,        GUID_WICPixelFormat64bppRGBAHalf }, // DXGI_FORMAT_R16G16B16A16_FLOAT
};

//查表确定兼容的最近格式是哪个
bool GetTargetPixelFormat(const GUID* pSourceFormat, GUID* pTargetFormat) {
	*pTargetFormat = *pSourceFormat;
	for (size_t i = 0; i < _countof(g_WICConvert); ++i) {
		if (InlineIsEqualGUID(g_WICConvert[i].source, *pSourceFormat)) {
			*pTargetFormat = g_WICConvert[i].target;
			return true;
		}
	}
	return false;
}

//查表确定最终对应的DXGI格式是什么
DXGI_FORMAT GetDXGIFormatFromPixelFormat(const GUID* pPixelFormat) {
	for (size_t i = 0; i < _countof(g_WICFormats); ++i) {
		if (InlineIsEqualGUID(g_WICFormats[i].wic, *pPixelFormat)) {
			return g_WICFormats[i].format;
		}
	}
	return DXGI_FORMAT_UNKNOWN;
}

struct ST_VERTEX {
	XMFLOAT4 m_v4Position;//Position顶点位置
	XMFLOAT2 m_vTex;//Texcoord纹理坐标
	XMFLOAT3 m_vNor;//Normal法线
};

struct ST_FRAME_MVP_BUFFER {
	XMFLOAT4X4 m_MVP;//MVP矩阵
};

UINT g_nCurrentSamplerNo = 0; //当前使用的采样器索引
UINT g_nSampleMaxCnt = 5; //创建五个典型的采样器

//初始的默认摄像机的位置
XMVECTOR g_v4EyePos = XMVectorSet(0.0f, 2.0f, -15.0f, 0.0f);//眼睛位置
XMVECTOR g_v4LookAt = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);//看向的位置
XMVECTOR g_v4UpDir = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);//向上的方向

double g_fPalstance = 10.0f * XM_PI / 180.0f;//物体旋转的角速度，单位: 弧度/秒

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
	::CoInitialize(nullptr);  //for WIC & COM

	const UINT nFrameBackBufCount = 3u;//缓冲区数量

	int iWidth = 1024;//窗口宽度
	int iHeight = 768;//窗口高度

	UINT nFrameIndex = 0;//当前帧索引

	DXGI_FORMAT emRenderTargetFormat = DXGI_FORMAT_B8G8R8A8_UNORM;//渲染目标格式
	const float faClearColor[] = { 0.0f,0.0f,0.0f,0.0f };//背景色

	UINT nDXGIFactoryFlags = 0u;
	UINT nRTVDescriptorSize = 0u;//RTV描述符大小
	UINT nSRVDescriptorSize = 0u;//SRV描述符大小

	HWND hWnd = nullptr;//窗口句柄
	MSG msg = {};//消息
	TCHAR pszAppPath[MAX_PATH] = {};//应用程序路径

	float fBoxSize = 3.0f;
	float fTCMax = 3.0f;

	ST_FRAME_MVP_BUFFER* pMVPBuffer = nullptr;
	SIZE_T szMVPBuffer = ALIGN_UP(sizeof(ST_FRAME_MVP_BUFFER), 256);

	D3D12_VERTEX_BUFFER_VIEW stVertexBufferView = {};//顶点缓冲区视图，用于描述顶点缓冲区的位置和大小
	D3D12_INDEX_BUFFER_VIEW stIndexBufferView = {};//索引缓冲区视图，用于描述索引缓冲区的位置和大小

	UINT64 n64FenceValue = 0ui64;//围栏值
	HANDLE hFenceEvent = nullptr;//围栏事件句柄

	UINT nTextureW = 0u;//纹理宽度
	UINT nTextureH = 0u;//纹理高度
	UINT nBPP = 0u;//Bits Per Pixel
	UINT nPicRowPitch = 0;//图片行距(单位:字节)
	UINT64 n64UploadBufferSize = 0;//上传堆缓冲区大小
	DXGI_FORMAT stTextureFormat = DXGI_FORMAT_UNKNOWN;//纹理格式
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT stTxtLayouts = {};//纹理布局
	D3D12_RESOURCE_DESC stTextureDesc = {};//纹理信息描述结构体
	D3D12_RESOURCE_DESC stDestDesc = {};//目标资源描述结构体

	UINT nSamplerDescriptorSize = 0;//采样器描述符大小

	D3D12_VIEWPORT stViewPort = { 0.0f, 0.0f, static_cast<float>(iWidth), static_cast<float>(iHeight), D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };//视口属性结构体
	D3D12_RECT stScissorRect = { 0, 0, static_cast<LONG>(iWidth), static_cast<LONG>(iHeight) };//裁剪矩形结构体

	ComPtr<IDXGIFactory5> pIDXGIFactory5;
	ComPtr<IDXGIAdapter1> pIAdapter1;

	ComPtr<ID3D12Device4> pID3D12Device4;
	ComPtr<ID3D12CommandQueue> pICommandQueue;
	ComPtr<ID3D12CommandAllocator> pICommandAllocator;
	ComPtr<ID3D12GraphicsCommandList> pICommandList;

	ComPtr<IDXGISwapChain1> pISwapChain1;
	ComPtr<IDXGISwapChain3> pISwapChain3;
	ComPtr<ID3D12Resource> pIARenderTargets[nFrameBackBufCount];
	ComPtr<ID3D12DescriptorHeap> pIRTVHeap;

	ComPtr<ID3D12Heap> pITextureHeap;//纹理的默认堆
	ComPtr<ID3D12Heap> pIUploadHeap;//上传堆
	ComPtr<ID3D12Resource> pITexture;//纹理资源
	ComPtr<ID3D12Resource> pITextureUpload; //用于上传纹理数据的缓冲资源
	ComPtr<ID3D12Resource> pICBVUpload;
	ComPtr<ID3D12Resource> pIVertexBuffer;//顶点缓冲区
	ComPtr<ID3D12Resource> pIIndexBuffer;

	ComPtr<ID3D12DescriptorHeap> pISRVHeap;
	ComPtr<ID3D12DescriptorHeap> pISamplerDescriptorHeap;//采样器描述符堆

	ComPtr<ID3D12Fence> pIFence;
	ComPtr<ID3DBlob> pIBlobVertexShader;//顶点着色器
	ComPtr<ID3DBlob> pIBlobPixelShader;//像素着色器
	ComPtr<ID3D12RootSignature> pIRootSignature;
	ComPtr<ID3D12PipelineState> pIPipelineState;

	//WIC接口
	ComPtr<IWICImagingFactory> pIWICFactory;//WIC工厂接口
	ComPtr<IWICBitmapDecoder> pIWICDecoder;//WIC解码器接口
	ComPtr<IWICBitmapFrameDecode> pIWICFrame;//WIC位图帧解码接口
	ComPtr<IWICBitmapSource> pIBMP;//位图格式的图片数据对象接口

	try {
		// 得到当前的工作目录，方便我们使用相对路径来访问各种资源文件
		{
			if (0 == ::GetModuleFileName(nullptr, pszAppPath, MAX_PATH))
			{
				THROW_IF_FAILED(HRESULT_FROM_WIN32(GetLastError()));
			}
			WCHAR* lastSlash = _tcsrchr(pszAppPath, _T('\\'));
			if (lastSlash)
			{//删除exe文件名
				*(lastSlash) = _T('\0');
			}
			lastSlash = _tcsrchr(pszAppPath, _T('\\'));
			if (lastSlash)
			{//删除x64路径
				*(lastSlash) = _T('\0');
			}
			lastSlash = _tcsrchr(pszAppPath, _T('\\'));
			if (lastSlash)
			{//删除Debug 或 Release路径
				*(lastSlash) = _T('\0');
			}
			lastSlash = _tcsrchr(pszAppPath, _T('\\'));
			if (lastSlash)
			{//删除当前路径
				*(lastSlash + 1) = _T('\0');
			}
		}
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
			wcex.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);//防止无聊的背景重绘
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
		}
		//使用WIC创建并加载一个图片，并转换为DXGI兼容的格式
		{
			//使用纯COM方式创建WIC类厂对象，也是调用WIC第一步要做的事情
			THROW_IF_FAILED(CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pIWICFactory)));
			//使用WIC类厂对象接口加载纹理图片，并得到一个WIC解码器对象接口，图片信息就在这个接口代表的对象中了
			WCHAR pszTexcuteFileName[MAX_PATH] = {};
			StringCchPrintfW(pszTexcuteFileName, MAX_PATH, _T("%sAssets\\bear.jpg"), pszAppPath);

			THROW_IF_FAILED(pIWICFactory->CreateDecoderFromFilename(
				pszTexcuteFileName,//文件名
				nullptr,//不指定解码器，使用默认
				GENERIC_READ,//读权限
				WICDecodeMetadataCacheOnDemand,//如需要则缓冲数据
				&pIWICDecoder));//解码器对象

			//获取第一帧图片(因为GIF等格式文件可能会有多帧图片，其他的格式一般只有一帧图片)，实际解析出来的往往是位图格式数据
			THROW_IF_FAILED(pIWICDecoder->GetFrame(0, &pIWICFrame));

			WICPixelFormatGUID wpf = {};
			//获取WIC图片格式
			THROW_IF_FAILED(pIWICFrame->GetPixelFormat(&wpf));
			GUID tgFormat = {};

			//转换为DXGI的等价格式
			if (GetTargetPixelFormat(&wpf, &tgFormat)) {
				stTextureFormat = GetDXGIFormatFromPixelFormat(&tgFormat);
			}

			if (stTextureFormat == DXGI_FORMAT_UNKNOWN) {
				throw COMException(S_FALSE);
			}
			if (!InlineIsEqualGUID(wpf, tgFormat)) {
				//如果WIC图片格式与DXGI等价格式不一致，则需要进行格式转换
				//创建图片格式转换器
				ComPtr<IWICFormatConverter> pIConverter;
				THROW_IF_FAILED(pIWICFactory->CreateFormatConverter(&pIConverter));

				//初始化一个图片转换器，实际也就是将图片数据进行了格式转换
				THROW_IF_FAILED(pIConverter->Initialize(
					pIWICFrame.Get(),//源图片数据
					tgFormat,//目标图片格式
					WICBitmapDitherTypeNone,//指定位图是否有调色板，现代都是真彩位图，不用调色板，所以为None
					nullptr,//不使用调色板
					0.0,//指定alpha阈值
					WICBitmapPaletteTypeCustom//调色板类型，没有使用，指定为Custom
				));
				//调用QueryInterface方法获得对象的位图数据源接口
				THROW_IF_FAILED(pIConverter.As(&pIBMP));

			}
			else {
				//图片数据格式不需要转换，直接获取其位图数据源接口
				THROW_IF_FAILED(pIWICFrame.As(&pIBMP));
			}

			//获取图片大小
			THROW_IF_FAILED(pIBMP->GetSize(&nTextureW, &nTextureH));
			//获取图片像素的位大小的BPP(bits per pixel)信息，用于计算图片行数据的真实大小(单位:字节)
			ComPtr<IWICComponentInfo> pIWICmntinfo;
			THROW_IF_FAILED(pIWICFactory->CreateComponentInfo(tgFormat, pIWICmntinfo.GetAddressOf()));

			WICComponentType type;
			THROW_IF_FAILED(pIWICmntinfo->GetComponentType(&type));

			if (type != WICPixelFormat) {
				throw COMException(S_FALSE);
			}

			ComPtr<IWICPixelFormatInfo> pIWICPixelinfo;
			THROW_IF_FAILED(pIWICmntinfo.As(&pIWICPixelinfo));
			//获得BPP
			THROW_IF_FAILED(pIWICPixelinfo->GetBitsPerPixel(&nBPP));
			//计算图片实际的行大小(单位:字节)，使用一个上取整除法即（A+B-1）/B
			nPicRowPitch = UPPER_DIV(uint64_t(nTextureW) * uint64_t(nBPP), 8);
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
			THROW_IF_FAILED(CreateDXGIFactory2(nDXGIFactoryFlags, IID_PPV_ARGS(&pIDXGIFactory5)));//创建工厂
		}
		//枚举适配器，并选择合适的适配器来创建3D设备对象
		{
			//选择NUMA架构的独显来创建3D设备对象
			DXGI_ADAPTER_DESC1	desc = {};
			D3D12_FEATURE_DATA_ARCHITECTURE stArchitecture = {};
			for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != pIDXGIFactory5->EnumAdapters1(adapterIndex, &pIAdapter1); ++adapterIndex) {
				pIAdapter1->GetDesc1(&desc);
				if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
					continue;//软件虚拟适配器，跳过
				}
				THROW_IF_FAILED(D3D12CreateDevice(pIAdapter1.Get(), D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&pID3D12Device4)));
				THROW_IF_FAILED(pID3D12Device4->CheckFeatureSupport(D3D12_FEATURE_ARCHITECTURE, &stArchitecture, sizeof(D3D12_FEATURE_DATA_ARCHITECTURE)));
				if (!stArchitecture.UMA) {
					break;
				}
				pID3D12Device4.Reset();
			}
			//没有独显，抛异常退出
			if (pID3D12Device4.Get() == nullptr) {
				throw COMException(E_FAIL);
			}

			TCHAR pszWndTitle[MAX_PATH] = {};
			THROW_IF_FAILED(pIAdapter1->GetDesc1(&desc));
			::GetWindowText(hWnd, pszWndTitle, MAX_PATH);
			StringCchPrintf(pszWndTitle
				, MAX_PATH
				, _T("%s (GPU:%s)")
				, pszWndTitle
				, desc.Description);
			::SetWindowText(hWnd, pszWndTitle);

			//获取每个描述符元素的大小
			nRTVDescriptorSize = pID3D12Device4->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
			nSRVDescriptorSize = pID3D12Device4->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			nSamplerDescriptorSize = pID3D12Device4->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
		}
		//创建3D图形命令队列
		{
			D3D12_COMMAND_QUEUE_DESC queueDesc = {};
			queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
			THROW_IF_FAILED(pID3D12Device4->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&pICommandQueue)));
		}
		//创建命令列表分配器和图形命令列表
		{
			THROW_IF_FAILED(pID3D12Device4->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&pICommandAllocator)));
			THROW_IF_FAILED(pID3D12Device4->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, pICommandAllocator.Get(), nullptr, IID_PPV_ARGS(&pICommandList)));
		}
		//创建交换链
		{
			DXGI_SWAP_CHAIN_DESC1 stSwapChainDesc = {};
			stSwapChainDesc.BufferCount = nFrameBackBufCount;
			stSwapChainDesc.Width = iWidth;
			stSwapChainDesc.Height = iHeight;
			stSwapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			stSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			stSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			stSwapChainDesc.SampleDesc.Count = 1;

			THROW_IF_FAILED(pIDXGIFactory5->CreateSwapChainForHwnd(
				pICommandQueue.Get(),
				hWnd,
				&stSwapChainDesc,
				nullptr,
				nullptr,
				&pISwapChain1
			));
			//关闭alt+enter切换全屏
			THROW_IF_FAILED(pIDXGIFactory5->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER));

			THROW_IF_FAILED(pISwapChain1.As(&pISwapChain3));//使用As方法将低版本的SwapChain接口转换为高版本的SwapChain接口
			nFrameIndex = pISwapChain3->GetCurrentBackBufferIndex();//调用高版本接口的GetCurrentBackBufferIndex方法获取当前后台缓冲区索引
		}
		//创建RTV描述符堆及RTV描述符
		{
			D3D12_DESCRIPTOR_HEAP_DESC stRtvHeapDesc = {};
			stRtvHeapDesc.NumDescriptors = nFrameBackBufCount;
			stRtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			stRtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

			THROW_IF_FAILED(pID3D12Device4->CreateDescriptorHeap(&stRtvHeapDesc, IID_PPV_ARGS(&pIRTVHeap)));

			//创建RTV描述符
			D3D12_CPU_DESCRIPTOR_HANDLE stRTVHandle = pIRTVHeap->GetCPUDescriptorHandleForHeapStart();//管理描述符堆的当前元素指针位置
			for (UINT i = 0; i < nFrameBackBufCount; ++i) {
				THROW_IF_FAILED(pISwapChain3->GetBuffer(i, IID_PPV_ARGS(&pIARenderTargets[i])));
				pID3D12Device4->CreateRenderTargetView(pIARenderTargets[i].Get(), nullptr, stRTVHandle);
				stRTVHandle.ptr += nRTVDescriptorSize;
			}
		}
		//创建SRV、CBV、Sample描述符堆
		{
			//将纹理视图描述符和CBV描述符放在一个描述符堆上
			D3D12_DESCRIPTOR_HEAP_DESC stSRVHeapDesc = {};
			stSRVHeapDesc.NumDescriptors = 2;//1SRV + 1CBV
			stSRVHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			stSRVHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			THROW_IF_FAILED(pID3D12Device4->CreateDescriptorHeap(&stSRVHeapDesc, IID_PPV_ARGS(&pISRVHeap)));

			D3D12_DESCRIPTOR_HEAP_DESC stSamplerHeapDesc = {};
			stSamplerHeapDesc.NumDescriptors = g_nSampleMaxCnt;
			stSamplerHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
			stSamplerHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

			THROW_IF_FAILED(pID3D12Device4->CreateDescriptorHeap(&stSamplerHeapDesc, IID_PPV_ARGS(&pISamplerDescriptorHeap)));
		}
		//创建根签名
		{
			D3D12_FEATURE_DATA_ROOT_SIGNATURE stFeatureData = {};
			//检测是否支持V1.1的根签名
			stFeatureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;
			if (FAILED(pID3D12Device4->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &stFeatureData, sizeof(stFeatureData)))) {
				THROW_IF_FAILED(E_NOTIMPL);
			}
			//在GPU上执行SetGraphicsRootDescriptorTable后，我们不修改命令列表中的SRV，因此我们可以使用默认Range行为
			D3D12_DESCRIPTOR_RANGE1 stDSPRanges[3] = {};
			stDSPRanges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
			stDSPRanges[0].NumDescriptors = 1;
			stDSPRanges[0].BaseShaderRegister = 0;
			stDSPRanges[0].RegisterSpace = 0;
			stDSPRanges[0].Flags = D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC_WHILE_SET_AT_EXECUTE;
			stDSPRanges[0].OffsetInDescriptorsFromTableStart = 0;

			stDSPRanges[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
			stDSPRanges[1].NumDescriptors = 1;
			stDSPRanges[1].BaseShaderRegister = 0;
			stDSPRanges[1].RegisterSpace = 0;
			stDSPRanges[1].Flags = D3D12_DESCRIPTOR_RANGE_FLAG_NONE;
			stDSPRanges[1].OffsetInDescriptorsFromTableStart = 0;

			stDSPRanges[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
			stDSPRanges[2].NumDescriptors = 1;
			stDSPRanges[2].BaseShaderRegister = 0;
			stDSPRanges[2].RegisterSpace = 0;
			stDSPRanges[2].Flags = D3D12_DESCRIPTOR_RANGE_FLAG_NONE;
			stDSPRanges[2].OffsetInDescriptorsFromTableStart = 0;

			D3D12_ROOT_PARAMETER1 stRootParameters[3] = {};
			stRootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			stRootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//SRV仅PS可见
			stRootParameters[0].DescriptorTable.NumDescriptorRanges = 1;
			stRootParameters[0].DescriptorTable.pDescriptorRanges = &stDSPRanges[0];

			stRootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			stRootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//CBV所有Shader可见
			stRootParameters[1].DescriptorTable.NumDescriptorRanges = 1;
			stRootParameters[1].DescriptorTable.pDescriptorRanges = &stDSPRanges[1];

			stRootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			stRootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//Sample仅PS可见
			stRootParameters[2].DescriptorTable.NumDescriptorRanges = 1;
			stRootParameters[2].DescriptorTable.pDescriptorRanges = &stDSPRanges[2];

			D3D12_VERSIONED_ROOT_SIGNATURE_DESC stRootSignatureDesc = {};

			stRootSignatureDesc.Version = D3D_ROOT_SIGNATURE_VERSION_1_1;
			stRootSignatureDesc.Desc_1_1.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
			stRootSignatureDesc.Desc_1_1.NumParameters = _countof(stRootParameters);
			stRootSignatureDesc.Desc_1_1.pParameters = stRootParameters;
			stRootSignatureDesc.Desc_1_1.NumStaticSamplers = 0;
			stRootSignatureDesc.Desc_1_1.pStaticSamplers = nullptr;

			ComPtr<ID3DBlob> pISignatureBlob;
			ComPtr<ID3DBlob> pIErrorBlob;

			THROW_IF_FAILED(D3D12SerializeVersionedRootSignature(&stRootSignatureDesc, pISignatureBlob.GetAddressOf(), pIErrorBlob.GetAddressOf()));
			THROW_IF_FAILED(pID3D12Device4->CreateRootSignature(0, pISignatureBlob->GetBufferPointer(), pISignatureBlob->GetBufferSize(), IID_PPV_ARGS(&pIRootSignature)));
		}
		//编译Shader并创建渲染管线状态对象
		{
			UINT compileFlags = 0;
#if defined(_DEBUG)
			compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif // defined(_DEBUG)

			//编译为行矩阵形式
			compileFlags |= D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;

			TCHAR pszShaderFileName[MAX_PATH] = {};
			StringCchPrintf(pszShaderFileName, MAX_PATH, _T("%s4-D3D12TextureCube\\Shader\\TextureCube.hlsl"), pszAppPath);
			THROW_IF_FAILED(D3DCompileFromFile(pszShaderFileName, nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &pIBlobVertexShader, nullptr));
			THROW_IF_FAILED(D3DCompileFromFile(pszShaderFileName, nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &pIBlobPixelShader, nullptr));

			D3D12_INPUT_ELEMENT_DESC stInputElementDescs[] = {
				{ "POSITION",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,0,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
				{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,16,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
				{ "NORMAL",0,DXGI_FORMAT_R32G32_FLOAT,0,24,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 }
			};

			//创建渲染管线
			D3D12_GRAPHICS_PIPELINE_STATE_DESC stPSODesc = {};
			stPSODesc.InputLayout = { stInputElementDescs,_countof(stInputElementDescs) };

			stPSODesc.pRootSignature = pIRootSignature.Get();
			stPSODesc.VS.BytecodeLength = pIBlobVertexShader->GetBufferSize();
			stPSODesc.VS.pShaderBytecode = pIBlobVertexShader->GetBufferPointer();
			stPSODesc.PS.BytecodeLength = pIBlobPixelShader->GetBufferSize();
			stPSODesc.PS.pShaderBytecode = pIBlobPixelShader->GetBufferPointer();

			stPSODesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
			stPSODesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;

			stPSODesc.BlendState.AlphaToCoverageEnable = FALSE;
			stPSODesc.BlendState.IndependentBlendEnable = FALSE;
			stPSODesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

			stPSODesc.DepthStencilState.DepthEnable = FALSE;
			stPSODesc.DepthStencilState.StencilEnable = FALSE;
			stPSODesc.SampleMask = UINT_MAX;
			stPSODesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
			stPSODesc.NumRenderTargets = 1;
			stPSODesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
			stPSODesc.SampleDesc.Count = 1;

			THROW_IF_FAILED(pID3D12Device4->CreateGraphicsPipelineState(&stPSODesc, IID_PPV_ARGS(&pIPipelineState)));
		}
		//创建纹理的默认堆
		{
			D3D12_HEAP_DESC stTextureHeapDesc = {};
			//为堆指定纹理图片至少两倍大小的空间，实际应用中也是要综合考虑分配堆的大小以便可以重用堆
			stTextureHeapDesc.SizeInBytes = ALIGN_UP(2 * nPicRowPitch * nTextureH, D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT);
			//指定堆的对齐方式，这里使用了默认的64K边界对其，因为我们暂时不需要MSAA支持
			stTextureHeapDesc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
			stTextureHeapDesc.Properties.Type = D3D12_HEAP_TYPE_DEFAULT;
			stTextureHeapDesc.Properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			stTextureHeapDesc.Properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
			//拒绝渲染目标纹理、拒绝深度模板纹理，实际只是用来摆放普通纹理
			stTextureHeapDesc.Flags = D3D12_HEAP_FLAG_DENY_RT_DS_TEXTURES | D3D12_HEAP_FLAG_DENY_BUFFERS;

			THROW_IF_FAILED(pID3D12Device4->CreateHeap(&stTextureHeapDesc, IID_PPV_ARGS(&pITextureHeap)));
		}
		//创建2D纹理
		{
			//根据图片信息填充2D纹理资源的信息结构体
			stTextureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
			stTextureDesc.MipLevels = 1;
			stTextureDesc.Format = stTextureFormat;
			stTextureDesc.Width = nTextureW;
			stTextureDesc.Height = nTextureH;
			stTextureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
			stTextureDesc.DepthOrArraySize = 1;
			stTextureDesc.SampleDesc.Count = 1;
			stTextureDesc.SampleDesc.Quality = 0;

			//使用定位方式来创建纹理，调用内部不存在存储分配和释放的实际操作，性能较高
			THROW_IF_FAILED(pID3D12Device4->CreatePlacedResource(
				pITextureHeap.Get(),
				0,
				&stTextureDesc,
				D3D12_RESOURCE_STATE_COPY_DEST,
				nullptr,
				IID_PPV_ARGS(&pITexture)
			));

			//获取上传堆缓冲资源的大小，这个尺寸通常大于实际图片的尺寸
			n64UploadBufferSize = 0;
			D3D12_RESOURCE_DESC stCopyDstDesc = pITexture->GetDesc();
			pID3D12Device4->GetCopyableFootprints(&stCopyDstDesc, 0, 1, 0, nullptr, nullptr, nullptr, &n64UploadBufferSize);
		}
		//创建上传堆
		{
			D3D12_HEAP_DESC stUploadHeapDesc = {};
			//尺寸依然是实际纹理数据大小的2倍并64K边界对齐
			stUploadHeapDesc.SizeInBytes = ALIGN_UP(2 * n64UploadBufferSize, D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT);
			//上传堆肯定是Buffer类型，可以不指定对其方式，默认是64k边界对其
			stUploadHeapDesc.Alignment = 0;
			stUploadHeapDesc.Properties.Type = D3D12_HEAP_TYPE_UPLOAD;
			stUploadHeapDesc.Properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			stUploadHeapDesc.Properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
			//上传堆是缓冲，可以摆放任意数据
			stUploadHeapDesc.Flags = D3D12_HEAP_FLAG_ALLOW_ONLY_BUFFERS;
			THROW_IF_FAILED(pID3D12Device4->CreateHeap(&stUploadHeapDesc, IID_PPV_ARGS(&pIUploadHeap)));
		}
		//使用定位方式创建用于上传纹理数据的缓冲资源
		{
			D3D12_RESOURCE_DESC stUploadResDesc = {};
			stUploadResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
			stUploadResDesc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
			stUploadResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
			stUploadResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
			stUploadResDesc.Format = DXGI_FORMAT_UNKNOWN;
			stUploadResDesc.Width = n64UploadBufferSize;
			stUploadResDesc.Height = 1;
			stUploadResDesc.DepthOrArraySize = 1;
			stUploadResDesc.MipLevels = 1;
			stUploadResDesc.SampleDesc.Count = 1;
			stUploadResDesc.SampleDesc.Quality = 0;

			THROW_IF_FAILED(pID3D12Device4->CreatePlacedResource(
				pIUploadHeap.Get(),
				0,
				&stUploadResDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&pITextureUpload)
			));
		}
		//加载图片至上传堆，即完成第一个Copy动作，从memcpy可知这是由CPU完成的
		{

			//按照资源缓冲大小来分配实际图片数据存储的内存大小
			void* pbPicData = ::HeapAlloc(::GetProcessHeap(), HEAP_ZERO_MEMORY, n64UploadBufferSize);
			if (pbPicData == nullptr) {
				throw COMException(HRESULT_FROM_WIN32(GetLastError()));
			}
			//从图片读出数据
			THROW_IF_FAILED(pIBMP->CopyPixels(
				nullptr,
				nPicRowPitch,
				static_cast<UINT>(nPicRowPitch * nTextureH),//这里才是图片数据真实的大小，这个值通常小于缓冲的大小
				reinterpret_cast<BYTE*>(pbPicData)
			));

			//获取向上传堆拷贝纹理数据的一些纹理转换尺寸信息，对于复杂的DDS纹理这是非常必要的过程
			UINT nNumSubresources = 1u;
			UINT nTextureRowNum = 0u;
			UINT64 n64RequiredSize = 0u;
			UINT64 n64TextureRowSizes = 0u;

			stDestDesc = pITexture->GetDesc();

			pID3D12Device4->GetCopyableFootprints(&stDestDesc, 0, nNumSubresources, 0, &stTxtLayouts, &nTextureRowNum, &n64TextureRowSizes, &n64RequiredSize);

			//因为上传堆实际就是CPU传递数据到GPU的中介,所以我们可以使用熟悉的Map方法将它先映射到CPU内存地址中,然后我们按行将数据复制到上传堆中,需要注意的是之所以按行拷贝是因为GPU资源的行大小,与实际图片的行大小是有差异的,二者的内存边界对齐要求是不一样的
			BYTE* pData = nullptr;
			THROW_IF_FAILED(pITextureUpload->Map(0, nullptr, reinterpret_cast<void**>(&pData)));

			BYTE* pDestSlice = reinterpret_cast<BYTE*>(pData) + stTxtLayouts.Offset;
			const BYTE* pSrcSlice = reinterpret_cast<const BYTE*>(pbPicData);
			for (UINT y = 0; y < nTextureRowNum; ++y) {
				memcpy(pDestSlice + static_cast<SIZE_T>(stTxtLayouts.Footprint.RowPitch) * y, pSrcSlice + static_cast<SIZE_T>(nPicRowPitch) * y, nPicRowPitch);
			}

			pITextureUpload->Unmap(0, nullptr);
			//释放图片数据
			::HeapFree(::GetProcessHeap(), 0, pbPicData);
		}
		//向命令列表发出从上传堆复制纹理数据到默认堆的命令，执行并同步等待，即完成第二个Copy动作，由GPU上的复制引擎完成
		{
			//注意此时直接命令列表还没有绑定PSO对象，因此它也是不能执行3D图形命令的，但是可以执行复制命令，因为复制引擎不需要什么额外的状态设置之类的参数
			D3D12_TEXTURE_COPY_LOCATION stDstCopyLocation = {};
			stDstCopyLocation.pResource = pITexture.Get();
			stDstCopyLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
			stDstCopyLocation.SubresourceIndex = 0;

			D3D12_TEXTURE_COPY_LOCATION stSrcCopyLocation = {};
			stSrcCopyLocation.pResource = pITextureUpload.Get();
			stSrcCopyLocation.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
			stSrcCopyLocation.PlacedFootprint = stTxtLayouts;

			pICommandList->CopyTextureRegion(&stDstCopyLocation, 0, 0, 0, &stSrcCopyLocation, nullptr);

			//设置一个资源屏障，同步并确认复制操作完成
			D3D12_RESOURCE_BARRIER stResBar = {};
			stResBar.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			stResBar.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			stResBar.Transition.pResource = pITexture.Get();
			stResBar.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
			stResBar.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
			stResBar.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

			pICommandList->ResourceBarrier(1, &stResBar);

			//执行并等待纹理资源上传完成
			{
				THROW_IF_FAILED(pICommandList->Close());
				ID3D12CommandList* ppCommandLists[] = { pICommandList.Get() };
				pICommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
			}
			//创建围栏对象等待渲染完成
			{
				THROW_IF_FAILED(pID3D12Device4->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&pIFence)));
				n64FenceValue = 1;
				//创建一个Event同步对象，用于等待围栏事件通知
				hFenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
				if (hFenceEvent == nullptr) {
					THROW_IF_FAILED(HRESULT_FROM_WIN32(GetLastError()));
				}
			}
			//等待纹理资源复制完成
			{
				const UINT64 n64CurrentFenceValue = n64FenceValue;
				THROW_IF_FAILED(pICommandQueue->Signal(pIFence.Get(), n64CurrentFenceValue));
				n64FenceValue++;
				THROW_IF_FAILED(pIFence->SetEventOnCompletion(n64CurrentFenceValue, hFenceEvent));
			}
		}
		//定义正方体的3D数据结构
		ST_VERTEX stTriangleVertices[] = {
			{ {-1.0f * fBoxSize,  1.0f * fBoxSize, -1.0f * fBoxSize, 1.0f}, {0.0f * fTCMax, 0.0f * fTCMax}, {0.0f,  0.0f, -1.0f} },
			{ {1.0f * fBoxSize,  1.0f * fBoxSize, -1.0f * fBoxSize, 1.0f}, {1.0f * fTCMax, 0.0f * fTCMax},  {0.0f,  0.0f, -1.0f} },
			{ {-1.0f * fBoxSize, -1.0f * fBoxSize, -1.0f * fBoxSize, 1.0f}, {0.0f * fTCMax, 1.0f * fTCMax}, {0.0f,  0.0f, -1.0f} },
			{ {-1.0f * fBoxSize, -1.0f * fBoxSize, -1.0f * fBoxSize, 1.0f}, {0.0f * fTCMax, 1.0f * fTCMax}, {0.0f,  0.0f, -1.0f} },
			{ {1.0f * fBoxSize,  1.0f * fBoxSize, -1.0f * fBoxSize, 1.0f}, {1.0f * fTCMax, 0.0f * fTCMax},  {0.0f, 0.0f, -1.0f} },
			{ {1.0f * fBoxSize, -1.0f * fBoxSize, -1.0f * fBoxSize, 1.0f}, {1.0f * fTCMax, 1.0f * fTCMax},  {0.0f,  0.0f, -1.0f} },
			{ {1.0f * fBoxSize,  1.0f * fBoxSize, -1.0f * fBoxSize, 1.0f}, {0.0f * fTCMax, 0.0f * fTCMax},  {1.0f,  0.0f,  0.0f} },
			{ {1.0f * fBoxSize,  1.0f * fBoxSize,  1.0f * fBoxSize, 1.0f}, {1.0f * fTCMax, 0.0f * fTCMax},  {1.0f,  0.0f,  0.0f} },
			{ {1.0f * fBoxSize, -1.0f * fBoxSize, -1.0f * fBoxSize, 1.0f}, {0.0f * fTCMax, 1.0f * fTCMax},  {1.0f,  0.0f,  0.0f} },
			{ {1.0f * fBoxSize, -1.0f * fBoxSize, -1.0f * fBoxSize, 1.0f}, {0.0f * fTCMax, 1.0f * fTCMax},  {1.0f,  0.0f,  0.0f} },
			{ {1.0f * fBoxSize,  1.0f * fBoxSize,  1.0f * fBoxSize, 1.0f}, {1.0f * fTCMax, 0.0f * fTCMax},  {1.0f,  0.0f,  0.0f} },
			{ {1.0f * fBoxSize, -1.0f * fBoxSize,  1.0f * fBoxSize, 1.0f}, {1.0f * fTCMax, 1.0f * fTCMax},  {1.0f,  0.0f,  0.0f} },
			{ {1.0f * fBoxSize,  1.0f * fBoxSize,  1.0f * fBoxSize, 1.0f}, {0.0f * fTCMax, 0.0f * fTCMax},  {0.0f,  0.0f,  1.0f} },
			{ {-1.0f * fBoxSize,  1.0f * fBoxSize,  1.0f * fBoxSize, 1.0f}, {1.0f * fTCMax, 0.0f * fTCMax},  {0.0f,  0.0f,  1.0f} },
			{ {1.0f * fBoxSize, -1.0f * fBoxSize,  1.0f * fBoxSize, 1.0f}, {0.0f * fTCMax, 1.0f * fTCMax}, {0.0f,  0.0f,  1.0f} },
			{ {1.0f * fBoxSize, -1.0f * fBoxSize,  1.0f * fBoxSize, 1.0f}, {0.0f * fTCMax, 1.0f * fTCMax},  {0.0f,  0.0f,  1.0f} },
			{ {-1.0f * fBoxSize,  1.0f * fBoxSize,  1.0f * fBoxSize, 1.0f}, {1.0f * fTCMax, 0.0f * fTCMax},  {0.0f,  0.0f,  1.0f} },
			{ {-1.0f * fBoxSize, -1.0f * fBoxSize,  1.0f * fBoxSize, 1.0f}, {1.0f * fTCMax, 1.0f * fTCMax},  {0.0f,  0.0f,  1.0f} },
			{ {-1.0f * fBoxSize,  1.0f * fBoxSize,  1.0f * fBoxSize, 1.0f}, {0.0f * fTCMax, 0.0f * fTCMax}, {-1.0f,  0.0f,  0.0f} },
			{ {-1.0f * fBoxSize,  1.0f * fBoxSize, -1.0f * fBoxSize, 1.0f}, {1.0f * fTCMax, 0.0f * fTCMax}, {-1.0f,  0.0f,  0.0f} },
			{ {-1.0f * fBoxSize, -1.0f * fBoxSize,  1.0f * fBoxSize, 1.0f}, {0.0f * fTCMax, 1.0f * fTCMax}, {-1.0f,  0.0f,  0.0f} },
			{ {-1.0f * fBoxSize, -1.0f * fBoxSize,  1.0f * fBoxSize, 1.0f}, {0.0f * fTCMax, 1.0f * fTCMax}, {-1.0f,  0.0f,  0.0f} },
			{ {-1.0f * fBoxSize,  1.0f * fBoxSize, -1.0f * fBoxSize, 1.0f}, {1.0f * fTCMax, 0.0f * fTCMax}, {-1.0f,  0.0f,  0.0f} },
			{ {-1.0f * fBoxSize, -1.0f * fBoxSize, -1.0f * fBoxSize, 1.0f}, {1.0f * fTCMax, 1.0f * fTCMax}, {-1.0f,  0.0f,  0.0f} },
			{ {-1.0f * fBoxSize,  1.0f * fBoxSize,  1.0f * fBoxSize, 1.0f}, {0.0f * fTCMax, 0.0f * fTCMax},  {0.0f,  1.0f,  0.0f} },
			{ {1.0f * fBoxSize,  1.0f * fBoxSize,  1.0f * fBoxSize, 1.0f}, {1.0f * fTCMax, 0.0f * fTCMax},  {0.0f,  1.0f,  0.0f} },
			{ {-1.0f * fBoxSize,  1.0f * fBoxSize, -1.0f * fBoxSize, 1.0f}, {0.0f * fTCMax, 1.0f * fTCMax},  {0.0f,  1.0f,  0.0f} },
			{ {-1.0f * fBoxSize,  1.0f * fBoxSize, -1.0f * fBoxSize, 1.0f}, {0.0f * fTCMax, 1.0f * fTCMax},  {0.0f,  1.0f,  0.0f} },
			{ {1.0f * fBoxSize,  1.0f * fBoxSize,  1.0f * fBoxSize, 1.0f}, {1.0f * fTCMax, 0.0f * fTCMax},  {0.0f,  1.0f,  0.0f} },
			{ {1.0f * fBoxSize,  1.0f * fBoxSize, -1.0f * fBoxSize, 1.0f}, {1.0f * fTCMax, 1.0f * fTCMax},  {0.0f,  1.0f,  0.0f }},
			{ {-1.0f * fBoxSize, -1.0f * fBoxSize, -1.0f * fBoxSize, 1.0f}, {0.0f * fTCMax, 0.0f * fTCMax},  {0.0f, -1.0f,  0.0f} },
			{ {1.0f * fBoxSize, -1.0f * fBoxSize, -1.0f * fBoxSize, 1.0f}, {1.0f * fTCMax, 0.0f * fTCMax},  {0.0f, -1.0f,  0.0f} },
			{ {-1.0f * fBoxSize, -1.0f * fBoxSize,  1.0f * fBoxSize, 1.0f}, {0.0f * fTCMax, 1.0f * fTCMax},  {0.0f, -1.0f,  0.0f} },
			{ {-1.0f * fBoxSize, -1.0f * fBoxSize,  1.0f * fBoxSize, 1.0f}, {0.0f * fTCMax, 1.0f * fTCMax},  {0.0f, -1.0f,  0.0f} },
			{ {1.0f * fBoxSize, -1.0f * fBoxSize, -1.0f * fBoxSize, 1.0f}, {1.0f * fTCMax, 0.0f * fTCMax},  {0.0f, -1.0f,  0.0f} },
			{ {1.0f * fBoxSize, -1.0f * fBoxSize,  1.0f * fBoxSize, 1.0f}, {1.0f * fTCMax, 1.0f * fTCMax},  {0.0f, -1.0f,  0.0f} },
		};

		const UINT nVertexBufferSize = sizeof(stTriangleVertices);

		UINT32 pBoxIndices[] //立方体索引数组
			= {
			0,1,2,
			3,4,5,

			6,7,8,
			9,10,11,

			12,13,14,
			15,16,17,

			18,19,20,
			21,22,23,

			24,25,26,
			27,28,29,

			30,31,32,
			33,34,35,
		};

		const UINT nszIndexBuffer = sizeof(pBoxIndices);

		UINT64 n64BufferOffset = ALIGN_UP(n64UploadBufferSize, D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT);

		//使用定位方式创建顶点缓冲
		{
			//使用定位方式在相同的上传堆上以定位方式创建顶点缓冲，注意第二个参数指出了堆中的便宜位置
			D3D12_RESOURCE_DESC stVertexBufDesc = {};
			stVertexBufDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
			stVertexBufDesc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
			stVertexBufDesc.Width = nVertexBufferSize;
			stVertexBufDesc.Height = 1;
			stVertexBufDesc.DepthOrArraySize = 1;
			stVertexBufDesc.MipLevels = 1;
			stVertexBufDesc.Format = DXGI_FORMAT_UNKNOWN;
			stVertexBufDesc.SampleDesc.Count = 1;
			stVertexBufDesc.SampleDesc.Quality = 0;
			stVertexBufDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
			stVertexBufDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

			THROW_IF_FAILED(pID3D12Device4->CreatePlacedResource(
				pIUploadHeap.Get(),
				n64BufferOffset,
				&stVertexBufDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&pIVertexBuffer)));

			//将数据传至顶点缓冲对象
			UINT8* pVertexDataBegin = nullptr;
			D3D12_RANGE stReadRange = { 0, 0 };
			THROW_IF_FAILED(pIVertexBuffer->Map(0, &stReadRange, reinterpret_cast<void**>(&pVertexDataBegin)));
			memcpy(pVertexDataBegin, stTriangleVertices, sizeof(stTriangleVertices));

			pIVertexBuffer->Unmap(0, nullptr);

			//创建资源视图
			stVertexBufferView.BufferLocation = pIVertexBuffer->GetGPUVirtualAddress();
			stVertexBufferView.StrideInBytes = sizeof(ST_VERTEX);
			stVertexBufferView.SizeInBytes = nVertexBufferSize;

			//计算边界对齐的正确的偏移位置
			n64BufferOffset = ALIGN_UP(n64BufferOffset + nVertexBufferSize, D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT);

			D3D12_RESOURCE_DESC stIBResDesc = {};
			stIBResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
			stIBResDesc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
			stIBResDesc.Width = nszIndexBuffer;
			stIBResDesc.Height = 1;
			stIBResDesc.DepthOrArraySize = 1;
			stIBResDesc.MipLevels = 1;
			stIBResDesc.Format = DXGI_FORMAT_UNKNOWN;
			stIBResDesc.SampleDesc.Count = 1;
			stIBResDesc.SampleDesc.Quality = 0;
			stIBResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
			stIBResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

			THROW_IF_FAILED(pID3D12Device4->CreatePlacedResource(
				pIUploadHeap.Get(),
				n64BufferOffset,
				&stIBResDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&pIIndexBuffer)
			));

			UINT8* pIndexDataBegin = nullptr;
			THROW_IF_FAILED(pIIndexBuffer->Map(0, &stReadRange, reinterpret_cast<void**>(&pIndexDataBegin)));
			memcpy(pIndexDataBegin, pBoxIndices, nszIndexBuffer);
			pIIndexBuffer->Unmap(0, nullptr);

			stIndexBufferView.BufferLocation = pIIndexBuffer->GetGPUVirtualAddress();
			stIndexBufferView.Format = DXGI_FORMAT_R32_UINT;
			stIndexBufferView.SizeInBytes = nszIndexBuffer;
		}
		//在上传堆以定位方式创建常量缓冲
		{
			n64BufferOffset = ALIGN_UP(n64BufferOffset + nszIndexBuffer, D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT);

			D3D12_RESOURCE_DESC	stMVPResDesc = {};
			stMVPResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
			stMVPResDesc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
			stMVPResDesc.Width = szMVPBuffer;
			stMVPResDesc.Height = 1;
			stMVPResDesc.DepthOrArraySize = 1;
			stMVPResDesc.MipLevels = 1;
			stMVPResDesc.Format = DXGI_FORMAT_UNKNOWN;
			stMVPResDesc.SampleDesc.Count = 1;
			stMVPResDesc.SampleDesc.Quality = 0;
			stMVPResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
			stMVPResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
			//创建常量缓冲，注意缓冲尺寸设置为256边界对齐大小
			THROW_IF_FAILED(pID3D12Device4->CreatePlacedResource(
				pIUploadHeap.Get(),
				n64BufferOffset,
				&stMVPResDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&pICBVUpload)
			));
			//Map后就不再Unmap了，直接复制数据进去，这样每帧都不用map-copy-unmap浪费事件了
			THROW_IF_FAILED(pICBVUpload->Map(0, nullptr, reinterpret_cast<void**>(&pMVPBuffer)));
		}
		//创建SRV描述符
		{
			D3D12_SHADER_RESOURCE_VIEW_DESC stSRVDesc = {};
			stSRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			stSRVDesc.Format = stTextureDesc.Format;
			stSRVDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			stSRVDesc.Texture2D.MipLevels = 1;
			pID3D12Device4->CreateShaderResourceView(pITexture.Get(), &stSRVDesc, pISRVHeap->GetCPUDescriptorHandleForHeapStart());
		}
		//创建CBV描述符
		{
			D3D12_CONSTANT_BUFFER_VIEW_DESC stCBVDesc = {};
			stCBVDesc.BufferLocation = pICBVUpload->GetGPUVirtualAddress();
			stCBVDesc.SizeInBytes = static_cast<UINT>(szMVPBuffer);

			D3D12_CPU_DESCRIPTOR_HANDLE stCPUCBVHandle = pISRVHeap->GetCPUDescriptorHandleForHeapStart();
			stCPUCBVHandle.ptr += nSRVDescriptorSize;

			pID3D12Device4->CreateConstantBufferView(&stCBVDesc, stCPUCBVHandle);
		}
		//创建采样器堆和各种采样器
		{
			D3D12_CPU_DESCRIPTOR_HANDLE hSamplerHeap = { pISamplerDescriptorHeap->GetCPUDescriptorHandleForHeapStart() };

			D3D12_SAMPLER_DESC stSamplerDesc = {};
			stSamplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
			stSamplerDesc.MinLOD = 0;
			stSamplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
			stSamplerDesc.MipLODBias = 0.0f;
			stSamplerDesc.MaxAnisotropy = 1;
			stSamplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;

			//Sampler 1
			stSamplerDesc.BorderColor[0] = 255.0f;
			stSamplerDesc.BorderColor[1] = 255.0f;
			stSamplerDesc.BorderColor[2] = 255.0f;
			stSamplerDesc.BorderColor[3] = 255.0f;
			stSamplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
			stSamplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
			stSamplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
			pID3D12Device4->CreateSampler(&stSamplerDesc, hSamplerHeap);

			hSamplerHeap.ptr += nSamplerDescriptorSize;

			//Sampler 2
			stSamplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			stSamplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			stSamplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			pID3D12Device4->CreateSampler(&stSamplerDesc, hSamplerHeap);

			hSamplerHeap.ptr += nSamplerDescriptorSize;

			//Sampler 3
			stSamplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			stSamplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			stSamplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			pID3D12Device4->CreateSampler(&stSamplerDesc, hSamplerHeap);

			hSamplerHeap.ptr += nSamplerDescriptorSize;

			//Sampler 4
			stSamplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
			stSamplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
			stSamplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
			pID3D12Device4->CreateSampler(&stSamplerDesc, hSamplerHeap);

			hSamplerHeap.ptr += nSamplerDescriptorSize;

			//Sampler 5
			stSamplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_MIRROR_ONCE;
			stSamplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_MIRROR_ONCE;
			stSamplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_MIRROR_ONCE;
			pID3D12Device4->CreateSampler(&stSamplerDesc, hSamplerHeap);

		}

		//填充资源屏障结构
		D3D12_RESOURCE_BARRIER stBeginResBarrier = {};
		stBeginResBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		stBeginResBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		stBeginResBarrier.Transition.pResource = pIARenderTargets[nFrameIndex].Get();
		stBeginResBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		stBeginResBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		stBeginResBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

		D3D12_RESOURCE_BARRIER stEndResBarrier = {};
		stEndResBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		stEndResBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		stEndResBarrier.Transition.pResource = pIARenderTargets[nFrameIndex].Get();
		stEndResBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		stEndResBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		stEndResBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

		//记录帧开始时间和当前时间，以循环结束为界
		ULONGLONG n64tmFrameStart = ::GetTickCount64();
		ULONGLONG n64timeCurrent = n64tmFrameStart;
		//计算旋转角度需要的变量
		double dModelRotationYAngle = 0.0f;

		DWORD dwRet = 0;
		BOOL bExit = FALSE;

		ShowWindow(hWnd, nCmdShow);
		UpdateWindow(hWnd);
		//开启消息循环
		while (!bExit) {
			dwRet = ::MsgWaitForMultipleObjects(1, &hFenceEvent, FALSE, INFINITE, QS_ALLINPUT);
			switch (dwRet - WAIT_OBJECT_0) {
			case 0:
			{
				//开始记录命令
				//准备一个MVP矩阵，让立方体转起来
				{
					n64timeCurrent = ::GetTickCount();
					//计算旋转的角度：旋转角度(弧度) = 时间(秒) * 角速度(弧度/秒)
					//下面这句代码相当于经典游戏消息循环中的OnUpdate函数中需要做的事情
					dModelRotationYAngle += ((n64timeCurrent - n64tmFrameStart) / 1000.0f) * g_fPalstance;

					n64tmFrameStart = n64timeCurrent;

					//如果旋转角度大于2PI，则取模
					if (dModelRotationYAngle > XM_2PI) {
						dModelRotationYAngle = fmod(dModelRotationYAngle, XM_2PI);
					}
					//M矩阵
					XMMATRIX xmRot = XMMatrixRotationY(static_cast<float>(dModelRotationYAngle));
					//计算MV
					XMMATRIX xmMVP = XMMatrixMultiply(xmRot, XMMatrixLookAtLH(g_v4EyePos, g_v4LookAt, g_v4UpDir));
					//P矩阵
					xmMVP = XMMatrixMultiply(xmMVP, XMMatrixPerspectiveFovLH(XM_PIDIV4, (FLOAT)iWidth / (FLOAT)iHeight, 0.1f, 1000.0f));

					XMStoreFloat4x4(&pMVPBuffer->m_MVP, xmMVP);
				}
				//Reset命令分配器、命令列表并重新指定命令分配器和PSO对象
				THROW_IF_FAILED(pICommandAllocator->Reset());
				THROW_IF_FAILED(pICommandList->Reset(pICommandAllocator.Get(), pIPipelineState.Get()));

				//获取新的后缓冲序号，因为Present真正完成时后缓冲的序号就更新了
				nFrameIndex = pISwapChain3->GetCurrentBackBufferIndex();

				//通过资源屏障判定后缓冲已经切换完毕可以开始渲染了
				stBeginResBarrier.Transition.pResource = pIARenderTargets[nFrameIndex].Get();
				pICommandList->ResourceBarrier(1, &stBeginResBarrier);

				//偏移描述符指针到指定帧缓冲视图位置
				D3D12_CPU_DESCRIPTOR_HANDLE stRTVHandle = pIRTVHeap->GetCPUDescriptorHandleForHeapStart();
				stRTVHandle.ptr += (nFrameIndex * nRTVDescriptorSize);

				//设置渲染目标
				pICommandList->OMSetRenderTargets(1, &stRTVHandle, FALSE, nullptr);

				pICommandList->RSSetViewports(1, &stViewPort);
				pICommandList->RSSetScissorRects(1, &stScissorRect);

				//继续记录命令，并开始新一帧的渲染
				pICommandList->ClearRenderTargetView(stRTVHandle, faClearColor, 0, nullptr);

				pICommandList->SetGraphicsRootSignature(pIRootSignature.Get());
				ID3D12DescriptorHeap* ppHeaps[] = { pISRVHeap.Get(),pISamplerDescriptorHeap.Get() };
				pICommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

				D3D12_GPU_DESCRIPTOR_HANDLE stGPUSRVHandle = pISRVHeap->GetGPUDescriptorHandleForHeapStart();
				//设置SRV
				pICommandList->SetGraphicsRootDescriptorTable(0, stGPUSRVHandle);

				stGPUSRVHandle.ptr += nSRVDescriptorSize;

				//设置CBV
				pICommandList->SetGraphicsRootDescriptorTable(1, stGPUSRVHandle);

				D3D12_GPU_DESCRIPTOR_HANDLE hGPUSampler = pISamplerDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
				hGPUSampler.ptr += (g_nCurrentSamplerNo * nSamplerDescriptorSize);

				//设置Sample
				pICommandList->SetGraphicsRootDescriptorTable(2, hGPUSampler);

				//注意我们使用的渲染手法是三角形列表，也就是通常的Mesh网格
				pICommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				pICommandList->IASetVertexBuffers(0, 1, &stVertexBufferView);
				pICommandList->IASetIndexBuffer(&stIndexBufferView);
				//Draw Call
				pICommandList->DrawInstanced(_countof(pBoxIndices), 1, 0, 0);
				//资源屏障，用于确定渲染已经结束可以提交画面去显示了
				stEndResBarrier.Transition.pResource = pIARenderTargets[nFrameIndex].Get();
				pICommandList->ResourceBarrier(1, &stEndResBarrier);
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
		ss << _T("Error message: ") << e.what() << std::endl;
		ss << _T("File: ") << (e.File().c_str()) << std::endl;
		ss << _T("Line: ") << e.Line() << std::endl;
		//ss << e.Error();
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
	case WM_KEYDOWN:
	{
		USHORT n16KeyCode = (wParam & 0xFF);
		if (VK_SPACE == n16KeyCode) {
			//按空格键切换不同的采样器看效果
			++g_nCurrentSamplerNo;
			g_nCurrentSamplerNo %= g_nSampleMaxCnt;
		}
		//根据用户输入变换
		if (VK_UP == n16KeyCode || 'w' == n16KeyCode || 'W' == n16KeyCode) {
			g_v4EyePos = XMVectorAdd(g_v4EyePos, XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f));
		}

		if (VK_DOWN == n16KeyCode || 's' == n16KeyCode || 'S' == n16KeyCode) {
			g_v4EyePos = XMVectorAdd(g_v4EyePos, XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f));
		}

		if (VK_RIGHT == n16KeyCode || 'd' == n16KeyCode || 'D' == n16KeyCode) {
			g_v4EyePos = XMVectorAdd(g_v4EyePos, XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f));
		}

		if (VK_LEFT == n16KeyCode || 'a' == n16KeyCode || 'A' == n16KeyCode) {
			g_v4EyePos = XMVectorAdd(g_v4EyePos, XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f));
		}

		if (VK_RIGHT == n16KeyCode || 'q' == n16KeyCode || 'Q' == n16KeyCode) {
			g_v4EyePos = XMVectorAdd(g_v4EyePos, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
		}

		if (VK_LEFT == n16KeyCode || 'e' == n16KeyCode || 'E' == n16KeyCode) {
			g_v4EyePos = XMVectorAdd(g_v4EyePos, XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f));
		}

		if (VK_ADD == n16KeyCode || VK_OEM_PLUS == n16KeyCode) {
			g_fPalstance += 10 * XM_PI / 180.f;
			if (g_fPalstance > XM_PI) {
				g_fPalstance = XM_PI;
			}
		}

		if (VK_SUBTRACT == n16KeyCode || VK_OEM_MINUS == n16KeyCode) {
			g_fPalstance -= 10 * XM_PI / 180.f;
			if (g_fPalstance < 0.0f) {
				g_fPalstance = XM_PI / 180.0f;
			}
		}
		break;
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}