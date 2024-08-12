#include "PreCompile.h"
#include "EngineGraphicDevice.h"
#include "EngineTexture.h"
#include "EngineRenderTarget.h"

// ���̷�Ʈ ���̺귯���� ����ϱ� ���ؼ�

UEngineGraphicDevice::UEngineGraphicDevice() 
{
}

UEngineGraphicDevice::~UEngineGraphicDevice() 
{
	if (nullptr != SwapChain)
	{
		SwapChain->Release();
	}

	if (nullptr != Context)
	{
		Context->Release();
	}

	if (nullptr != Device)
	{
		Device->Release();
	}
}

IDXGIAdapter* UEngineGraphicDevice::GetHighPerFormanceAdapter()
{
	// �׷���ī�尡 2����

	IDXGIFactory* Factory = nullptr;
	IDXGIAdapter* Adapter = nullptr;

	// IDXGIFactory* Factory�� ���̷�Ʈx ���̺귯���� ����� �뵵�� 
	// �������̽��Դϴ�
	// Adapter�� ����� ������  Factory �ʿ��ϴ�.
	// CreateDXGIFactory()
	
	// ���� ���� �ȵǸ� �������
	HRESULT HR = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&Factory));

	if (nullptr == Factory)
	{
		MsgBoxAssert("�׷���ī�忡�� ���丮 �������̽��� �������� ���߽��ϴ�");
		return nullptr;
	}

	size_t VideoMemory = 0;
	// �׷���ī�� ���� ū�ְ�
	// ���� ������ ���� �׷���ī���� �����ϰڴ�

	for (int Index = 0;; Index++)
	{
		IDXGIAdapter* CurAdapter = nullptr;

		Factory->EnumAdapters(Index, &CurAdapter);

		if (nullptr == CurAdapter)
		{
			break;
		}

		DXGI_ADAPTER_DESC Desc;
		CurAdapter->GetDesc(&Desc);

		if (VideoMemory <= Desc.DedicatedVideoMemory)
		{
			VideoMemory = Desc.DedicatedVideoMemory;

			if (nullptr != Adapter)
			{
				// ������ ���� ��Ͱ� �ִٸ�
				// �� ��ʹ� ����������
				Adapter->Release();
			}

			Adapter = CurAdapter;
			continue;
		}

		CurAdapter->Release();
	}

	Factory->Release();
	return Adapter;
}

void UEngineGraphicDevice::Initialize(const UEngineWindow& _Window, const float4& _ClearColor)
{
	if (nullptr == _Window.GetHWND())
	{
		MsgBoxAssert("Open ���� ���� ������� �׷���ī�� ��ġ�� �ʱ�ȭ�Ϸ��� �߽��ϴ�.");
		return;
	}

	int iFlag = 0;

#ifdef _DEBUG
	// ����� ����϶���
	//  D3D11_CREATE_DEVICE_SINGLETHREADED = 0x1,
	//	D3D11_CREATE_DEVICE_DEBUG = 0x2,
	//	D3D11_CREATE_DEVICE_SWITCH_TO_REF = 0x4,
	//	D3D11_CREATE_DEVICE_PREVENT_INTERNAL_THREADING_OPTIMIZATIONS = 0x8,
	//	D3D11_CREATE_DEVICE_BGRA_SUPPORT = 0x20,
	//	D3D11_CREATE_DEVICE_DEBUGGABLE = 0x40,
	//	D3D11_CREATE_DEVICE_PREVENT_ALTERING_LAYER_SETTINGS_FROM_REGISTRY = 0x80,
	//	D3D11_CREATE_DEVICE_DISABLE_GPU_TIMEOUT = 0x100,
	//	D3D11_CREATE_DEVICE_VIDEO_SUPPORT = 0x800


	iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

	// �� �׷���ī�尡 ���̷�Ʈ x�� ��������� �����Ҽ� �ִ��� �˾Ƴ��� ���� ����
	D3D_FEATURE_LEVEL Level = D3D_FEATURE_LEVEL_11_0;

	// �׷���ī�带 ��ǥ�ϴ� �ڵ�
	// �ִ� ���� �׷���ī�带 ã�� ���� �Լ�
	Adapter = GetHighPerFormanceAdapter();

	if (nullptr == Adapter)
	{
		MsgBoxAssert("�׷���ī�尡 �������� �ʴ� ��ǻ�� �Դϴ�");
		return;
	}

	// �׷���ī�带 ã������
	// Adapter �׷���ī�忡 ����� ������ �ִ� �׷���ī�忡 ����� ������ �ִ� ����̽��� ���ؽ�Ʈ�� ����̴ϴ�.

	// D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_UNKNOWN  // �ϰ� �˾Ƽ� ����
	// D3D_DRIVER_TYPE_HARDWARE	= ( D3D_DRIVER_TYPE_UNKNOWN + 1 ) , �׷���ī���
	// D3D_DRIVER_TYPE_REFERENCE = (D3D_DRIVER_TYPE_HARDWARE + 1), // Ưȭ�� ���Ҳ�
	// D3D_DRIVER_TYPE_NULL = (D3D_DRIVER_TYPE_REFERENCE + 1),
	// D3D_DRIVER_TYPE_SOFTWARE = (D3D_DRIVER_TYPE_NULL + 1), Cpu�� �����Ҳ�.
	// D3D_DRIVER_TYPE_WARP = (D3D_DRIVER_TYPE_SOFTWARE + 1)

	// �Լ��� �뵵�� 2���ΰž�

	HRESULT Result = D3D11CreateDevice(
		Adapter, // nullptr; ���̷�Ʈ x�� 
		D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_UNKNOWN, // -0 
		nullptr,
		iFlag,
		nullptr, // ��ó���� ���������ε�
		0, // ���߿��� ���� ���ð����� ��ó������ ��������� 
		D3D11_SDK_VERSION, // 
		&Device,
		&Level, // ���� ������ ��ó����
		&Context
	);

	if (nullptr == Context)
	{
		MsgBoxAssert("�׷���ī�尡 ������ ���� ���� ���ؽ�Ʈ�� �����ϴµ� �����߽��ϴ�.");
		return;
	}

	if (nullptr == Device)
	{
		MsgBoxAssert("�׷���ī�尡 �޸����� ���� ����̽��� �����ϴµ� �����߽��ϴ�.");
		return;
	}
	
	// �� ������ ��� ������ null�˻�
	// ������ü�� ������ �׳� �ϴ°Ŵϱ�.
	//if (nullptr != Adapter)
	//{
	//	Adapter->Release();
	//}

	if (S_OK != Result)
	{
		MsgBoxAssert("����̽� ������ �����߽��ϴ�.");
		// DWORD Result = GetLastError(); // �� ������ ���ؼ� �ڼ��ϰ� �˼� �ִ�.
		// ������ ����
		return;
	}

	if (D3D_FEATURE_LEVEL_11_0 != Level)
	{
		MsgBoxAssert("���̷�Ʈ 11������ ����Ҽ� ���� �׷���ī�� �Դϴ�.");
		return;
	}

	// CPU���� ���� ��Ƽ������ε� �����ϰ� �����ϰ� �ϰڴ�.
	Result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	//0x80010106;
	//E_INVALIDARG;
	//E_OUTOFMEMORY;
	//E_UNEXPECTED;
	//S_OK;
	//S_FALSE;
	//RPC_E_CHANGED_MODE;

	// ��Ƽ������� �� �م������
	if (S_OK != Result)
	{
		if (RPC_E_CHANGED_MODE != Result)
		{
			MsgBoxAssert("��Ƽ������ �ɼ��� ����� ���� �����ϴ�.");
			return;
		}
	}


	WindowPtr = &_Window;

	CreateSwapChain(_ClearColor);

	EngineResourcesInit();
}

void UEngineGraphicDevice::CreateSwapChain(const float4& _ClearColor)
{
	// ����ü�� ��ü�� ����Դϴ�.
	// ���� ���������� ������ �̹����� ó���ϱ� ������.
	// �Ȼ���Ҽ��� ����.
	// �׸��� �׷����� ���� �����Ǿ��ִ� ������ �������ܰ��� ����Ǵµ�.
	// �׸��� �׷����� ���� �������� �� �����Ǿ��ִ� ������ �����������������̶�� �ϰ�
	// �̰� ��Ȥ���� ȸ�簡���� �ʰ� ������� �־
	// ����ü���� �� ������ ������������ ���� �������ܰ踦 ����մϴ�.
	// ����Ϳ� ���.
	// ���������� ������ �ִ� �׸��� ������ ����� �ٽñ׸��°� ��� �������� ���� ��������
	// ����Ͷ�� �ϴ� �ϵ��� ������ ����ϴ� �����ȿ� �츮�� ���ܼ� ����ϴ� ���̴�.

	// �ػ󵵶�� �θ���.
	float4 Resolution = WindowPtr->GetWindowScale();

	// ����ü���� ����� ���� ����ü�� ����� �����Ѵ�.
	DXGI_SWAP_CHAIN_DESC ScInfo = { 0 };

	ScInfo.BufferCount = 2;
	ScInfo.BufferDesc.Width = Resolution.iX();
	ScInfo.BufferDesc.Height = Resolution.iY();
	ScInfo.OutputWindow = WindowPtr->GetHWND();

	// �ֻ��� ����Ϳ� �󸶳� ������ �����Ұų�
	ScInfo.BufferDesc.RefreshRate.Denominator = 1;
	ScInfo.BufferDesc.RefreshRate.Numerator = 60;

	// ���� ����
	ScInfo.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	// ����� �ȼ��� �����ϴ� ���� ����� �״��
	ScInfo.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	// �ƿ� ���ȳ�
	ScInfo.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// �� ����ü���� �����뵵�� ���ǰǰ���?
	// RENDER_TARGET �ؽ�ó 1���� ���մϴ�.
	// DXGI_USAGE_SHADER_INPUT ���̴��� �Է��ϴ� ���·� �ϰڴ�.
	// ���̴���
	ScInfo.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;

	// �̰� �̿ܿ� ���ʿ䰡 ����.
	// ���ø�
	ScInfo.SampleDesc.Quality = 0;
	ScInfo.SampleDesc.Count = 1;

	// ���۰� ������
	// ��ü�ϴ� �����ε� 010101010101
	// �˰� �־���� �ǹ� ����.
	// DXGI_SWAP_EFFECT_FLIP_DISCARD 2���� ����� �������.
	// �̰Ŷ����� ȭ���� ������
	// ScInfo.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;
	ScInfo.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD;
	// ���� ��� �ȳ���.
	ScInfo.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// ��üȭ���̳� �ƴϾ�.
	// �߰��� ��üȭ�鿡�� ������� �ٲٴ� 
	// ��üȭ���� �������� ������.
	ScInfo.Windowed = true;

	// ������ �̰� �����ؾ� �Ѵ�.

	// �ɼǳ�
	// ����ü���� ����̴ϴ�.
	// �޸𸮸� �Ҵ��ϴ°ų�. ����̽�
	// �׳� ���� �貼��� �Ф�

	// ����ü���� ����ԵǸ� 
	// DC�� ����� ���ο� ȭ��ũ�⸸�� �ؽ�ó�� ����� �ݴϴ�.

	// ����̽��� ������ �׾ȿ� �ִ� ��͸� ���ɴϴ�.
	// IDXGIDevice* pD = 
	// Device->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>())

	// �� ��ġ�� ���õ� ������(���丮)�� ���� �Լ�

	IDXGIFactory* pF = nullptr;

	Adapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&pF));

	// �޸��Ҵ��ϴ� �����Լ��� �ϴ� ����̽� �־�޶���Ұ̴ϴ�.
	pF->CreateSwapChain(Device, &ScInfo, &SwapChain);

	Adapter->Release();
	pF->Release();

	// ������ ���� �����̴�.
	
	// ���� ������ ȭ��ũ�⸸�� directx�� image�� ����ִ�.
	// ID3d11Texture2D
	// ���Ը����ڸ� IDXGISwapChain ���ο� ���� ��� �ִµ�
	// ���� Ű�� key�� �� ���ڿ��̶�� �����ض�.
	// MIDL_INTERFACE("7b7166ec-21c7-44ae-b21a-c9ae321ae369")
	// IDXGIFactory: public IDXGIObject

	ID3D11Texture2D* DXBackBufferTexture = nullptr;
	if (S_OK != SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&DXBackBufferTexture)))
	{
		MsgBoxAssert("����� �ؽ�ó�� ������ ���߽��ϴ�");
		return;
	}

	// �̱��� �Ⱦ�
	// �������� �׳� �� Ŭ������ ������.
	// �翬�� static���� ��� ���ҽ��� ��������� �����ؾ� �Ѵ�.
	// �츮�� ���ϴ� �������׶� �̳༮�� ���� �߿��� �ؽ�ó �ᱹ ���⿡ �׷��� ȭ�鿡 ���´�.
	// ������ �׸��� �ִ� ������ ���� �����
	std::shared_ptr<UEngineTexture> Texture = UEngineTexture::Create(DXBackBufferTexture);

	// �׸��� �ִ� �����̳� ����� ��ɵ��� �� ����Ÿ���̶�� �ɷ� 
	// �ؽ�ó �״�δ� ��������� ������� �ʰ�
	// ������ ����Ÿ�� ��θ� ����� �̴ϴ�.
	// �̹������� �̹����� �׸���.
	BackBufferRenderTarget = UEngineRenderTarget::Create(Texture, _ClearColor);
	// �츮 winapi HDC�� HDC�״�� ����߳���?
	// 


	
}

void UEngineGraphicDevice::RenderStart()
{
	BackBufferRenderTarget->Clear();
}

void UEngineGraphicDevice::RenderEnd()
{
	// BackBufferRenderTarget�� �׷����� ȭ�鿡 �׷���
	// tn
	HRESULT Result = SwapChain->Present(0, 0);

	if (Result == DXGI_ERROR_DEVICE_REMOVED || Result == DXGI_ERROR_DEVICE_RESET)
	{
		MsgBoxAssert("�ػ󵵳� ����̽� ���� ������ ���� �Ǿ����ϴ�");
		return;
	}

}

