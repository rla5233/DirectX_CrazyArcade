#pragma once
#include <EnginePlatform/EngineResources.h>
#include "RenderUnit.h"
#include <memory>

// ���� : �ؽ�ó�� �׸��� ������ �ٸ���?
// �ؽ�ó => �̹��� ����ü ��������� ���� HBITMAP

// ����Ÿ�� => �� �̹����� �����ϰ� �׸��� �����ϴ� ����� ���� ��. UWindowImage

// ȿ���� �ش� => �׸���.
class UEngineRenderTarget;
class UEffect : public URenderUnit
{
	friend UEngineRenderTarget;

public:
	UEffect()
	{
		SetMesh("FullRect");
	}

	virtual void Init() = 0;
	// EffectTarget ȿ���� �ְ� ���� Ÿ��
	virtual void Effect(std::shared_ptr<UEngineRenderTarget> EffectTarget) = 0;

	bool IsActive()
	{
		return IsActiveValue;
	}

	void Active(bool _IsActive)
	{
		IsActiveValue = _IsActive;
	}

private:
	bool IsActiveValue = true;
};

class ULevel;
class UEngineTexture;
class UEngineGraphicDevice;
class UEngineRenderTarget : public UEngineResources<UEngineRenderTarget>, public std::enable_shared_from_this<UEngineRenderTarget>
{
	friend ULevel;
	friend UEngineGraphicDevice;

	static URenderUnit CopyUnit;

public:
	// constrcuter destructer
	UEngineRenderTarget();
	~UEngineRenderTarget();

	// delete Function
	UEngineRenderTarget(const UEngineRenderTarget& _Other) = delete;
	UEngineRenderTarget(UEngineRenderTarget&& _Other) noexcept = delete;
	UEngineRenderTarget& operator=(const UEngineRenderTarget& _Other) = delete;
	UEngineRenderTarget& operator=(UEngineRenderTarget&& _Other) noexcept = delete;

	static std::shared_ptr<UEngineRenderTarget> Create(std::shared_ptr<UEngineTexture> _Texture, const float4& _Color)
	{
		std::shared_ptr<UEngineRenderTarget> NewRes = CreateResUnName();
		NewRes->AddNewTexture(_Texture, _Color);
		return NewRes;
	}

	static std::shared_ptr<UEngineRenderTarget> Create()
	{
		std::shared_ptr<UEngineRenderTarget> NewRes = CreateResUnName();
		return NewRes;
	}

	// ����ִ� �̹����� �����.
	void CreateTexture(DXGI_FORMAT _Format, float4 _Scale, float4 _ClearColor);

	void Clear();

	void Setting();

	// _Other �� ������ ī���Ѵ�.
	void Copy(std::shared_ptr<UEngineRenderTarget> _Other);

	// _Other �� ������ ��ģ���Ѵ�.
	void Merge(std::shared_ptr<UEngineRenderTarget> _Other, int _Index = 0);

	template<typename EffectType>
	std::shared_ptr<EffectType> AddEffect()
	{
		std::shared_ptr<UEffect> Effect = std::make_shared<EffectType>();

		Effect->Init();

		Effects.push_back(Effect);

		return std::dynamic_pointer_cast<EffectType>(Effect);
	}

	std::shared_ptr<UEngineTexture> GetTexture(int _Index = 0)
	{
		return Textures[_Index];
	}

protected:

private:
	// �� �ؽ�ó �������� ��� ����Ҽ� �ְ� �Ұ̴ϴ�.
	std::vector<std::shared_ptr<UEngineTexture>> Textures;
	std::vector<ID3D11RenderTargetView*> RTVs;
	std::vector<float4> ClearColors;

	void AddNewTexture(std::shared_ptr<UEngineTexture> _Texture, const float4& _Color);

	static void RenderTargetInit();

	// �� ����Ÿ�ٿ� ����� ȿ����
	std::vector<std::shared_ptr<UEffect>> Effects;

	void Effect(float _DeltaTime);
};

