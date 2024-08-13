#pragma once

// ���� : ��� ��Ƽ�÷��� �����̸� OpenGl�� ����Ҽ� �ְ�
//        Directx�� ����Ҽ��� �������̴�.
//        �Ϲ������� ������ ���⿡�� �̰� � ���̺귯���� �����ϴ��� �������� �ȵȴ�.

class ULevel;
class UEngineCore;
class UEngineWindow;
class UEngineRenderTarget;

class UEngineGraphicDevice
{
	friend UEngineCore;
	friend ULevel;

public:
	// constrcuter destructer
	UEngineGraphicDevice();
	~UEngineGraphicDevice();

	// delete Function
	UEngineGraphicDevice(const UEngineGraphicDevice& _Other) = delete;
	UEngineGraphicDevice(UEngineGraphicDevice&& _Other) noexcept = delete;
	UEngineGraphicDevice& operator=(const UEngineGraphicDevice& _Other) = delete;
	UEngineGraphicDevice& operator=(UEngineGraphicDevice&& _Other) noexcept = delete;

	// nullptr�� ���ɼ��� ���ֹ����� ���ؼ�
	void Initialize(const UEngineWindow& _Window, const float4& _ClearColor);

	struct ID3D11Device* GetDevice()
	{
		return Device;
	}

	struct ID3D11DeviceContext* GetContext()
	{
		return Context;
	}

	void RenderStart();

	void RenderEnd();

protected:

private:
	// 9���������� Device��� �������̽� �ۿ� ������.
	// �翬�� ���Ұ� ��������ϱ� �ڵ� ����̴�.
	// ID3D9Device* Device; <= com��ü ����� ǥ���̶�µ�
	// Device->MeshRender(); �׸���
	// Device->LoadTexture(); ���ҽ� �����Ҷ�
	// ���� ���� �ܼ����� �ʽ��ϴ�.

	// 11�ͼ� 2���� �о߷� �������̽��� �и��ߴ�.

	// Device->LoadTexture(); �޸�(�׸�, �̹���, ��, ������) �����Ҷ�
	struct ID3D11Device* Device = nullptr;

	// Context->MeshRender(); �׸���
	struct ID3D11DeviceContext* Context = nullptr;

	const class UEngineWindow* WindowPtr;

	// ���� ���۸�
	struct IDXGISwapChain* SwapChain = nullptr;

	struct IDXGIAdapter* Adapter = nullptr;

	std::shared_ptr<UEngineRenderTarget> BackBufferRenderTarget = nullptr;

	// �׷���ī���� ���� ����� ���� �׷���ī�带 �������� �Լ�
	struct IDXGIAdapter* GetHighPerFormanceAdapter();
	void CreateSwapChain(const float4& _ClearColor);
	void EngineResourcesInit();
	void EngineResourcesRelease();

	std::shared_ptr<UEngineRenderTarget> GetBackBufferRenderTarget()
	{
		return BackBufferRenderTarget;
	}
};

