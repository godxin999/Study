//#include "..\include\directx\d3dx12.h"
#include <sdkddkver.h>
#define WIN32_LEAN_AND_MEAN //��Windowsͷ���ų�����ʹ�õ�����
#include <Windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <atlconv.h>//for T2A
#include <atlcoll.h>
#include <wrl.h>//���WTL֧�֣�����ʹ��COM�ӿ�
#include <dxgi1_6.h>
#include <DirectXMath.h>
#include <d3d12.h>
//#include <d3d12shader.h>
#include <d3dcompiler.h>
#if defined(_DEBUG)
#include <dxgidebug.h>
#endif

#include <wincodec.h>//for WIC
//#include "..\WindowsCommons\d3dx12.h"
#include "..\WindowsCommons\DDSTextureLoader12.h"

#include <cstring>
#include <sstream>
#include <fstream>

//using namespace std;
using namespace Microsoft;
using namespace Microsoft::WRL;
using namespace DirectX;

#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"d3dcompiler.lib")

#ifndef BLOCK

#define WND_CLASS_NAME _T("Window Class")
#define WND_TITLE _T("Skybox")
#define GRS_THROW_IF_FAILED(hr) THROW_IF_FAILED(hr)
#define GRS_UPPER_DIV(A,B) UPPER_DIV(A,B)
#define GRS_UPPER(A,B) ALIGN_UP(A,B)
#define THROW_IF_FAILED(hr) if(FAILED(hr)){throw COMException(hr,"Error",__FILE__,__LINE__);}
//��ȡ������
#define UPPER_DIV(A,B) ((UINT)(((A)+((B)-1))/(B)))
//���ϱ߽����
#define ALIGN_UP(A, B) ((UINT)(((A) + ((B) - 1)) & ~((B) - 1)))
//�ڴ����궨��
#define ALLOC(sz) ::HeapAlloc(GetProcessHeap(), 0, (sz))
#define CALLOC(sz) ::HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (sz))
#define CREALLOC(p, sz) ::HeapReAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (p), (sz))
#define SAFE_FREE(p) if ((p) != nullptr) { ::HeapFree(GetProcessHeap(), 0, (p)); (p) = nullptr; }



//�쳣��
#include <exception>

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



//WIC��ʽ��DXGI��ʽת���Ľṹ��
struct WICTranslate {
	GUID                wic;
	DXGI_FORMAT         format;
};

static WICTranslate g_WICFormats[] = {
	//WIC��ʽ��DXGI���ظ�ʽ�Ķ�Ӧ���ñ��еĸ�ʽΪ��֧�ֵĸ�ʽ
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

// WIC ���ظ�ʽת����.
struct WICConvert {
	GUID source;
	GUID target;
};

static WICConvert g_WICConvert[] = {
	//��Ҫ��ǰ��ת��Ϊ���߲ſ���ת��ΪDXGI��ʽ����,Ŀ���ʽһ������ӽ��ı�֧�ֵĸ�ʽ
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

//���ȷ�����ݵ������ʽ���ĸ�
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

//���ȷ�����ն�Ӧ��DXGI��ʽ��ʲô
DXGI_FORMAT GetDXGIFormatFromPixelFormat(const GUID* pPixelFormat) {
	for (size_t i = 0; i < _countof(g_WICFormats); ++i) {
		if (InlineIsEqualGUID(g_WICFormats[i].wic, *pPixelFormat)) {
			return g_WICFormats[i].format;
		}
	}
	return DXGI_FORMAT_UNKNOWN;
}

#endif // !BLOCK

struct ST_VERTEX {
	XMFLOAT4 m_v4Position;//Position����λ��
	XMFLOAT2 m_vTex;//Texcoord��������
	XMFLOAT3 m_vNor;//Normal����
};

struct ST_SKYBOX_VERTEX {
public:
	XMFLOAT4 m_v4Position;
public:
	ST_SKYBOX_VERTEX() :m_v4Position() {}
	ST_SKYBOX_VERTEX(float x, float y, float z) :m_v4Position(x, y, z, 1.0f) {}
	ST_SKYBOX_VERTEX& operator=(const ST_SKYBOX_VERTEX& vt) {
		m_v4Position = vt.m_v4Position;
	}
};

struct ST_FRAME_MVP_BUFFER {
	XMFLOAT4X4 m_MVP;//MVP����
	XMFLOAT4X4 m_mWorld;
	XMFLOAT4 m_v4EyePos;
};

UINT g_nCurrentSamplerNo = 1; //��ǰʹ�õĲ���������
UINT g_nSampleMaxCnt = 5; //����������͵Ĳ�����

//��ʼ��Ĭ���������λ��
XMFLOAT3 g_f3EyePos = XMFLOAT3(0.0f, 2.0f, -10.0f);//�۾�λ��
XMFLOAT3 g_f3LookAt = XMFLOAT3(0.0f, 0.0f, 0.0f);//�����λ��
XMFLOAT3 g_f3HeapUp = XMFLOAT3(0.0f, 1.0f, 0.0f);//���ϵķ���

float g_fYaw = 0.0f;//����Z�����ת��
float g_fPitch = 0.0f;//��XZƽ�����ת��

double g_fPalstance = 10.0f * XM_PI / 180.0f;//������ת�Ľ��ٶȣ���λ: ����/��

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
	::CoInitialize(nullptr);  //for WIC & COM

	const UINT nFrameBackBufCount = 3u;//����������
	int iWndWidth = 1024;//���ڿ��
	int iWndHeight = 768;//���ڸ߶�
	UINT nCurrentFrameIndex = 0;//��ǰ֡����
	UINT nDXGIFactoryFlags = 0u;

	HWND hWnd = nullptr;//���ھ��
	MSG msg = {};//��Ϣ
	TCHAR pszAppPath[MAX_PATH] = {};//Ӧ�ó���·��

	ST_FRAME_MVP_BUFFER* pMVPBufEarth = nullptr;
	ST_FRAME_MVP_BUFFER* pMVPBufSkybox = nullptr;
	//������������С���뵽256Bytes�߽�
	SIZE_T szMVPBuf = ALIGN_UP(sizeof(ST_FRAME_MVP_BUFFER), 256);

	float fSphereSize = 3.0f;

	D3D12_VERTEX_BUFFER_VIEW stVBVEarth = {};
	D3D12_INDEX_BUFFER_VIEW stIBVEarth = {};

	D3D12_VERTEX_BUFFER_VIEW stVBVSkybox = {};
	D3D12_INDEX_BUFFER_VIEW stIBVSkybox = {};

	UINT64 n64FenceValue = 0ui64;//Χ��ֵ
	HANDLE hFenceEvent = nullptr;//Χ���¼����

	UINT nTxtWEarth = 0u;
	UINT nTxtHEarth = 0u;
	UINT nTxtWSkybox = 0u;
	UINT nTxtHSkybox = 0u;
	UINT nBPPEarth = 0u;
	UINT nBPPSkybox = 0u;
	UINT nRowPitchEarth = 0;
	UINT nRowPitchSkybox = 0;
	UINT64 n64szUploadBufEarth = 0;//�ϴ��ѻ�������С
	UINT64 n64szUploadBufSkybox = 0;

	DXGI_FORMAT emRTFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGI_FORMAT emDSFormat = DXGI_FORMAT_D32_FLOAT;
	DXGI_FORMAT emTxtFmtEarth = DXGI_FORMAT_UNKNOWN;//�����ʽ
	const float faClearColor[] = { 0.0f,0.0f,0.0f,0.0f };

	D3D12_PLACED_SUBRESOURCE_FOOTPRINT stTxtLayoutsEarth = {};//������
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT stTxtLayoutsSkybox = {};
	D3D12_RESOURCE_DESC stTextureDesc = {};//������Ϣ�����ṹ��
	D3D12_RESOURCE_DESC stDestDesc = {};//Ŀ����Դ�����ṹ��

	UINT nRTVDescriptorSize = 0u;//RTV��������С
	UINT nSRVDescriptorSize = 0u;//SRV��������С
	UINT nSamplerDescriptorSize = 0;//��������������С

	D3D12_VIEWPORT stViewPort = { 0.0f, 0.0f, static_cast<float>(iWndWidth), static_cast<float>(iWndHeight), D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };//�ӿ����Խṹ��
	D3D12_RECT stScissorRect = { 0, 0, static_cast<LONG>(iWndWidth), static_cast<LONG>(iWndHeight) };//�ü����νṹ��

	//�������������
	ST_VERTEX* pstSphereVertices = nullptr;
	UINT nSphereVertexCnt = 0;
	UINT* pSphereIndices = nullptr;
	UINT nSphereIndexCnt = 0;
	//Skybox����������
	UINT nSkyboxVertexCnt = 0;
	//����Skybox��CubeMap��Ҫ�ı���
	std::unique_ptr<uint8_t[]> ddsData;
	std::vector<D3D12_SUBRESOURCE_DATA> arSubResources;
	DDS_ALPHA_MODE emAlphaMode = DDS_ALPHA_MODE_UNKNOWN;
	bool bIsCube = false;


	ComPtr<IDXGIFactory5> pIDXGIFactory5;
	ComPtr<IDXGIAdapter1> pIAdapter1;

	ComPtr<ID3D12Device4> pID3D12Device4;
	ComPtr<ID3D12CommandQueue> pICmdQueue;
	ComPtr<ID3D12CommandAllocator> pICmdAllocDirect;
	ComPtr<ID3D12CommandAllocator> pICmdAllocSkybox;
	ComPtr<ID3D12CommandAllocator> pICmdAllocEarth;
	ComPtr<ID3D12GraphicsCommandList> pICmdListDirect;
	ComPtr<ID3D12GraphicsCommandList> pIBundlesSkybox;
	ComPtr<ID3D12GraphicsCommandList> pIBundlesEarth;

	ComPtr<IDXGISwapChain1> pISwapChain1;
	ComPtr<IDXGISwapChain3> pISwapChain3;
	ComPtr<ID3D12Resource> pIARenderTargets[nFrameBackBufCount];
	ComPtr<ID3D12DescriptorHeap> pIRTVHeap;//��ȾĿ����������
	ComPtr<ID3D12DescriptorHeap> pIDSVHeap;//��Ȼ�����������
	ComPtr<ID3D12Resource> pIDepthStencilBuffer;//���ģ�建����

	ComPtr<ID3D12Heap> pIRESHeapEarth;//Ĭ�϶�
	ComPtr<ID3D12Heap> pIRESHeapSkybox;//Ĭ�϶�
	ComPtr<ID3D12Heap> pIUploadHeapEarth;//�ϴ���
	ComPtr<ID3D12Heap> pIUploadHeapSkybox;//�ϴ���

	ComPtr<ID3D12Resource> pITextureEarth;//������Դ
	ComPtr<ID3D12Resource> pITextureUploadEarth;//�ϴ���������Դ
	ComPtr<ID3D12Resource> pIVBEarth;//���㻺����
	ComPtr<ID3D12Resource> pIIBEarth;//����������
	ComPtr<ID3D12Resource> pICBUploadEarth;//�ϴ��ѳ���������

	ComPtr<ID3D12Resource> pITextureSkybox;
	ComPtr<ID3D12Resource> pITextureUploadSkybox;
	ComPtr<ID3D12Resource> pIVBSkybox;//���㻺����
	ComPtr<ID3D12Resource> pICBUploadSkybox;//�ϴ��ѳ���������

	ComPtr<ID3D12DescriptorHeap> pISRVHpEarth;//��ɫ����Դ��������
	ComPtr<ID3D12DescriptorHeap> pISampleHpEarth;//��������������
	ComPtr<ID3D12DescriptorHeap> pISRVHpSkybox;
	ComPtr<ID3D12DescriptorHeap> pISampleHpSkybox;

	ComPtr<ID3D12Fence> pIFence;

	ComPtr<ID3D12RootSignature> pIRootSignature;
	ComPtr<ID3D12PipelineState> pIPSOEarth;//����״̬����
	ComPtr<ID3D12PipelineState> pIPSOSkybox;

	//WIC�ӿ�
	ComPtr<IWICImagingFactory> pIWICFactory;//WIC�����ӿ�
	ComPtr<IWICBitmapDecoder> pIWICDecoder;//WIC�������ӿ�
	ComPtr<IWICBitmapFrameDecode> pIWICFrame;//WICλͼ֡����ӿ�
	ComPtr<IWICBitmapSource> pIBMPEarth;//λͼ��ʽ��ͼƬ���ݶ���ӿ�
	//ʹ��WIC����������ͼƬ��תΪDXGI���ݸ�ʽ����ȡͼƬ����->����Ĭ�϶�->����ͼƬ���Դ���2D������Ĭ�϶ѷ�����Դ����(����)->�����ϴ���->���ϴ��ѷ�����Դ����(������)->����ͼƬ���ϴ��ѵ���Դ������->ͨ�������б��ϴ��ѵ���Դ���󿽱���Ĭ�϶ѵ���Դ������
	try {
		// �õ���ǰ�Ĺ���Ŀ¼����������ʹ�����·�������ʸ�����Դ�ļ�
		{
			if (0 == ::GetModuleFileName(nullptr, pszAppPath, MAX_PATH))
			{
				THROW_IF_FAILED(HRESULT_FROM_WIN32(GetLastError()));
			}
			WCHAR* lastSlash = _tcsrchr(pszAppPath, _T('\\'));
			if (lastSlash)
			{//ɾ��exe�ļ���
				*(lastSlash) = _T('\0');
			}
			lastSlash = _tcsrchr(pszAppPath, _T('\\'));
			if (lastSlash)
			{//ɾ��x64·��
				*(lastSlash) = _T('\0');
			}
			lastSlash = _tcsrchr(pszAppPath, _T('\\'));
			if (lastSlash)
			{//ɾ��Debug �� Release·��
				*(lastSlash) = _T('\0');
			}
			lastSlash = _tcsrchr(pszAppPath, _T('\\'));
			if (lastSlash)
			{//ɾ����ǰ·��
				*(lastSlash + 1) = _T('\0');
			}
		}
		//��������
		{
			WNDCLASSEX wcex = {};
			wcex.cbSize = sizeof(WNDCLASSEX);
			wcex.style = CS_GLOBALCLASS;
			wcex.lpfnWndProc = WndProc;
			wcex.cbClsExtra = 0;
			wcex.cbWndExtra = 0;
			wcex.hInstance = hInstance;
			wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
			wcex.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);//��ֹ���ĵı����ػ�
			wcex.lpszClassName = WND_CLASS_NAME;

			RegisterClassEx(&wcex);

			DWORD dwWndStyle = WS_OVERLAPPED | WS_SYSMENU;
			RECT rtWnd = { 0,0,iWndWidth,iWndHeight };
			AdjustWindowRect(&rtWnd, dwWndStyle, FALSE);

			//���㴰�ھ��е���Ļ����
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
		//ʹ��WIC����������һ��ͼƬ����ת��ΪDXGI���ݵĸ�ʽ
		{
			ComPtr<IWICFormatConverter> pIConverter;
			ComPtr<IWICComponentInfo> pIWICmntinfo;
			WICPixelFormatGUID wpf = {};
			GUID tgFormat = {};
			WICComponentType type;
			ComPtr<IWICPixelFormatInfo> pIWICPixelinfo;
			//ʹ�ô�COM��ʽ����WIC�೧����Ҳ�ǵ���WIC��һ��Ҫ��������
			THROW_IF_FAILED(CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pIWICFactory)));
			//ʹ��WIC�೧����ӿڼ�������ͼƬ�����õ�һ��WIC����������ӿڣ�ͼƬ��Ϣ��������ӿڴ���Ķ�������
			WCHAR pszTexcuteFileName[MAX_PATH] = {};
			StringCchPrintfW(pszTexcuteFileName, MAX_PATH, _T("%sAssets\\����.jpg"), pszAppPath);

			THROW_IF_FAILED(pIWICFactory->CreateDecoderFromFilename(
				pszTexcuteFileName,//�ļ���
				nullptr,//��ָ����������ʹ��Ĭ��
				GENERIC_READ,//��Ȩ��
				WICDecodeMetadataCacheOnDemand,//����Ҫ�򻺳�����
				&pIWICDecoder));//����������

			//��ȡ��һ֡ͼƬ(��ΪGIF�ȸ�ʽ�ļ����ܻ��ж�֡ͼƬ�������ĸ�ʽһ��ֻ��һ֡ͼƬ)��ʵ�ʽ���������������λͼ��ʽ����
			THROW_IF_FAILED(pIWICDecoder->GetFrame(0, &pIWICFrame));

			//��ȡWICͼƬ��ʽ
			THROW_IF_FAILED(pIWICFrame->GetPixelFormat(&wpf));

			//ת��ΪDXGI�ĵȼ۸�ʽ
			if (GetTargetPixelFormat(&wpf, &tgFormat)) {
				emTxtFmtEarth = GetDXGIFormatFromPixelFormat(&tgFormat);
			}

			if (emTxtFmtEarth == DXGI_FORMAT_UNKNOWN) {
				throw COMException(S_FALSE);
			}
			if (!InlineIsEqualGUID(wpf, tgFormat)) {
				//���WICͼƬ��ʽ��DXGI�ȼ۸�ʽ��һ�£�����Ҫ���и�ʽת��
				//����ͼƬ��ʽת����
				THROW_IF_FAILED(pIWICFactory->CreateFormatConverter(&pIConverter));

				//��ʼ��һ��ͼƬת������ʵ��Ҳ���ǽ�ͼƬ���ݽ����˸�ʽת��
				THROW_IF_FAILED(pIConverter->Initialize(
					pIWICFrame.Get(),//ԴͼƬ����
					tgFormat,//Ŀ��ͼƬ��ʽ
					WICBitmapDitherTypeNone,//ָ��λͼ�Ƿ��е�ɫ�壬�ִ��������λͼ�����õ�ɫ�壬����ΪNone
					nullptr,//��ʹ�õ�ɫ��
					0.0,//ָ��alpha��ֵ
					WICBitmapPaletteTypeCustom//��ɫ�����ͣ�û��ʹ�ã�ָ��ΪCustom
				));
				//����QueryInterface������ö����λͼ����Դ�ӿ�
				THROW_IF_FAILED(pIConverter.As(&pIBMPEarth));

			}
			else {
				//ͼƬ���ݸ�ʽ����Ҫת����ֱ�ӻ�ȡ��λͼ����Դ�ӿ�
				THROW_IF_FAILED(pIWICFrame.As(&pIBMPEarth));
			}

			//��ȡͼƬ��С
			THROW_IF_FAILED(pIBMPEarth->GetSize(&nTxtWEarth, &nTxtHEarth));
			//��ȡͼƬ���ص�λ��С��BPP(bits per pixel)��Ϣ�����ڼ���ͼƬ�����ݵ���ʵ��С(��λ:�ֽ�)
			THROW_IF_FAILED(pIWICFactory->CreateComponentInfo(tgFormat, pIWICmntinfo.GetAddressOf()));

			THROW_IF_FAILED(pIWICmntinfo->GetComponentType(&type));

			if (type != WICPixelFormat) {
				throw COMException(S_FALSE);
			}

			THROW_IF_FAILED(pIWICmntinfo.As(&pIWICPixelinfo));
			//���BPP
			THROW_IF_FAILED(pIWICPixelinfo->GetBitsPerPixel(&nBPPEarth));
			//����ͼƬʵ�ʵ��д�С(��λ:�ֽ�)��ʹ��һ����ȡ����������A+B-1��/B
			nRowPitchEarth = UPPER_DIV(uint64_t(nTxtWEarth) * uint64_t(nBPPEarth), 8);
		}
		//����ʾ��ϵͳ�ĵ���֧��
		{
#if defined(_DEBUG)
			ComPtr<ID3D12Debug> debugController;
			if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
			{
				debugController->EnableDebugLayer();
				// �򿪸��ӵĵ���֧��
				nDXGIFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
			}
#endif
		}
		//����DXGI Factory����
		{
			THROW_IF_FAILED(CreateDXGIFactory2(nDXGIFactoryFlags, IID_PPV_ARGS(&pIDXGIFactory5)));//��������
		}
		//ö������������ѡ����ʵ�������������3D�豸����
		{
			//ѡ��NUMA�ܹ��Ķ���������3D�豸����
			DXGI_ADAPTER_DESC1	desc = {};
			D3D12_FEATURE_DATA_ARCHITECTURE stArchitecture = {};
			for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != pIDXGIFactory5->EnumAdapters1(adapterIndex, &pIAdapter1); ++adapterIndex) {
				pIAdapter1->GetDesc1(&desc);
				if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
					continue;//�������������������
				}
				THROW_IF_FAILED(D3D12CreateDevice(pIAdapter1.Get(), D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&pID3D12Device4)));
				THROW_IF_FAILED(pID3D12Device4->CheckFeatureSupport(D3D12_FEATURE_ARCHITECTURE, &stArchitecture, sizeof(D3D12_FEATURE_DATA_ARCHITECTURE)));
				if (!stArchitecture.UMA) {
					break;
				}
				pID3D12Device4.Reset();
			}
			//û�ж��ԣ����쳣�˳�
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

			//��ȡÿ��������Ԫ�صĴ�С
			nRTVDescriptorSize = pID3D12Device4->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
			nSRVDescriptorSize = pID3D12Device4->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			nSamplerDescriptorSize = pID3D12Device4->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
		}
		//����3Dͼ���������
		{
			D3D12_COMMAND_QUEUE_DESC queueDesc = {};
			queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
			THROW_IF_FAILED(pID3D12Device4->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&pICmdQueue)));
		}
		//���������б��������ͼ�������б�������
		{
			THROW_IF_FAILED(pID3D12Device4->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&pICmdAllocDirect)));
			THROW_IF_FAILED(pID3D12Device4->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, pICmdAllocDirect.Get(), nullptr, IID_PPV_ARGS(&pICmdListDirect)));

			THROW_IF_FAILED(pID3D12Device4->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_BUNDLE, IID_PPV_ARGS(&pICmdAllocEarth)));
			THROW_IF_FAILED(pID3D12Device4->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_BUNDLE, pICmdAllocEarth.Get(), nullptr, IID_PPV_ARGS(&pIBundlesEarth)));

			THROW_IF_FAILED(pID3D12Device4->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_BUNDLE, IID_PPV_ARGS(&pICmdAllocSkybox)));
			THROW_IF_FAILED(pID3D12Device4->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_BUNDLE, pICmdAllocSkybox.Get(), nullptr, IID_PPV_ARGS(&pIBundlesSkybox)));

		}
		//����������
		{
			DXGI_SWAP_CHAIN_DESC1 stSwapChainDesc = {};
			stSwapChainDesc.BufferCount = nFrameBackBufCount;
			stSwapChainDesc.Width = iWndWidth;
			stSwapChainDesc.Height = iWndHeight;
			stSwapChainDesc.Format = emRTFormat;
			stSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			stSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			stSwapChainDesc.SampleDesc.Count = 1;

			THROW_IF_FAILED(pIDXGIFactory5->CreateSwapChainForHwnd(
				pICmdQueue.Get(),
				hWnd,
				&stSwapChainDesc,
				nullptr,
				nullptr,
				&pISwapChain1
			));
			//�ر�alt+enter�л�ȫ��
			THROW_IF_FAILED(pIDXGIFactory5->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER));

			THROW_IF_FAILED(pISwapChain1.As(&pISwapChain3));//ʹ��As�������Ͱ汾��SwapChain�ӿ�ת��Ϊ�߰汾��SwapChain�ӿ�
			nCurrentFrameIndex = pISwapChain3->GetCurrentBackBufferIndex();//���ø߰汾�ӿڵ�GetCurrentBackBufferIndex������ȡ��ǰ��̨����������
		}
		//����RTV�������Ѽ�RTV������
		{
			D3D12_DESCRIPTOR_HEAP_DESC stRtvHeapDesc = {};
			stRtvHeapDesc.NumDescriptors = nFrameBackBufCount;
			stRtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			stRtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

			THROW_IF_FAILED(pID3D12Device4->CreateDescriptorHeap(&stRtvHeapDesc, IID_PPV_ARGS(&pIRTVHeap)));

			//����RTV������
			D3D12_CPU_DESCRIPTOR_HANDLE stRTVHandle = pIRTVHeap->GetCPUDescriptorHandleForHeapStart();//�����������ѵĵ�ǰԪ��ָ��λ��
			for (UINT i = 0; i < nFrameBackBufCount; ++i) {
				THROW_IF_FAILED(pISwapChain3->GetBuffer(i, IID_PPV_ARGS(&pIARenderTargets[i])));
				pID3D12Device4->CreateRenderTargetView(pIARenderTargets[i].Get(), nullptr, stRTVHandle);
				stRTVHandle.ptr += nRTVDescriptorSize;
			}
		}
		//������Ȼ����Լ���Ȼ�����������
		{
			D3D12_HEAP_PROPERTIES stDSBufHeapDesc = {};

			stDSBufHeapDesc.Type = D3D12_HEAP_TYPE_DEFAULT;
			stDSBufHeapDesc.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			stDSBufHeapDesc.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
			stDSBufHeapDesc.CreationNodeMask = 0;
			stDSBufHeapDesc.VisibleNodeMask = 0;

			D3D12_CLEAR_VALUE depthOptimizedClearValue = {};
			depthOptimizedClearValue.Format = emDSFormat;
			depthOptimizedClearValue.DepthStencil.Depth = 1.0f;
			depthOptimizedClearValue.DepthStencil.Stencil = 0;

			D3D12_RESOURCE_DESC stDSResDesc = {};
			stDSResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
			stDSResDesc.Alignment = 0;
			stDSResDesc.Width = iWndWidth;
			stDSResDesc.Height = iWndHeight;
			stDSResDesc.DepthOrArraySize = 1;
			stDSResDesc.MipLevels = 1;
			stDSResDesc.Format = emDSFormat;
			stDSResDesc.SampleDesc.Count = 1;
			stDSResDesc.SampleDesc.Quality = 0;
			stDSResDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
			stDSResDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
			//ʹ����ʽĬ�϶Ѵ������ģ�建��������Ϊ��Ȼ�����һֱ��ʹ�ã��������õ����岻��ֱ��ʹ����ʽ��
			THROW_IF_FAILED(pID3D12Device4->CreateCommittedResource(
				&stDSBufHeapDesc,
				D3D12_HEAP_FLAG_NONE,
				&stDSResDesc,
				D3D12_RESOURCE_STATE_DEPTH_WRITE,
				&depthOptimizedClearValue,
				IID_PPV_ARGS(&pIDepthStencilBuffer)
			));
			//�������ģ�建������������
			D3D12_DESCRIPTOR_HEAP_DESC stDSVHeapDesc = {};
			stDSVHeapDesc.NumDescriptors = 1;
			stDSVHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
			stDSVHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			THROW_IF_FAILED(pID3D12Device4->CreateDescriptorHeap(&stDSVHeapDesc, IID_PPV_ARGS(&pIDSVHeap)));
			//�������ģ�建������ͼ
			D3D12_DEPTH_STENCIL_VIEW_DESC stDepthStencilDesc = {};
			stDepthStencilDesc.Format = emDSFormat;
			stDepthStencilDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
			stDepthStencilDesc.Flags = D3D12_DSV_FLAG_NONE;
			pID3D12Device4->CreateDepthStencilView(pIDepthStencilBuffer.Get(), &stDepthStencilDesc, pIDSVHeap->GetCPUDescriptorHandleForHeapStart());
		}
		//����SRV��CBV��Sample��������
		{
			//��������ͼ��������CBV����������һ������������
			D3D12_DESCRIPTOR_HEAP_DESC stSRVHeapDesc = {};
			stSRVHeapDesc.NumDescriptors = 2;//1SRV + 1CBV
			stSRVHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			stSRVHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			THROW_IF_FAILED(pID3D12Device4->CreateDescriptorHeap(&stSRVHeapDesc, IID_PPV_ARGS(&pISRVHpEarth)));

			D3D12_DESCRIPTOR_HEAP_DESC stSamplerHeapDesc = {};
			stSamplerHeapDesc.NumDescriptors = g_nSampleMaxCnt;
			stSamplerHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
			stSamplerHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

			THROW_IF_FAILED(pID3D12Device4->CreateDescriptorHeap(&stSamplerHeapDesc, IID_PPV_ARGS(&pISampleHpEarth)));

			//Skybox��SRV��CBV��Sample��������
			THROW_IF_FAILED(pID3D12Device4->CreateDescriptorHeap(&stSRVHeapDesc, IID_PPV_ARGS(&pISRVHpSkybox)));
			stSamplerHeapDesc.NumDescriptors = 1;//Skyboxֻ��һ��������
			THROW_IF_FAILED(pID3D12Device4->CreateDescriptorHeap(&stSamplerHeapDesc, IID_PPV_ARGS(&pISampleHpSkybox)));

		}
		//������ǩ��
		{
			//��������У������Skyboxʹ����ͬ�ĸ�ǩ������Ϊ��Ⱦ��������Ҫ�Ĳ�����һ����
			D3D12_FEATURE_DATA_ROOT_SIGNATURE stFeatureData = {};
			//����Ƿ�֧��V1.1�ĸ�ǩ��
			stFeatureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;
			if (FAILED(pID3D12Device4->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &stFeatureData, sizeof(stFeatureData)))) {
				THROW_IF_FAILED(E_NOTIMPL);
			}
			//��GPU��ִ��SetGraphicsRootDescriptorTable�����ǲ��޸������б��е�SRV��������ǿ���ʹ��Ĭ��Range��Ϊ
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
			stRootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//SRV��PS�ɼ�
			stRootParameters[0].DescriptorTable.NumDescriptorRanges = 1;
			stRootParameters[0].DescriptorTable.pDescriptorRanges = &stDSPRanges[0];

			stRootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			stRootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//CBV����Shader�ɼ�
			stRootParameters[1].DescriptorTable.NumDescriptorRanges = 1;
			stRootParameters[1].DescriptorTable.pDescriptorRanges = &stDSPRanges[1];

			stRootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			stRootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//Sample��PS�ɼ�
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
		//����Shader��������Ⱦ����״̬����
		{
			ComPtr<ID3DBlob> pIVSEarth;
			ComPtr<ID3DBlob> pIPSEarth;
			UINT nCompileFlags = 0;
#if defined(_DEBUG)
			nCompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif // defined(_DEBUG)

			//����Ϊ�о�����ʽ
			nCompileFlags |= D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;

			TCHAR pszShaderFileName[MAX_PATH] = {};
			StringCchPrintf(pszShaderFileName, MAX_PATH, _T("%s5-SkyBox\\Shader\\TextureCube.hlsl"), pszAppPath);
			THROW_IF_FAILED(D3DCompileFromFile(pszShaderFileName, nullptr, nullptr, "VSMain", "vs_5_0", nCompileFlags, 0, &pIVSEarth, nullptr));
			THROW_IF_FAILED(D3DCompileFromFile(pszShaderFileName, nullptr, nullptr, "PSMain", "ps_5_0", nCompileFlags, 0, &pIPSEarth, nullptr));

			D3D12_INPUT_ELEMENT_DESC stIALayoutEarth[] = {
				{ "POSITION",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,0,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
				{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,16,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
				{ "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,24,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 }
			};

			//������Ⱦ����
			D3D12_GRAPHICS_PIPELINE_STATE_DESC stPSODesc = {};
			stPSODesc.InputLayout = { stIALayoutEarth,_countof(stIALayoutEarth) };

			stPSODesc.pRootSignature = pIRootSignature.Get();
			stPSODesc.VS.BytecodeLength = pIVSEarth->GetBufferSize();
			stPSODesc.VS.pShaderBytecode = pIVSEarth->GetBufferPointer();
			stPSODesc.PS.BytecodeLength = pIPSEarth->GetBufferSize();
			stPSODesc.PS.pShaderBytecode = pIPSEarth->GetBufferPointer();

			stPSODesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
			stPSODesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;

			stPSODesc.BlendState.AlphaToCoverageEnable = FALSE;
			stPSODesc.BlendState.IndependentBlendEnable = FALSE;
			stPSODesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

			stPSODesc.SampleMask = UINT_MAX;
			stPSODesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
			stPSODesc.NumRenderTargets = 1;
			stPSODesc.RTVFormats[0] = emRTFormat;
			stPSODesc.DSVFormat = emDSFormat;
			stPSODesc.DepthStencilState.DepthEnable = TRUE;
			stPSODesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//������Ȼ���д�빦��
			stPSODesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;//��Ȳ��Ժ���(��ͨ����Ȳ���)
			stPSODesc.DepthStencilState.StencilEnable = FALSE;
			stPSODesc.SampleDesc.Count = 1;

			THROW_IF_FAILED(pID3D12Device4->CreateGraphicsPipelineState(&stPSODesc, IID_PPV_ARGS(&pIPSOEarth)));

			//����Ϊ�о�����ʽ
			nCompileFlags |= D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
			TCHAR pszSMFileSkybox[MAX_PATH] = {};
			StringCchPrintf(pszSMFileSkybox, MAX_PATH, _T("%s5-SkyBox\\Shader\\Skybox.hlsl"), pszAppPath);

			ComPtr<ID3DBlob> pIVSSkybox;
			ComPtr<ID3DBlob> pIPSSkybox;

			THROW_IF_FAILED(D3DCompileFromFile(pszSMFileSkybox, nullptr, nullptr, "SkyboxVS", "vs_5_0", nCompileFlags, 0, &pIVSSkybox, nullptr));
			THROW_IF_FAILED(D3DCompileFromFile(pszSMFileSkybox, nullptr, nullptr, "SkyboxPS", "ps_5_0", nCompileFlags, 0, &pIPSSkybox, nullptr));

			//Skyboxֻ�ж���ֻ��λ�ò���
			D3D12_INPUT_ELEMENT_DESC stIALayoutSkybox[] = {
				{ "POSITION",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,0,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 }
			};

			//����Skybox��PSO����
			stPSODesc.InputLayout = { stIALayoutSkybox,_countof(stIALayoutSkybox) };
			stPSODesc.VS.BytecodeLength = pIVSSkybox->GetBufferSize();
			stPSODesc.VS.pShaderBytecode = pIVSSkybox->GetBufferPointer();
			stPSODesc.PS.BytecodeLength = pIPSSkybox->GetBufferSize();
			stPSODesc.PS.pShaderBytecode = pIPSSkybox->GetBufferPointer();
			stPSODesc.DepthStencilState.DepthEnable = FALSE;
			stPSODesc.DepthStencilState.StencilEnable = FALSE;

			THROW_IF_FAILED(pID3D12Device4->CreateGraphicsPipelineState(&stPSODesc, IID_PPV_ARGS(&pIPSOSkybox)));

		}
		//���������Ĭ�϶�
		{
			D3D12_HEAP_DESC stTextureHeapDesc = {};
			//Ϊ��ָ������ͼƬ����������С�Ŀռ䣬ʵ��Ӧ����Ҳ��Ҫ�ۺϿ��Ƿ���ѵĴ�С�Ա�������ö�
			stTextureHeapDesc.SizeInBytes = ALIGN_UP(2 * nRowPitchEarth * nTxtHEarth, D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT);
			//ָ���ѵĶ��뷽ʽ������ʹ����Ĭ�ϵ�64K�߽���䣬��Ϊ������ʱ����ҪMSAA֧��
			stTextureHeapDesc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
			stTextureHeapDesc.Properties.Type = D3D12_HEAP_TYPE_DEFAULT;
			stTextureHeapDesc.Properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			stTextureHeapDesc.Properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
			//�ܾ���ȾĿ�������ܾ����ģ������ʵ��ֻ�������ڷ���ͨ����
			stTextureHeapDesc.Flags = D3D12_HEAP_FLAG_DENY_RT_DS_TEXTURES | D3D12_HEAP_FLAG_DENY_BUFFERS;

			THROW_IF_FAILED(pID3D12Device4->CreateHeap(&stTextureHeapDesc, IID_PPV_ARGS(&pIRESHeapEarth)));
		}
		//����2D����
		{
			//����ͼƬ��Ϣ���2D������Դ����Ϣ�ṹ��
			stTextureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
			stTextureDesc.MipLevels = 1;
			stTextureDesc.Format = emTxtFmtEarth;
			stTextureDesc.Width = nTxtWEarth;
			stTextureDesc.Height = nTxtHEarth;
			stTextureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
			stTextureDesc.DepthOrArraySize = 1;
			stTextureDesc.SampleDesc.Count = 1;
			stTextureDesc.SampleDesc.Quality = 0;

			//ʹ�ö�λ��ʽ���������������ڲ������ڴ洢������ͷŵ�ʵ�ʲ��������ܽϸ�
			THROW_IF_FAILED(pID3D12Device4->CreatePlacedResource(
				pIRESHeapEarth.Get(),
				0,
				&stTextureDesc,
				D3D12_RESOURCE_STATE_COPY_DEST,
				nullptr,
				IID_PPV_ARGS(&pITextureEarth)
			));

			//��ȡ�ϴ��ѻ�����Դ�Ĵ�С������ߴ�ͨ������ʵ��ͼƬ�ĳߴ�
			D3D12_RESOURCE_DESC stCopyDstDesc = pITextureEarth->GetDesc();
			pID3D12Device4->GetCopyableFootprints(&stCopyDstDesc, 0, 1, 0, nullptr, nullptr, nullptr, &n64szUploadBufEarth);
		}
		//�����ϴ���
		{
			D3D12_HEAP_DESC stUploadHeapDesc = {};
			//�ߴ���Ȼ��ʵ���������ݴ�С��2����64K�߽����
			stUploadHeapDesc.SizeInBytes = ALIGN_UP(2 * n64szUploadBufEarth, D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT);
			//�ϴ��ѿ϶���Buffer���ͣ����Բ�ָ�����䷽ʽ��Ĭ����64k�߽����
			stUploadHeapDesc.Alignment = 0;
			stUploadHeapDesc.Properties.Type = D3D12_HEAP_TYPE_UPLOAD;
			stUploadHeapDesc.Properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			stUploadHeapDesc.Properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
			//�ϴ����ǻ��壬���԰ڷ���������
			stUploadHeapDesc.Flags = D3D12_HEAP_FLAG_ALLOW_ONLY_BUFFERS;
			THROW_IF_FAILED(pID3D12Device4->CreateHeap(&stUploadHeapDesc, IID_PPV_ARGS(&pIUploadHeapEarth)));
		}
		//ʹ�ö�λ��ʽ���������ϴ��������ݵĻ�����Դ
		{
			D3D12_RESOURCE_DESC stUploadResDesc = {};
			stUploadResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
			stUploadResDesc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
			stUploadResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
			stUploadResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
			stUploadResDesc.Format = DXGI_FORMAT_UNKNOWN;
			stUploadResDesc.Width = n64szUploadBufEarth;
			stUploadResDesc.Height = 1;
			stUploadResDesc.DepthOrArraySize = 1;
			stUploadResDesc.MipLevels = 1;
			stUploadResDesc.SampleDesc.Count = 1;
			stUploadResDesc.SampleDesc.Quality = 0;

			THROW_IF_FAILED(pID3D12Device4->CreatePlacedResource(
				pIUploadHeapEarth.Get(),
				0,
				&stUploadResDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&pITextureUploadEarth)
			));
		}
		//����ͼƬ���ϴ��ѣ�����ɵ�һ��Copy��������memcpy��֪������CPU��ɵ�
		{

			//������Դ�����С������ʵ��ͼƬ���ݴ洢���ڴ��С
			void* pbPicData = CALLOC(n64szUploadBufEarth);
			if (pbPicData == nullptr) {
				throw COMException(HRESULT_FROM_WIN32(GetLastError()));
			}
			//��ͼƬ��������
			THROW_IF_FAILED(pIBMPEarth->CopyPixels(
				nullptr,
				nRowPitchEarth,
				static_cast<UINT>(nRowPitchEarth * nTxtHEarth),//�������ͼƬ������ʵ�Ĵ�С�����ֵͨ��С�ڻ���Ĵ�С
				reinterpret_cast<BYTE*>(pbPicData)
			));

			//��ȡ���ϴ��ѿ����������ݵ�һЩ����ת���ߴ���Ϣ�����ڸ��ӵ�DDS�������Ƿǳ���Ҫ�Ĺ���
			UINT nNumSubresources = 1u;
			UINT nTextureRowNum = 0u;
			UINT64 n64RequiredSize = 0u;
			UINT64 n64TextureRowSizes = 0u;

			stDestDesc = pITextureEarth->GetDesc();

			pID3D12Device4->GetCopyableFootprints(&stDestDesc, 0, nNumSubresources, 0, &stTxtLayoutsEarth, &nTextureRowNum, &n64TextureRowSizes, &n64RequiredSize);

			//��Ϊ�ϴ���ʵ�ʾ���CPU�������ݵ�GPU���н�,�������ǿ���ʹ����Ϥ��Map����������ӳ�䵽CPU�ڴ��ַ��,Ȼ�����ǰ��н����ݸ��Ƶ��ϴ�����,��Ҫע�����֮���԰��п�������ΪGPU��Դ���д�С,��ʵ��ͼƬ���д�С���в����,���ߵ��ڴ�߽����Ҫ���ǲ�һ����
			BYTE* pData = nullptr;
			THROW_IF_FAILED(pITextureUploadEarth->Map(0, nullptr, reinterpret_cast<void**>(&pData)));

			BYTE* pDestSlice = reinterpret_cast<BYTE*>(pData) + stTxtLayoutsEarth.Offset;
			const BYTE* pSrcSlice = reinterpret_cast<const BYTE*>(pbPicData);
			for (UINT y = 0; y < nTextureRowNum; ++y) {
				memcpy(pDestSlice + static_cast<SIZE_T>(stTxtLayoutsEarth.Footprint.RowPitch) * y, pSrcSlice + static_cast<SIZE_T>(nRowPitchEarth) * y, nRowPitchEarth);
			}

			pITextureUploadEarth->Unmap(0, nullptr);
			//�ͷ�ͼƬ����
			SAFE_FREE(pbPicData);
		}
		//ʹ��DDSLoader������������Skybox������
		{
			TCHAR pszSkyboxTextureFile[MAX_PATH] = {};
			StringCchPrintf(pszSkyboxTextureFile, MAX_PATH, _T("%sAssets\\Sky_cube_1024.dds"), pszAppPath);

			ID3D12Resource* pIResSkybox = nullptr;
			THROW_IF_FAILED(LoadDDSTextureFromFile(
				pID3D12Device4.Get(),
				pszSkyboxTextureFile,
				&pIResSkybox,
				ddsData,
				arSubResources,
				SIZE_MAX,
				&emAlphaMode,
				&bIsCube));
			//���溯�����ص���������ʽĬ�϶��ϣ�����Copy������Ҫ�����Լ����
			pITextureSkybox.Attach(pIResSkybox);//��Ĭ�϶��е������ӵ�pITextureSkybox����ָ���ϣ��Ӷ��ӹ���Դ���������Ȩ
			//��ȡSkybox�Ĵ�С���������ϴ���
			D3D12_RESOURCE_DESC stCopyDstDest = pITextureSkybox->GetDesc();
			//��һ�ε���GetCopyableFootprints
			pID3D12Device4->GetCopyableFootprints(&stCopyDstDest, 0, static_cast<UINT>(arSubResources.size()), 0, nullptr, nullptr, nullptr, &n64szUploadBufSkybox);

			D3D12_HEAP_DESC stUploadHeapDesc = {};
			//�ߴ���Ȼ��ʵ���������ݴ�С��2����64K�߽�����С
			stUploadHeapDesc.SizeInBytes = ALIGN_UP(2 * n64szUploadBufSkybox, D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT);
			stUploadHeapDesc.Alignment = 0;
			stUploadHeapDesc.Properties.Type = D3D12_HEAP_TYPE_UPLOAD;
			stUploadHeapDesc.Properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			stUploadHeapDesc.Properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
			//�ϴ��Ѿ��ǻ��壬���԰ݷ���������
			stUploadHeapDesc.Flags = D3D12_HEAP_FLAG_ALLOW_ONLY_BUFFERS;

			THROW_IF_FAILED(pID3D12Device4->CreateHeap(&stUploadHeapDesc, IID_PPV_ARGS(&pIUploadHeapSkybox)));

			D3D12_RESOURCE_DESC stUploadBufDesc = {};
			stUploadBufDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
			stUploadBufDesc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
			stUploadBufDesc.Width = n64szUploadBufSkybox;
			stUploadBufDesc.Height = 1;
			stUploadBufDesc.DepthOrArraySize = 1;
			stUploadBufDesc.MipLevels = 1;
			stUploadBufDesc.Format = DXGI_FORMAT_UNKNOWN;
			stUploadBufDesc.SampleDesc.Count = 1;
			stUploadBufDesc.SampleDesc.Quality = 0;
			stUploadBufDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
			stUploadBufDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

			THROW_IF_FAILED(pID3D12Device4->CreatePlacedResource(pIUploadHeapSkybox.Get(), 0, &stUploadBufDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&pITextureUploadSkybox)));

			//�ϴ�Skybox������ע����Ϊ�������պ�����Դ��DDS��ʽ�ģ������ж��Mipmap����������Դ�������ܶ�
			UINT nFirstSubResources = 0;
			UINT nNumSubResources = static_cast<UINT>(arSubResources.size());
			D3D12_RESOURCE_DESC stUploadResDesc = pITextureUploadSkybox->GetDesc();
			D3D12_RESOURCE_DESC stDefaultResDesc = pITextureSkybox->GetDesc();

			UINT64 n64RequiredSize = 0;
			SIZE_T szMemToAlloc = static_cast<UINT64>(sizeof(D3D12_PLACED_SUBRESOURCE_FOOTPRINT) + sizeof(UINT) + sizeof(UINT64)) * nNumSubResources;

			void* pMem = CALLOC(static_cast<SIZE_T>(szMemToAlloc));

			if (pMem == nullptr) {
				throw COMException(HRESULT_FROM_WIN32(GetLastError()));
			}
			//���ڴ��Ϊ�������֣���һ�����ǲ��֣��ڶ�������ÿ�еĴ�С����������������
			D3D12_PLACED_SUBRESOURCE_FOOTPRINT* pLayouts = reinterpret_cast<D3D12_PLACED_SUBRESOURCE_FOOTPRINT*>(pMem);
			UINT64* pRowSizesInBytes = reinterpret_cast<UINT64*>(pLayouts + nNumSubResources);
			UINT* pNumRows = reinterpret_cast<UINT*>(pRowSizesInBytes + nNumSubResources);

			//�ڶ��ε���GetCopyableFootprints���õ���������Դ����ϸ��Ϣ
			pID3D12Device4->GetCopyableFootprints(&stDefaultResDesc, nFirstSubResources, nNumSubResources, 0, pLayouts, pNumRows, pRowSizesInBytes, &n64RequiredSize);

			BYTE* pData = nullptr;
			HRESULT hr = pITextureUploadSkybox->Map(0, nullptr, reinterpret_cast<void**>(&pData));
			if (FAILED(hr)) {
				return 0;
			}
			//��һ��Copy
			for (UINT i = 0; i < nNumSubResources; ++i) {
				//SubReSources
				if (pRowSizesInBytes[i] > (SIZE_T)-1) {
					throw COMException(E_FAIL);
				}

				D3D12_MEMCPY_DEST stCopyDestData = {
					pData + pLayouts[i].Offset,
					pLayouts[i].Footprint.RowPitch,
					pLayouts[i].Footprint.RowPitch * pNumRows[i]
				};

				for (UINT z = 0; z < pLayouts[i].Footprint.Depth; ++z) {
					//Mipmap
					BYTE* pDestSlice = reinterpret_cast<BYTE*>(stCopyDestData.pData) + stCopyDestData.SlicePitch * z;
					const BYTE* pSrcSlice = reinterpret_cast<const BYTE*>(arSubResources[i].pData) + arSubResources[i].SlicePitch * z;

					for (UINT y = 0; y < pNumRows[i]; ++y) {
						//Rows
						memcpy(pDestSlice + stCopyDestData.RowPitch * y, pSrcSlice + arSubResources[i].RowPitch * y, (SIZE_T)pRowSizesInBytes[i]);
					}
				}
			}
			pITextureUploadSkybox->Unmap(0, nullptr);

			//�ڶ���Copy
			if (stDefaultResDesc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER) {
				//���������Buffer����ôһ���Ը��ƾͿ����ˣ���Ϊû���ж�����д�С��һ�µ����⣬Buffer����������1
				pICmdListDirect->CopyBufferRegion(
					pITextureSkybox.Get(),
					0,
					pITextureUploadSkybox.Get(),
					pLayouts[0].Offset,
					pLayouts[0].Footprint.Width
				);
			}
			else {
				for (UINT i = 0; i < nNumSubResources; ++i) {
					D3D12_TEXTURE_COPY_LOCATION stDstCopyLocation = {};
					stDstCopyLocation.pResource = pITextureSkybox.Get();
					stDstCopyLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
					stDstCopyLocation.SubresourceIndex = i;

					D3D12_TEXTURE_COPY_LOCATION stSrcCopyLocation = {};
					stSrcCopyLocation.pResource = pITextureUploadSkybox.Get();
					stSrcCopyLocation.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
					stSrcCopyLocation.PlacedFootprint = pLayouts[i];

					pICmdListDirect->CopyTextureRegion(&stDstCopyLocation, 0, 0, 0, &stSrcCopyLocation, nullptr);
				}
			}
			D3D12_RESOURCE_BARRIER stTransResBarrier = {};
			stTransResBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			stTransResBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			stTransResBarrier.Transition.pResource = pITextureSkybox.Get();
			stTransResBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
			stTransResBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
			stTransResBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

			pICmdListDirect->ResourceBarrier(1, &stTransResBarrier);

		}
		//�������б������ϴ��Ѹ����������ݵ�Ĭ�϶ѵ����ִ�в�ͬ���ȴ�������ɵڶ���Copy��������GPU�ϵĸ����������
		{
			//ע���ʱֱ�������б�û�а�PSO���������Ҳ�ǲ���ִ��3Dͼ������ģ����ǿ���ִ�и��������Ϊ�������治��Ҫʲô�����״̬����֮��Ĳ���
			D3D12_TEXTURE_COPY_LOCATION stDstCopyLocation = {};
			stDstCopyLocation.pResource = pITextureEarth.Get();
			stDstCopyLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
			stDstCopyLocation.SubresourceIndex = 0;

			D3D12_TEXTURE_COPY_LOCATION stSrcCopyLocation = {};
			stSrcCopyLocation.pResource = pITextureUploadEarth.Get();
			stSrcCopyLocation.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
			stSrcCopyLocation.PlacedFootprint = stTxtLayoutsEarth;

			pICmdListDirect->CopyTextureRegion(&stDstCopyLocation, 0, 0, 0, &stSrcCopyLocation, nullptr);

			//����һ����Դ���ϣ�ͬ����ȷ�ϸ��Ʋ������
			D3D12_RESOURCE_BARRIER stResBar = {};
			stResBar.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			stResBar.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			stResBar.Transition.pResource = pITextureEarth.Get();
			stResBar.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
			stResBar.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
			stResBar.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

			pICmdListDirect->ResourceBarrier(1, &stResBar);
		}
		//ִ�еڶ���Copy����ȴ�������Դ�ϴ����
		{
			//����Χ������ȴ���Ⱦ���
			THROW_IF_FAILED(pID3D12Device4->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&pIFence)));
			n64FenceValue = 1;
			//����һ��Eventͬ���������ڵȴ�Χ���¼�֪ͨ
			hFenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
			if (hFenceEvent == nullptr) {
				THROW_IF_FAILED(HRESULT_FROM_WIN32(GetLastError()));
			}

			//ִ�������б��ȴ�������Դ�ϴ����
			THROW_IF_FAILED(pICmdListDirect->Close());
			ID3D12CommandList* ppCommandLists[] = { pICmdListDirect.Get() };
			pICmdQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

			//�ȴ�������Դ�������
			const UINT64 n64CurrentFenceValue = n64FenceValue;
			THROW_IF_FAILED(pICmdQueue->Signal(pIFence.Get(), n64CurrentFenceValue));
			n64FenceValue++;
			THROW_IF_FAILED(pIFence->SetEventOnCompletion(n64CurrentFenceValue, hFenceEvent));
		}
		//�����������������
		{
			std::ifstream fin;
			char input;
			USES_CONVERSION;
			char pModuleFileName[MAX_PATH] = {};
			StringCchPrintfA(pModuleFileName, MAX_PATH, "%sAssets\\sphere.txt", T2A(pszAppPath));
			fin.open(pModuleFileName);

			if (fin.fail()) {
				throw COMException(E_FAIL);
			}
			fin.get(input);
			while (input != ':') {
				fin.get(input);
			}
			fin >> nSphereVertexCnt;
			nSphereIndexCnt = nSphereVertexCnt;
			fin.get(input);
			while (input != ':') {
				fin.get(input);
			}
			fin.get(input);
			fin.get(input);

			pstSphereVertices = (ST_VERTEX*)CALLOC(nSphereVertexCnt * sizeof(ST_VERTEX));
			pSphereIndices = (UINT*)CALLOC(nSphereIndexCnt * sizeof(UINT));

			for (UINT i = 0; i < nSphereVertexCnt; ++i) {
				fin >> pstSphereVertices[i].m_v4Position.x >> pstSphereVertices[i].m_v4Position.y >> pstSphereVertices[i].m_v4Position.z;
				pstSphereVertices[i].m_v4Position.w = 1.0f;
				fin >> pstSphereVertices[i].m_vTex.x >> pstSphereVertices[i].m_vTex.y;
				fin >> pstSphereVertices[i].m_vNor.x >> pstSphereVertices[i].m_vNor.y >> pstSphereVertices[i].m_vNor.z;

				pSphereIndices[i] = i;
			}
		}
		//ʹ�ö�λ��ʽ�������㻺�塢��������ͳ������壬ʹ�����ϴ��������ݻ�����ͬ��һ���ϴ���
		{
			UINT64 n64BufferOffset = ALIGN_UP(n64szUploadBufEarth, D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT);

			D3D12_RESOURCE_DESC stBufResDesc = {};
			stBufResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
			stBufResDesc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
			stBufResDesc.Width = nSphereVertexCnt * sizeof(ST_VERTEX);
			stBufResDesc.Height = 1;
			stBufResDesc.DepthOrArraySize = 1;
			stBufResDesc.MipLevels = 1;
			stBufResDesc.Format = DXGI_FORMAT_UNKNOWN;
			stBufResDesc.SampleDesc.Count = 1;
			stBufResDesc.SampleDesc.Quality = 0;
			stBufResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
			stBufResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

			THROW_IF_FAILED(pID3D12Device4->CreatePlacedResource(
				pIUploadHeapEarth.Get(),
				n64BufferOffset,
				&stBufResDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&pIVBEarth)));

			//�����ݴ������㻺����
			UINT8* pVertexDataBegin = nullptr;
			D3D12_RANGE stReadRange = { 0, 0 };
			THROW_IF_FAILED(pIVBEarth->Map(0, &stReadRange, reinterpret_cast<void**>(&pVertexDataBegin)));
			memcpy(pVertexDataBegin, pstSphereVertices, nSphereVertexCnt * sizeof(ST_VERTEX));

			pIVBEarth->Unmap(0, nullptr);

			SAFE_FREE(pstSphereVertices);

			//���ö��㻺����ͼ
			stVBVEarth.BufferLocation = pIVBEarth->GetGPUVirtualAddress();
			stVBVEarth.StrideInBytes = sizeof(ST_VERTEX);
			stVBVEarth.SizeInBytes = nSphereVertexCnt * sizeof(ST_VERTEX);

			//����߽�������ȷ��ƫ��λ��
			n64BufferOffset = ALIGN_UP(n64BufferOffset + nSphereVertexCnt * sizeof(ST_VERTEX), D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT);

			stBufResDesc.Width = nSphereIndexCnt * sizeof(UINT);

			THROW_IF_FAILED(pID3D12Device4->CreatePlacedResource(
				pIUploadHeapEarth.Get(),
				n64BufferOffset,
				&stBufResDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&pIIBEarth)
			));

			//�����ݴ�������������
			UINT8* pIndexDataBegin = nullptr;
			THROW_IF_FAILED(pIIBEarth->Map(0, &stReadRange, reinterpret_cast<void**>(&pIndexDataBegin)));
			memcpy(pIndexDataBegin, pSphereIndices, nSphereIndexCnt * sizeof(UINT));
			pIIBEarth->Unmap(0, nullptr);

			SAFE_FREE(pSphereIndices);

			//��������������ͼ
			stIBVEarth.BufferLocation = pIIBEarth->GetGPUVirtualAddress();
			stIBVEarth.Format = DXGI_FORMAT_R32_UINT;
			stIBVEarth.SizeInBytes = nSphereIndexCnt * sizeof(UINT);

			//������������
			n64BufferOffset = ALIGN_UP(n64BufferOffset + nSphereIndexCnt * sizeof(UINT), D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT);

			stBufResDesc.Width = szMVPBuf;

			//�����������壬ע�⻺��ߴ�����Ϊ256�߽�����С
			THROW_IF_FAILED(pID3D12Device4->CreatePlacedResource(
				pIUploadHeapEarth.Get(),
				n64BufferOffset,
				&stBufResDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&pICBUploadEarth)
			));
			//Map��Ͳ���Unmap�ˣ�ֱ�Ӹ������ݽ�ȥ������ÿ֡������map-copy-unmap�˷�ʱ����
			THROW_IF_FAILED(pICBUploadEarth->Map(0, nullptr, reinterpret_cast<void**>(&pMVPBufEarth)));
		}
		//������պ�(Զƽ����)
		{
			float fHighW = -1.0f - (1.0f / (float)iWndWidth);
			float fHighH = -1.0f - (1.0f / (float)iWndHeight);
			float fLowW = 1.0f + (1.0f / (float)iWndWidth);
			float fLowH = 1.0f + (1.0f / (float)iWndHeight);

			ST_SKYBOX_VERTEX stSkyboxVertices[4] = {};

			stSkyboxVertices[0].m_v4Position = XMFLOAT4(fLowW, fLowH, 1.0f, 1.0f);
			stSkyboxVertices[1].m_v4Position = XMFLOAT4(fLowW, fHighH, 1.0f, 1.0f);
			stSkyboxVertices[2].m_v4Position = XMFLOAT4(fHighW, fLowH, 1.0f, 1.0f);
			stSkyboxVertices[3].m_v4Position = XMFLOAT4(fHighW, fHighH, 1.0f, 1.0f);

			nSkyboxVertexCnt = 4;

			//������պе�����
			D3D12_RESOURCE_DESC stBufResDesc = {};
			stBufResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
			stBufResDesc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
			stBufResDesc.Width = nSkyboxVertexCnt * sizeof(ST_SKYBOX_VERTEX);
			stBufResDesc.Height = 1;
			stBufResDesc.DepthOrArraySize = 1;
			stBufResDesc.MipLevels = 1;
			stBufResDesc.Format = DXGI_FORMAT_UNKNOWN;
			stBufResDesc.SampleDesc.Count = 1;
			stBufResDesc.SampleDesc.Quality = 0;
			stBufResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
			stBufResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

			UINT64 n64BufferOffset = ALIGN_UP(n64szUploadBufSkybox, D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT);

			THROW_IF_FAILED(pID3D12Device4->CreatePlacedResource(
				pIUploadHeapSkybox.Get(),
				n64BufferOffset,
				&stBufResDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&pIVBSkybox)
			));

			//�����ݴ������㻺����
			ST_SKYBOX_VERTEX* pVertexDataBegin = nullptr;
			THROW_IF_FAILED(pIVBSkybox->Map(0, nullptr, reinterpret_cast<void**>(&pVertexDataBegin)));
			memcpy(pVertexDataBegin, stSkyboxVertices, nSkyboxVertexCnt * sizeof(ST_SKYBOX_VERTEX));
			pIVBSkybox->Unmap(0, nullptr);

			//���ö��㻺����ͼ
			stVBVSkybox.BufferLocation = pIVBSkybox->GetGPUVirtualAddress();
			stVBVSkybox.StrideInBytes = sizeof(ST_SKYBOX_VERTEX);
			stVBVSkybox.SizeInBytes = nSkyboxVertexCnt * sizeof(ST_SKYBOX_VERTEX);

			//����߽�������ȷ��ƫ��λ��
			n64BufferOffset = ALIGN_UP(n64BufferOffset + nSkyboxVertexCnt * sizeof(ST_SKYBOX_VERTEX), D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT);

			//������������
			stBufResDesc.Width = szMVPBuf;
			THROW_IF_FAILED(pID3D12Device4->CreatePlacedResource(
				pIUploadHeapSkybox.Get(),
				n64BufferOffset,
				&stBufResDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&pICBUploadSkybox)
			));
			//Map��Ͳ���Unmap�ˣ�ֱ�Ӹ������ݽ�ȥ������ÿ֡������map-copy-unmap�˷�ʱ����
			THROW_IF_FAILED(pICBUploadSkybox->Map(0, nullptr, reinterpret_cast<void**>(&pMVPBufSkybox)));
		}
		//����SRV������
		{
			D3D12_SHADER_RESOURCE_VIEW_DESC stSRVDesc = {};
			stSRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			stSRVDesc.Format = emTxtFmtEarth;
			stSRVDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			stSRVDesc.Texture2D.MipLevels = 1;
			pID3D12Device4->CreateShaderResourceView(pITextureEarth.Get(), &stSRVDesc, pISRVHpEarth->GetCPUDescriptorHandleForHeapStart());

			D3D12_RESOURCE_DESC stDescSkybox = pITextureSkybox->GetDesc();
			stSRVDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
			stSRVDesc.Format = stDescSkybox.Format;
			stSRVDesc.TextureCube.MipLevels = stDescSkybox.MipLevels;
			pID3D12Device4->CreateShaderResourceView(pITextureSkybox.Get(), &stSRVDesc, pISRVHpSkybox->GetCPUDescriptorHandleForHeapStart());
		}
		//����CBV������
		{
			D3D12_CONSTANT_BUFFER_VIEW_DESC stCBVDesc = {};
			stCBVDesc.BufferLocation = pICBUploadEarth->GetGPUVirtualAddress();
			stCBVDesc.SizeInBytes = static_cast<UINT>(szMVPBuf);

			D3D12_CPU_DESCRIPTOR_HANDLE stSRVCBVHandle = pISRVHpEarth->GetCPUDescriptorHandleForHeapStart();
			stSRVCBVHandle.ptr += nSRVDescriptorSize;

			pID3D12Device4->CreateConstantBufferView(&stCBVDesc, stSRVCBVHandle);

			stCBVDesc.BufferLocation = pICBUploadSkybox->GetGPUVirtualAddress();
			stCBVDesc.SizeInBytes = static_cast<UINT>(szMVPBuf);

			stSRVCBVHandle = pISRVHpSkybox->GetCPUDescriptorHandleForHeapStart();
			stSRVCBVHandle.ptr += nSRVDescriptorSize;

			pID3D12Device4->CreateConstantBufferView(&stCBVDesc, stSRVCBVHandle);
		}
		//�����������Ѻ͸��ֲ�����
		{
			D3D12_CPU_DESCRIPTOR_HANDLE hSamplerHeap = pISampleHpEarth->GetCPUDescriptorHandleForHeapStart();

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

			//Skybox�Ĳ�����
			stSamplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;

			stSamplerDesc.MinLOD = 0;
			stSamplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
			stSamplerDesc.MipLODBias = 0.0f;
			stSamplerDesc.MaxAnisotropy = 1;
			stSamplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
			stSamplerDesc.BorderColor[0] = 0.0f;
			stSamplerDesc.BorderColor[1] = 0.0f;
			stSamplerDesc.BorderColor[2] = 0.0f;
			stSamplerDesc.BorderColor[3] = 0.0f;
			stSamplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			stSamplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			stSamplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;

			pID3D12Device4->CreateSampler(&stSamplerDesc, pISampleHpSkybox->GetCPUDescriptorHandleForHeapStart());
		}
		//ʹ���������¼�̻�������
		{
			//����������
			pIBundlesEarth->SetGraphicsRootSignature(pIRootSignature.Get());
			pIBundlesEarth->SetPipelineState(pIPSOEarth.Get());

			ID3D12DescriptorHeap* ppHeapsEarth[] = { pISRVHpEarth.Get(),pISampleHpEarth.Get() };
			pIBundlesEarth->SetDescriptorHeaps(_countof(ppHeapsEarth), ppHeapsEarth);
			//����SRV
			pIBundlesEarth->SetGraphicsRootDescriptorTable(0, pISRVHpEarth->GetGPUDescriptorHandleForHeapStart());

			D3D12_GPU_DESCRIPTOR_HANDLE stGPUCBVhandleEarth = pISRVHpEarth->GetGPUDescriptorHandleForHeapStart();
			stGPUCBVhandleEarth.ptr += nSRVDescriptorSize;
			//����CBV
			pIBundlesEarth->SetGraphicsRootDescriptorTable(1, stGPUCBVhandleEarth);

			D3D12_GPU_DESCRIPTOR_HANDLE hGPUSamplerEarth = pISampleHpEarth->GetGPUDescriptorHandleForHeapStart();
			hGPUSamplerEarth.ptr += g_nCurrentSamplerNo * nSamplerDescriptorSize;

			//����Sample
			pIBundlesEarth->SetGraphicsRootDescriptorTable(2, hGPUSamplerEarth);
			//ע������ʹ�õ���Ⱦ�ַ����������б�Ҳ����ͨ����Mesh����
			pIBundlesEarth->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			pIBundlesEarth->IASetVertexBuffers(0, 1, &stVBVEarth);
			pIBundlesEarth->IASetIndexBuffer(&stIBVEarth);

			//Draw Call
			pIBundlesEarth->DrawIndexedInstanced(nSphereIndexCnt, 1, 0, 0, 0);
			pIBundlesEarth->Close();

			//Skybox�������
			pIBundlesSkybox->SetGraphicsRootSignature(pIRootSignature.Get());
			pIBundlesSkybox->SetPipelineState(pIPSOSkybox.Get());
			ID3D12DescriptorHeap* ppHeapsSkybox[] = { pISRVHpSkybox.Get(),pISampleHpSkybox.Get() };
			pIBundlesSkybox->SetDescriptorHeaps(_countof(ppHeapsSkybox), ppHeapsSkybox);

			//����SRV
			pIBundlesSkybox->SetGraphicsRootDescriptorTable(0, pISRVHpSkybox->GetGPUDescriptorHandleForHeapStart());

			D3D12_GPU_DESCRIPTOR_HANDLE stGPUCBVHandleSkybox = pISRVHpSkybox->GetGPUDescriptorHandleForHeapStart();
			stGPUCBVHandleSkybox.ptr += nSRVDescriptorSize;
			//����CBV
			pIBundlesSkybox->SetGraphicsRootDescriptorTable(1, stGPUCBVHandleSkybox);
			pIBundlesSkybox->SetGraphicsRootDescriptorTable(2, pISampleHpSkybox->GetGPUDescriptorHandleForHeapStart());
			pIBundlesSkybox->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			pIBundlesSkybox->IASetVertexBuffers(0, 1, &stVBVSkybox);

			//Draw Call
			pIBundlesSkybox->DrawInstanced(4, 1, 0, 0);
			pIBundlesSkybox->Close();
		}
		//�����Դ���Ͻṹ
		D3D12_RESOURCE_BARRIER stBeginResBarrier = {};
		stBeginResBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		stBeginResBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		stBeginResBarrier.Transition.pResource = pIARenderTargets[nCurrentFrameIndex].Get();
		stBeginResBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		stBeginResBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		stBeginResBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

		D3D12_RESOURCE_BARRIER stEndResBarrier = {};
		stEndResBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		stEndResBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		stEndResBarrier.Transition.pResource = pIARenderTargets[nCurrentFrameIndex].Get();
		stEndResBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		stEndResBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		stEndResBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

		//��¼֡��ʼʱ��͵�ǰʱ�䣬��ѭ������Ϊ��
		ULONGLONG n64tmFrameStart = ::GetTickCount64();
		ULONGLONG n64timeCurrent = n64tmFrameStart;
		//������ת�Ƕ���Ҫ�ı���
		double dModelRotationYAngle = 0.0f;

		DWORD dwRet = 0;
		BOOL bExit = FALSE;

		ShowWindow(hWnd, nCmdShow);
		UpdateWindow(hWnd);
		//������Ϣѭ��
		while (!bExit) {
			dwRet = ::MsgWaitForMultipleObjects(1, &hFenceEvent, FALSE, INFINITE, QS_ALLINPUT);
			switch (dwRet - WAIT_OBJECT_0) {
			case 0:
			{
				//��ʼ��¼����
				//׼��һ��MVP������������ת����
				{
					n64timeCurrent = ::GetTickCount64();
					//������ת�ĽǶȣ���ת�Ƕ�(����) = ʱ��(��) * ���ٶ�(����/��)
					//�����������൱�ھ�����Ϸ��Ϣѭ���е�OnUpdate��������Ҫ��������
					dModelRotationYAngle += ((n64timeCurrent - n64tmFrameStart) / 1000.0f) * g_fPalstance;

					n64tmFrameStart = n64timeCurrent;

					//�����ת�Ƕȴ���2PI����ȡģ
					if (dModelRotationYAngle > XM_2PI) {
						dModelRotationYAngle = fmod(dModelRotationYAngle, XM_2PI);
					}

					XMMATRIX xmView = XMMatrixLookAtLH(XMLoadFloat3(&g_f3EyePos), XMLoadFloat3(&g_f3LookAt), XMLoadFloat3(&g_f3HeapUp));
					XMMATRIX xmProj = XMMatrixPerspectiveFovLH(XM_PIDIV4, (FLOAT)iWndWidth / (FLOAT)iWndHeight, 1.0f, 2000.0f);

					XMMATRIX xmSkybox = xmView;//��Ϊ��պ��ǲ����ģ�����M=I
					xmSkybox = XMMatrixMultiply(xmSkybox, xmProj);//�任�����ҳ˽�ϣ������(M)VP
					xmSkybox = XMMatrixInverse(nullptr, xmSkybox);//ȡ����󣬽��ü��ռ��е�����任�ع۲��߿ռ�

					//����Skybox��MVP����
					XMStoreFloat4x4(&pMVPBufSkybox->m_MVP, xmSkybox);

					//����VP
					XMMATRIX xmVP = XMMatrixMultiply(xmView, xmProj);

					pMVPBufSkybox->m_v4EyePos = XMFLOAT4(g_f3EyePos.x, g_f3EyePos.y, g_f3EyePos.z, 1.0f);

					//M���������ǷŴ����ת
					XMMATRIX xmRot = XMMatrixMultiply(XMMatrixScaling(fSphereSize, fSphereSize, fSphereSize), XMMatrixRotationY(static_cast<float>(dModelRotationYAngle)));

					//���������MVP����
					xmVP = XMMatrixMultiply(xmRot, xmVP);

					XMStoreFloat4x4(&pMVPBufEarth->m_MVP, xmVP);
				}
				//Reset����������������б�����ָ�������������PSO����
				THROW_IF_FAILED(pICmdAllocDirect->Reset());
				THROW_IF_FAILED(pICmdListDirect->Reset(pICmdAllocDirect.Get(), pIPSOEarth.Get()));

				//��ȡ�µĺ󻺳���ţ���ΪPresent�������ʱ�󻺳����ž͸�����
				nCurrentFrameIndex = pISwapChain3->GetCurrentBackBufferIndex();

				//ͨ����Դ�����ж��󻺳��Ѿ��л���Ͽ��Կ�ʼ��Ⱦ��
				stBeginResBarrier.Transition.pResource = pIARenderTargets[nCurrentFrameIndex].Get();
				pICmdListDirect->ResourceBarrier(1, &stBeginResBarrier);

				//ƫ��������ָ�뵽ָ��֡������ͼλ��
				D3D12_CPU_DESCRIPTOR_HANDLE stRTVHandle = pIRTVHeap->GetCPUDescriptorHandleForHeapStart();
				stRTVHandle.ptr += (nCurrentFrameIndex * nRTVDescriptorSize);
				D3D12_CPU_DESCRIPTOR_HANDLE stDSVHandle = pIDSVHeap->GetCPUDescriptorHandleForHeapStart();
				//������ȾĿ��
				pICmdListDirect->OMSetRenderTargets(1, &stRTVHandle, FALSE, &stDSVHandle);//ע������RTV��������DSV������

				pICmdListDirect->RSSetViewports(1, &stViewPort);
				pICmdListDirect->RSSetScissorRects(1, &stScissorRect);

				//������¼�������ʼ��һ֡����Ⱦ
				pICmdListDirect->ClearRenderTargetView(stRTVHandle, faClearColor, 0, nullptr);
				pICmdListDirect->ClearDepthStencilView(pIDSVHeap->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

				//ִ��Skybox�������
				ID3D12DescriptorHeap* ppHeapsSkybox[] = { pISRVHpSkybox.Get(),pISampleHpSkybox.Get() };
				pICmdListDirect->SetDescriptorHeaps(_countof(ppHeapsSkybox), ppHeapsSkybox);
				pICmdListDirect->ExecuteBundle(pIBundlesSkybox.Get());

				//ִ������������
				ID3D12DescriptorHeap* ppHeapsEarth[] = { pISRVHpEarth.Get(),pISampleHpEarth.Get() };
				pICmdListDirect->SetDescriptorHeaps(_countof(ppHeapsEarth), ppHeapsEarth);
				pICmdListDirect->ExecuteBundle(pIBundlesEarth.Get());

				//��Դ���ϣ�����ȷ����Ⱦ�Ѿ����������ύ����ȥ��ʾ��
				stEndResBarrier.Transition.pResource = pIARenderTargets[nCurrentFrameIndex].Get();
				pICmdListDirect->ResourceBarrier(1, &stEndResBarrier);
				//�ر������б����Կ�ʼִ����
				THROW_IF_FAILED(pICmdListDirect->Close());
				//ִ�������б�
				ID3D12CommandList* ppCommandLists[] = { pICmdListDirect.Get() };
				pICmdQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
				//�ύ����
				THROW_IF_FAILED(pISwapChain3->Present(1, 0));
				//��ʼͬ��GPU��CPU��ִ�У��߼�¼Χ�����ֵ
				const UINT64 n64CurrentFenceValue = n64FenceValue;
				THROW_IF_FAILED(pICmdQueue->Signal(pIFence.Get(), n64CurrentFenceValue));
				n64FenceValue++;
				THROW_IF_FAILED(pIFence->SetEventOnCompletion(n64CurrentFenceValue, hFenceEvent));
			}
			break;
			case 1:
			{//������Ϣ
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
			//���ո���л���ͬ�Ĳ�������Ч��
			++g_nCurrentSamplerNo;
			g_nCurrentSamplerNo %= g_nSampleMaxCnt;
		}
		//�����û�����任
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

		XMFLOAT3 move(0, 0, 0);
		float fMoveSpeed = 2.0f;
		float fTurnSpeed = XM_PIDIV2 * 0.005f;

		if ('w' == n16KeyCode || 'W' == n16KeyCode)
		{
			move.z -= 1.0f;
		}

		if ('s' == n16KeyCode || 'S' == n16KeyCode)
		{
			move.z += 1.0f;
		}

		if ('d' == n16KeyCode || 'D' == n16KeyCode)
		{
			move.x += 1.0f;
		}

		if ('a' == n16KeyCode || 'A' == n16KeyCode)
		{
			move.x -= 1.0f;
		}

		if (fabs(move.x) > 0.1f && fabs(move.z) > 0.1f)
		{
			XMVECTOR vector = XMVector3Normalize(XMLoadFloat3(&move));
			move.x = XMVectorGetX(vector);
			move.z = XMVectorGetZ(vector);
		}

		if (VK_UP == n16KeyCode)
		{
			g_fPitch += fTurnSpeed;
		}

		if (VK_DOWN == n16KeyCode)
		{
			g_fPitch -= fTurnSpeed;
		}

		if (VK_RIGHT == n16KeyCode)
		{
			g_fYaw -= fTurnSpeed;
		}

		if (VK_LEFT == n16KeyCode)
		{
			g_fYaw += fTurnSpeed;
		}

		// Prevent looking too far up or down.
		g_fPitch = min(g_fPitch, XM_PIDIV4);
		g_fPitch = max(-XM_PIDIV4, g_fPitch);

		// Move the camera in model space.
		float x = move.x * -cosf(g_fYaw) - move.z * sinf(g_fYaw);
		float z = move.x * sinf(g_fYaw) - move.z * cosf(g_fYaw);
		g_f3EyePos.x += x * fMoveSpeed;
		g_f3EyePos.z += z * fMoveSpeed;

		// Determine the look direction.
		float r = cosf(g_fPitch);
		g_f3LookAt.x = r * sinf(g_fYaw);
		g_f3LookAt.y = sinf(g_fPitch);
		g_f3LookAt.z = r * cosf(g_fYaw);

		if (VK_TAB == n16KeyCode)
		{//��Tab����ԭ�����λ��
			g_f3EyePos = XMFLOAT3(0.0f, 0.0f, -10.0f); //�۾�λ��
			g_f3LookAt = XMFLOAT3(0.0f, 0.0f, 0.0f);    //�۾�������λ��
			g_f3HeapUp = XMFLOAT3(0.0f, 1.0f, 0.0f);    //ͷ�����Ϸ�λ��
		}
		break;
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}