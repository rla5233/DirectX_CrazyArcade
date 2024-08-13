#pragma once
#include <EnginePlatform/EngineResources.h>

// ���� :
class UEngineMaterial;
class UEngineBlend : public UEngineResources<UEngineBlend>
{
	friend UEngineMaterial;
public:
	// constrcuter destructer
	UEngineBlend();
	~UEngineBlend();

	// delete Function
	UEngineBlend(const UEngineBlend& _Other) = delete;
	UEngineBlend(UEngineBlend&& _Other) noexcept = delete;
	UEngineBlend& operator=(const UEngineBlend& _Other) = delete;
	UEngineBlend& operator=(UEngineBlend&& _Other) noexcept = delete;

	static std::shared_ptr<UEngineBlend> Create(std::string_view _Name, const D3D11_BLEND_DESC& _Info)
	{
		std::shared_ptr<UEngineBlend> Res = CreateResName(_Name);
		Res->ResCreate(_Info);
		return Res;
	}

protected:


private:
	ID3D11BlendState* State = nullptr;
	// ���� ���Ŀ� ������ ���� �ʰڴ�
	// ���ʹ� ������ ���� �ʰڴ�.
	float4 Factor = float4::Zero;
	// ���� ��� �κп� ���� ����� ����ǰ� �ض�.
	// ����� ������ ������ ���� ���̰� �ض�.
	//            0b111111111111111111111111111111111111111111111111
	UINT Mask = 0xFFFFFFFF;
	// RGBA

	void ResCreate(const D3D11_BLEND_DESC& _Info);
	void Setting();
};

