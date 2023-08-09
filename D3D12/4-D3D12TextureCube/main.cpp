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
//��ȡ������
#define UPPER_DIV(A,B) ((UINT)(((A)+((B)-1))/(B)))
//���ϱ߽����
#define ALIGN_UP(A, B) ((UINT)(((A) + ((B) - 1)) & ~((B) - 1)))


#include <exception>
#include <cstring>
#include <sstream>
#include <strsafe.h>

//�쳣��
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

//WIC�ӿ�ͷ�ļ�
#include <wincodec.h>

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

struct ST_VERTEX {
	XMFLOAT4 m_v4Position;//Position����λ��
	XMFLOAT2 m_vTex;//Texcoord��������
	XMFLOAT3 m_vNor;//Normal����
};

struct ST_FRAME_MVP_BUFFER {
	XMFLOAT4X4 m_MVP;//MVP����
};

UINT g_nCurrentSamplerNo = 0; //��ǰʹ�õĲ���������
UINT g_nSampleMaxCnt = 5; //����������͵Ĳ�����

//��ʼ��Ĭ���������λ��
XMVECTOR g_v4EyePos = XMVectorSet(0.0f, 2.0f, -15.0f, 0.0f);//�۾�λ��
XMVECTOR g_v4LookAt = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);//�����λ��
XMVECTOR g_v4UpDir = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);//���ϵķ���

double g_fPalstance = 10.0f * XM_PI / 180.0f;//������ת�Ľ��ٶȣ���λ: ����/��

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
	::CoInitialize(nullptr);  //for WIC & COM

	const UINT nFrameBackBufCount = 3u;//����������

	int iWidth = 1024;//���ڿ��
	int iHeight = 768;//���ڸ߶�

	UINT nFrameIndex = 0;//��ǰ֡����

	DXGI_FORMAT emRenderTargetFormat = DXGI_FORMAT_B8G8R8A8_UNORM;//��ȾĿ���ʽ
	const float faClearColor[] = { 0.0f,0.0f,0.0f,0.0f };//����ɫ

	UINT nDXGIFactoryFlags = 0u;
	UINT nRTVDescriptorSize = 0u;//RTV��������С
	UINT nSRVDescriptorSize = 0u;//SRV��������С

	HWND hWnd = nullptr;//���ھ��
	MSG msg = {};//��Ϣ
	TCHAR pszAppPath[MAX_PATH] = {};//Ӧ�ó���·��

	float fBoxSize = 3.0f;
	float fTCMax = 3.0f;

	ST_FRAME_MVP_BUFFER* pMVPBuffer = nullptr;
	SIZE_T szMVPBuffer = ALIGN_UP(sizeof(ST_FRAME_MVP_BUFFER), 256);

	D3D12_VERTEX_BUFFER_VIEW stVertexBufferView = {};//���㻺������ͼ�������������㻺������λ�úʹ�С
	D3D12_INDEX_BUFFER_VIEW stIndexBufferView = {};//������������ͼ����������������������λ�úʹ�С

	UINT64 n64FenceValue = 0ui64;//Χ��ֵ
	HANDLE hFenceEvent = nullptr;//Χ���¼����

	UINT nTextureW = 0u;//������
	UINT nTextureH = 0u;//����߶�
	UINT nBPP = 0u;//Bits Per Pixel
	UINT nPicRowPitch = 0;//ͼƬ�о�(��λ:�ֽ�)
	UINT64 n64UploadBufferSize = 0;//�ϴ��ѻ�������С
	DXGI_FORMAT stTextureFormat = DXGI_FORMAT_UNKNOWN;//�����ʽ
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT stTxtLayouts = {};//������
	D3D12_RESOURCE_DESC stTextureDesc = {};//������Ϣ�����ṹ��
	D3D12_RESOURCE_DESC stDestDesc = {};//Ŀ����Դ�����ṹ��

	UINT nSamplerDescriptorSize = 0;//��������������С

	D3D12_VIEWPORT stViewPort = { 0.0f, 0.0f, static_cast<float>(iWidth), static_cast<float>(iHeight), D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };//�ӿ����Խṹ��
	D3D12_RECT stScissorRect = { 0, 0, static_cast<LONG>(iWidth), static_cast<LONG>(iHeight) };//�ü����νṹ��

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

	ComPtr<ID3D12Heap> pITextureHeap;//�����Ĭ�϶�
	ComPtr<ID3D12Heap> pIUploadHeap;//�ϴ���
	ComPtr<ID3D12Resource> pITexture;//������Դ
	ComPtr<ID3D12Resource> pITextureUpload; //�����ϴ��������ݵĻ�����Դ
	ComPtr<ID3D12Resource> pICBVUpload;
	ComPtr<ID3D12Resource> pIVertexBuffer;//���㻺����
	ComPtr<ID3D12Resource> pIIndexBuffer;

	ComPtr<ID3D12DescriptorHeap> pISRVHeap;
	ComPtr<ID3D12DescriptorHeap> pISamplerDescriptorHeap;//��������������

	ComPtr<ID3D12Fence> pIFence;
	ComPtr<ID3DBlob> pIBlobVertexShader;//������ɫ��
	ComPtr<ID3DBlob> pIBlobPixelShader;//������ɫ��
	ComPtr<ID3D12RootSignature> pIRootSignature;
	ComPtr<ID3D12PipelineState> pIPipelineState;

	//WIC�ӿ�
	ComPtr<IWICImagingFactory> pIWICFactory;//WIC�����ӿ�
	ComPtr<IWICBitmapDecoder> pIWICDecoder;//WIC�������ӿ�
	ComPtr<IWICBitmapFrameDecode> pIWICFrame;//WICλͼ֡����ӿ�
	ComPtr<IWICBitmapSource> pIBMP;//λͼ��ʽ��ͼƬ���ݶ���ӿ�

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
			RECT rtWnd = { 0,0,iWidth,iHeight };
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
			//ʹ�ô�COM��ʽ����WIC�೧����Ҳ�ǵ���WIC��һ��Ҫ��������
			THROW_IF_FAILED(CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pIWICFactory)));
			//ʹ��WIC�೧����ӿڼ�������ͼƬ�����õ�һ��WIC����������ӿڣ�ͼƬ��Ϣ��������ӿڴ���Ķ�������
			WCHAR pszTexcuteFileName[MAX_PATH] = {};
			StringCchPrintfW(pszTexcuteFileName, MAX_PATH, _T("%sAssets\\bear.jpg"), pszAppPath);

			THROW_IF_FAILED(pIWICFactory->CreateDecoderFromFilename(
				pszTexcuteFileName,//�ļ���
				nullptr,//��ָ����������ʹ��Ĭ��
				GENERIC_READ,//��Ȩ��
				WICDecodeMetadataCacheOnDemand,//����Ҫ�򻺳�����
				&pIWICDecoder));//����������

			//��ȡ��һ֡ͼƬ(��ΪGIF�ȸ�ʽ�ļ����ܻ��ж�֡ͼƬ�������ĸ�ʽһ��ֻ��һ֡ͼƬ)��ʵ�ʽ���������������λͼ��ʽ����
			THROW_IF_FAILED(pIWICDecoder->GetFrame(0, &pIWICFrame));

			WICPixelFormatGUID wpf = {};
			//��ȡWICͼƬ��ʽ
			THROW_IF_FAILED(pIWICFrame->GetPixelFormat(&wpf));
			GUID tgFormat = {};

			//ת��ΪDXGI�ĵȼ۸�ʽ
			if (GetTargetPixelFormat(&wpf, &tgFormat)) {
				stTextureFormat = GetDXGIFormatFromPixelFormat(&tgFormat);
			}

			if (stTextureFormat == DXGI_FORMAT_UNKNOWN) {
				throw COMException(S_FALSE);
			}
			if (!InlineIsEqualGUID(wpf, tgFormat)) {
				//���WICͼƬ��ʽ��DXGI�ȼ۸�ʽ��һ�£�����Ҫ���и�ʽת��
				//����ͼƬ��ʽת����
				ComPtr<IWICFormatConverter> pIConverter;
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
				THROW_IF_FAILED(pIConverter.As(&pIBMP));

			}
			else {
				//ͼƬ���ݸ�ʽ����Ҫת����ֱ�ӻ�ȡ��λͼ����Դ�ӿ�
				THROW_IF_FAILED(pIWICFrame.As(&pIBMP));
			}

			//��ȡͼƬ��С
			THROW_IF_FAILED(pIBMP->GetSize(&nTextureW, &nTextureH));
			//��ȡͼƬ���ص�λ��С��BPP(bits per pixel)��Ϣ�����ڼ���ͼƬ�����ݵ���ʵ��С(��λ:�ֽ�)
			ComPtr<IWICComponentInfo> pIWICmntinfo;
			THROW_IF_FAILED(pIWICFactory->CreateComponentInfo(tgFormat, pIWICmntinfo.GetAddressOf()));

			WICComponentType type;
			THROW_IF_FAILED(pIWICmntinfo->GetComponentType(&type));

			if (type != WICPixelFormat) {
				throw COMException(S_FALSE);
			}

			ComPtr<IWICPixelFormatInfo> pIWICPixelinfo;
			THROW_IF_FAILED(pIWICmntinfo.As(&pIWICPixelinfo));
			//���BPP
			THROW_IF_FAILED(pIWICPixelinfo->GetBitsPerPixel(&nBPP));
			//����ͼƬʵ�ʵ��д�С(��λ:�ֽ�)��ʹ��һ����ȡ����������A+B-1��/B
			nPicRowPitch = UPPER_DIV(uint64_t(nTextureW) * uint64_t(nBPP), 8);
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
			THROW_IF_FAILED(pID3D12Device4->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&pICommandQueue)));
		}
		//���������б��������ͼ�������б�
		{
			THROW_IF_FAILED(pID3D12Device4->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&pICommandAllocator)));
			THROW_IF_FAILED(pID3D12Device4->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, pICommandAllocator.Get(), nullptr, IID_PPV_ARGS(&pICommandList)));
		}
		//����������
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
			//�ر�alt+enter�л�ȫ��
			THROW_IF_FAILED(pIDXGIFactory5->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER));

			THROW_IF_FAILED(pISwapChain1.As(&pISwapChain3));//ʹ��As�������Ͱ汾��SwapChain�ӿ�ת��Ϊ�߰汾��SwapChain�ӿ�
			nFrameIndex = pISwapChain3->GetCurrentBackBufferIndex();//���ø߰汾�ӿڵ�GetCurrentBackBufferIndex������ȡ��ǰ��̨����������
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
		//����SRV��CBV��Sample��������
		{
			//��������ͼ��������CBV����������һ������������
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
		//������ǩ��
		{
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
			UINT compileFlags = 0;
#if defined(_DEBUG)
			compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif // defined(_DEBUG)

			//����Ϊ�о�����ʽ
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

			//������Ⱦ����
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
		//���������Ĭ�϶�
		{
			D3D12_HEAP_DESC stTextureHeapDesc = {};
			//Ϊ��ָ������ͼƬ����������С�Ŀռ䣬ʵ��Ӧ����Ҳ��Ҫ�ۺϿ��Ƿ���ѵĴ�С�Ա�������ö�
			stTextureHeapDesc.SizeInBytes = ALIGN_UP(2 * nPicRowPitch * nTextureH, D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT);
			//ָ���ѵĶ��뷽ʽ������ʹ����Ĭ�ϵ�64K�߽���䣬��Ϊ������ʱ����ҪMSAA֧��
			stTextureHeapDesc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
			stTextureHeapDesc.Properties.Type = D3D12_HEAP_TYPE_DEFAULT;
			stTextureHeapDesc.Properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			stTextureHeapDesc.Properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
			//�ܾ���ȾĿ�������ܾ����ģ������ʵ��ֻ�������ڷ���ͨ����
			stTextureHeapDesc.Flags = D3D12_HEAP_FLAG_DENY_RT_DS_TEXTURES | D3D12_HEAP_FLAG_DENY_BUFFERS;

			THROW_IF_FAILED(pID3D12Device4->CreateHeap(&stTextureHeapDesc, IID_PPV_ARGS(&pITextureHeap)));
		}
		//����2D����
		{
			//����ͼƬ��Ϣ���2D������Դ����Ϣ�ṹ��
			stTextureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
			stTextureDesc.MipLevels = 1;
			stTextureDesc.Format = stTextureFormat;
			stTextureDesc.Width = nTextureW;
			stTextureDesc.Height = nTextureH;
			stTextureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
			stTextureDesc.DepthOrArraySize = 1;
			stTextureDesc.SampleDesc.Count = 1;
			stTextureDesc.SampleDesc.Quality = 0;

			//ʹ�ö�λ��ʽ���������������ڲ������ڴ洢������ͷŵ�ʵ�ʲ��������ܽϸ�
			THROW_IF_FAILED(pID3D12Device4->CreatePlacedResource(
				pITextureHeap.Get(),
				0,
				&stTextureDesc,
				D3D12_RESOURCE_STATE_COPY_DEST,
				nullptr,
				IID_PPV_ARGS(&pITexture)
			));

			//��ȡ�ϴ��ѻ�����Դ�Ĵ�С������ߴ�ͨ������ʵ��ͼƬ�ĳߴ�
			n64UploadBufferSize = 0;
			D3D12_RESOURCE_DESC stCopyDstDesc = pITexture->GetDesc();
			pID3D12Device4->GetCopyableFootprints(&stCopyDstDesc, 0, 1, 0, nullptr, nullptr, nullptr, &n64UploadBufferSize);
		}
		//�����ϴ���
		{
			D3D12_HEAP_DESC stUploadHeapDesc = {};
			//�ߴ���Ȼ��ʵ���������ݴ�С��2����64K�߽����
			stUploadHeapDesc.SizeInBytes = ALIGN_UP(2 * n64UploadBufferSize, D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT);
			//�ϴ��ѿ϶���Buffer���ͣ����Բ�ָ�����䷽ʽ��Ĭ����64k�߽����
			stUploadHeapDesc.Alignment = 0;
			stUploadHeapDesc.Properties.Type = D3D12_HEAP_TYPE_UPLOAD;
			stUploadHeapDesc.Properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			stUploadHeapDesc.Properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
			//�ϴ����ǻ��壬���԰ڷ���������
			stUploadHeapDesc.Flags = D3D12_HEAP_FLAG_ALLOW_ONLY_BUFFERS;
			THROW_IF_FAILED(pID3D12Device4->CreateHeap(&stUploadHeapDesc, IID_PPV_ARGS(&pIUploadHeap)));
		}
		//ʹ�ö�λ��ʽ���������ϴ��������ݵĻ�����Դ
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
		//����ͼƬ���ϴ��ѣ�����ɵ�һ��Copy��������memcpy��֪������CPU��ɵ�
		{

			//������Դ�����С������ʵ��ͼƬ���ݴ洢���ڴ��С
			void* pbPicData = ::HeapAlloc(::GetProcessHeap(), HEAP_ZERO_MEMORY, n64UploadBufferSize);
			if (pbPicData == nullptr) {
				throw COMException(HRESULT_FROM_WIN32(GetLastError()));
			}
			//��ͼƬ��������
			THROW_IF_FAILED(pIBMP->CopyPixels(
				nullptr,
				nPicRowPitch,
				static_cast<UINT>(nPicRowPitch * nTextureH),//�������ͼƬ������ʵ�Ĵ�С�����ֵͨ��С�ڻ���Ĵ�С
				reinterpret_cast<BYTE*>(pbPicData)
			));

			//��ȡ���ϴ��ѿ����������ݵ�һЩ����ת���ߴ���Ϣ�����ڸ��ӵ�DDS�������Ƿǳ���Ҫ�Ĺ���
			UINT nNumSubresources = 1u;
			UINT nTextureRowNum = 0u;
			UINT64 n64RequiredSize = 0u;
			UINT64 n64TextureRowSizes = 0u;

			stDestDesc = pITexture->GetDesc();

			pID3D12Device4->GetCopyableFootprints(&stDestDesc, 0, nNumSubresources, 0, &stTxtLayouts, &nTextureRowNum, &n64TextureRowSizes, &n64RequiredSize);

			//��Ϊ�ϴ���ʵ�ʾ���CPU�������ݵ�GPU���н�,�������ǿ���ʹ����Ϥ��Map����������ӳ�䵽CPU�ڴ��ַ��,Ȼ�����ǰ��н����ݸ��Ƶ��ϴ�����,��Ҫע�����֮���԰��п�������ΪGPU��Դ���д�С,��ʵ��ͼƬ���д�С���в����,���ߵ��ڴ�߽����Ҫ���ǲ�һ����
			BYTE* pData = nullptr;
			THROW_IF_FAILED(pITextureUpload->Map(0, nullptr, reinterpret_cast<void**>(&pData)));

			BYTE* pDestSlice = reinterpret_cast<BYTE*>(pData) + stTxtLayouts.Offset;
			const BYTE* pSrcSlice = reinterpret_cast<const BYTE*>(pbPicData);
			for (UINT y = 0; y < nTextureRowNum; ++y) {
				memcpy(pDestSlice + static_cast<SIZE_T>(stTxtLayouts.Footprint.RowPitch) * y, pSrcSlice + static_cast<SIZE_T>(nPicRowPitch) * y, nPicRowPitch);
			}

			pITextureUpload->Unmap(0, nullptr);
			//�ͷ�ͼƬ����
			::HeapFree(::GetProcessHeap(), 0, pbPicData);
		}
		//�������б������ϴ��Ѹ����������ݵ�Ĭ�϶ѵ����ִ�в�ͬ���ȴ�������ɵڶ���Copy��������GPU�ϵĸ����������
		{
			//ע���ʱֱ�������б�û�а�PSO���������Ҳ�ǲ���ִ��3Dͼ������ģ����ǿ���ִ�и��������Ϊ�������治��Ҫʲô�����״̬����֮��Ĳ���
			D3D12_TEXTURE_COPY_LOCATION stDstCopyLocation = {};
			stDstCopyLocation.pResource = pITexture.Get();
			stDstCopyLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
			stDstCopyLocation.SubresourceIndex = 0;

			D3D12_TEXTURE_COPY_LOCATION stSrcCopyLocation = {};
			stSrcCopyLocation.pResource = pITextureUpload.Get();
			stSrcCopyLocation.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
			stSrcCopyLocation.PlacedFootprint = stTxtLayouts;

			pICommandList->CopyTextureRegion(&stDstCopyLocation, 0, 0, 0, &stSrcCopyLocation, nullptr);

			//����һ����Դ���ϣ�ͬ����ȷ�ϸ��Ʋ������
			D3D12_RESOURCE_BARRIER stResBar = {};
			stResBar.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			stResBar.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			stResBar.Transition.pResource = pITexture.Get();
			stResBar.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
			stResBar.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
			stResBar.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

			pICommandList->ResourceBarrier(1, &stResBar);

			//ִ�в��ȴ�������Դ�ϴ����
			{
				THROW_IF_FAILED(pICommandList->Close());
				ID3D12CommandList* ppCommandLists[] = { pICommandList.Get() };
				pICommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
			}
			//����Χ������ȴ���Ⱦ���
			{
				THROW_IF_FAILED(pID3D12Device4->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&pIFence)));
				n64FenceValue = 1;
				//����һ��Eventͬ���������ڵȴ�Χ���¼�֪ͨ
				hFenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
				if (hFenceEvent == nullptr) {
					THROW_IF_FAILED(HRESULT_FROM_WIN32(GetLastError()));
				}
			}
			//�ȴ�������Դ�������
			{
				const UINT64 n64CurrentFenceValue = n64FenceValue;
				THROW_IF_FAILED(pICommandQueue->Signal(pIFence.Get(), n64CurrentFenceValue));
				n64FenceValue++;
				THROW_IF_FAILED(pIFence->SetEventOnCompletion(n64CurrentFenceValue, hFenceEvent));
			}
		}
		//�����������3D���ݽṹ
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

		UINT32 pBoxIndices[] //��������������
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

		//ʹ�ö�λ��ʽ�������㻺��
		{
			//ʹ�ö�λ��ʽ����ͬ���ϴ������Զ�λ��ʽ�������㻺�壬ע��ڶ�������ָ���˶��еı���λ��
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

			//�����ݴ������㻺�����
			UINT8* pVertexDataBegin = nullptr;
			D3D12_RANGE stReadRange = { 0, 0 };
			THROW_IF_FAILED(pIVertexBuffer->Map(0, &stReadRange, reinterpret_cast<void**>(&pVertexDataBegin)));
			memcpy(pVertexDataBegin, stTriangleVertices, sizeof(stTriangleVertices));

			pIVertexBuffer->Unmap(0, nullptr);

			//������Դ��ͼ
			stVertexBufferView.BufferLocation = pIVertexBuffer->GetGPUVirtualAddress();
			stVertexBufferView.StrideInBytes = sizeof(ST_VERTEX);
			stVertexBufferView.SizeInBytes = nVertexBufferSize;

			//����߽�������ȷ��ƫ��λ��
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
		//���ϴ����Զ�λ��ʽ������������
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
			//�����������壬ע�⻺��ߴ�����Ϊ256�߽�����С
			THROW_IF_FAILED(pID3D12Device4->CreatePlacedResource(
				pIUploadHeap.Get(),
				n64BufferOffset,
				&stMVPResDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&pICBVUpload)
			));
			//Map��Ͳ���Unmap�ˣ�ֱ�Ӹ������ݽ�ȥ������ÿ֡������map-copy-unmap�˷��¼���
			THROW_IF_FAILED(pICBVUpload->Map(0, nullptr, reinterpret_cast<void**>(&pMVPBuffer)));
		}
		//����SRV������
		{
			D3D12_SHADER_RESOURCE_VIEW_DESC stSRVDesc = {};
			stSRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			stSRVDesc.Format = stTextureDesc.Format;
			stSRVDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			stSRVDesc.Texture2D.MipLevels = 1;
			pID3D12Device4->CreateShaderResourceView(pITexture.Get(), &stSRVDesc, pISRVHeap->GetCPUDescriptorHandleForHeapStart());
		}
		//����CBV������
		{
			D3D12_CONSTANT_BUFFER_VIEW_DESC stCBVDesc = {};
			stCBVDesc.BufferLocation = pICBVUpload->GetGPUVirtualAddress();
			stCBVDesc.SizeInBytes = static_cast<UINT>(szMVPBuffer);

			D3D12_CPU_DESCRIPTOR_HANDLE stCPUCBVHandle = pISRVHeap->GetCPUDescriptorHandleForHeapStart();
			stCPUCBVHandle.ptr += nSRVDescriptorSize;

			pID3D12Device4->CreateConstantBufferView(&stCBVDesc, stCPUCBVHandle);
		}
		//�����������Ѻ͸��ֲ�����
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

		//�����Դ���Ͻṹ
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
					n64timeCurrent = ::GetTickCount();
					//������ת�ĽǶȣ���ת�Ƕ�(����) = ʱ��(��) * ���ٶ�(����/��)
					//�����������൱�ھ�����Ϸ��Ϣѭ���е�OnUpdate��������Ҫ��������
					dModelRotationYAngle += ((n64timeCurrent - n64tmFrameStart) / 1000.0f) * g_fPalstance;

					n64tmFrameStart = n64timeCurrent;

					//�����ת�Ƕȴ���2PI����ȡģ
					if (dModelRotationYAngle > XM_2PI) {
						dModelRotationYAngle = fmod(dModelRotationYAngle, XM_2PI);
					}
					//M����
					XMMATRIX xmRot = XMMatrixRotationY(static_cast<float>(dModelRotationYAngle));
					//����MV
					XMMATRIX xmMVP = XMMatrixMultiply(xmRot, XMMatrixLookAtLH(g_v4EyePos, g_v4LookAt, g_v4UpDir));
					//P����
					xmMVP = XMMatrixMultiply(xmMVP, XMMatrixPerspectiveFovLH(XM_PIDIV4, (FLOAT)iWidth / (FLOAT)iHeight, 0.1f, 1000.0f));

					XMStoreFloat4x4(&pMVPBuffer->m_MVP, xmMVP);
				}
				//Reset����������������б�����ָ�������������PSO����
				THROW_IF_FAILED(pICommandAllocator->Reset());
				THROW_IF_FAILED(pICommandList->Reset(pICommandAllocator.Get(), pIPipelineState.Get()));

				//��ȡ�µĺ󻺳���ţ���ΪPresent�������ʱ�󻺳����ž͸�����
				nFrameIndex = pISwapChain3->GetCurrentBackBufferIndex();

				//ͨ����Դ�����ж��󻺳��Ѿ��л���Ͽ��Կ�ʼ��Ⱦ��
				stBeginResBarrier.Transition.pResource = pIARenderTargets[nFrameIndex].Get();
				pICommandList->ResourceBarrier(1, &stBeginResBarrier);

				//ƫ��������ָ�뵽ָ��֡������ͼλ��
				D3D12_CPU_DESCRIPTOR_HANDLE stRTVHandle = pIRTVHeap->GetCPUDescriptorHandleForHeapStart();
				stRTVHandle.ptr += (nFrameIndex * nRTVDescriptorSize);

				//������ȾĿ��
				pICommandList->OMSetRenderTargets(1, &stRTVHandle, FALSE, nullptr);

				pICommandList->RSSetViewports(1, &stViewPort);
				pICommandList->RSSetScissorRects(1, &stScissorRect);

				//������¼�������ʼ��һ֡����Ⱦ
				pICommandList->ClearRenderTargetView(stRTVHandle, faClearColor, 0, nullptr);

				pICommandList->SetGraphicsRootSignature(pIRootSignature.Get());
				ID3D12DescriptorHeap* ppHeaps[] = { pISRVHeap.Get(),pISamplerDescriptorHeap.Get() };
				pICommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

				D3D12_GPU_DESCRIPTOR_HANDLE stGPUSRVHandle = pISRVHeap->GetGPUDescriptorHandleForHeapStart();
				//����SRV
				pICommandList->SetGraphicsRootDescriptorTable(0, stGPUSRVHandle);

				stGPUSRVHandle.ptr += nSRVDescriptorSize;

				//����CBV
				pICommandList->SetGraphicsRootDescriptorTable(1, stGPUSRVHandle);

				D3D12_GPU_DESCRIPTOR_HANDLE hGPUSampler = pISamplerDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
				hGPUSampler.ptr += (g_nCurrentSamplerNo * nSamplerDescriptorSize);

				//����Sample
				pICommandList->SetGraphicsRootDescriptorTable(2, hGPUSampler);

				//ע������ʹ�õ���Ⱦ�ַ����������б�Ҳ����ͨ����Mesh����
				pICommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				pICommandList->IASetVertexBuffers(0, 1, &stVertexBufferView);
				pICommandList->IASetIndexBuffer(&stIndexBufferView);
				//Draw Call
				pICommandList->DrawInstanced(_countof(pBoxIndices), 1, 0, 0);
				//��Դ���ϣ�����ȷ����Ⱦ�Ѿ����������ύ����ȥ��ʾ��
				stEndResBarrier.Transition.pResource = pIARenderTargets[nFrameIndex].Get();
				pICommandList->ResourceBarrier(1, &stEndResBarrier);
				//�ر������б����Կ�ʼִ����
				THROW_IF_FAILED(pICommandList->Close());
				//ִ�������б�
				ID3D12CommandList* ppCommandLists[] = { pICommandList.Get() };
				pICommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
				//�ύ����
				THROW_IF_FAILED(pISwapChain3->Present(1, 0));
				//��ʼͬ��GPU��CPU��ִ�У��߼�¼Χ�����ֵ
				const UINT64 n64CurrentFenceValue = n64FenceValue;
				THROW_IF_FAILED(pICommandQueue->Signal(pIFence.Get(), n64CurrentFenceValue));
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