#pragma once
#include <map>
#include <string>
#include <memory>
#include <list>

#include <EngineBase/EngineString.h>
#include <EnginePlatform/EngineResources.h>

// #include�� �̷��� ����ϴ��� ������ �� �ڵ尡 ������� ������
// �����Ϸ��� �����Ѵ�.
#include "ThirdParty\DirectXTex\inc\DirectXTex.h"


#include "EngineSampler.h"

class UEngineTextureSetter;
class UEngineTexture : public UEngineResources<UEngineTexture>
{
public:
	friend UEngineTextureSetter;
	// constrcuter destructer
	UEngineTexture();
	~UEngineTexture();

	// delete Function
	UEngineTexture(const UEngineTexture& _Other) = delete;
	UEngineTexture(UEngineTexture&& _Other) noexcept = delete;
	UEngineTexture& operator=(const UEngineTexture& _Other) = delete;
	UEngineTexture& operator=(UEngineTexture&& _Other) noexcept = delete;

	static std::shared_ptr<UEngineTexture> Create(ID3D11Texture2D* _Texture)
	{
		std::shared_ptr<UEngineTexture> NewRes = CreateResUnName();
		NewRes->ResCreate(_Texture);
		return NewRes;
	}

	static std::shared_ptr<UEngineTexture> Create(const D3D11_TEXTURE2D_DESC& _Desc)
	{
		std::shared_ptr<UEngineTexture> NewRes = CreateResUnName();
		NewRes->ResCreate(_Desc);
		return NewRes;
	}

	static std::shared_ptr<UEngineTexture> Load(std::string_view _Path)
	{
		UEnginePath NewPath = UEnginePath(std::filesystem::path(_Path));
		std::string FileName = NewPath.GetFileName();
		return Load(_Path, FileName);
	}

	static std::shared_ptr<UEngineTexture> Load(std::string_view _Path, std::string_view _Name)
	{
		std::shared_ptr<UEngineTexture> NewRes = CreateResName(_Path, _Name);
		NewRes->ResLoad();
		return NewRes;
	}

	///

	static std::shared_ptr<UEngineTexture> ThreadSafeCreate(ID3D11Texture2D* _Texture)
	{
		std::shared_ptr<UEngineTexture> NewRes = ThreadSafeCreateResUnName();
		NewRes->ResCreate(_Texture);
		return NewRes;
	}

	static std::shared_ptr<UEngineTexture> ThreadSafeCreate(const D3D11_TEXTURE2D_DESC& _Desc)
	{
		std::shared_ptr<UEngineTexture> NewRes = ThreadSafeCreateResUnName();
		NewRes->ResCreate(_Desc);
		return NewRes;
	}

	static std::shared_ptr<UEngineTexture> ThreadSafeLoad(std::string_view _Path)
	{
		UEnginePath NewPath = UEnginePath(std::filesystem::path(_Path));
		std::string FileName = NewPath.GetFileName();
		return ThreadSafeLoad(_Path, FileName);
	}

	static std::shared_ptr<UEngineTexture> ThreadSafeLoad(std::string_view _Path, std::string_view _Name)
	{
		std::shared_ptr<UEngineTexture> NewRes = ThreadSafeCreateResName(_Path, _Name);
		NewRes->ResLoad();
		return NewRes;
	}


	ID3D11RenderTargetView* GetRTV()
	{
		return RTV;
	}

	ID3D11ShaderResourceView* GetSRV()
	{
		return SRV;
	}

	float4 GetScale()
	{
		return float4(Desc.Width, Desc.Height);
	}

	// float ��ü�� ������ �ڷ���
	// �뷫���� �� �Ǽ������� ������ �߻��Ѵ�.

	// float4 GetColor(unsigned)

	// Color8Bit _DefaultColor => ȭ�� �ٱ��� �ִ� �÷��� ������ �޶�� �ϸ�  _DefaultColor�� ���ϵȴ�.

	Color8Bit GetColor(float4 _Pos, Color8Bit _DefaultColor)
	{
		return GetColor(_Pos.iX(), _Pos.iY(), _DefaultColor);
	}

	Color8Bit GetColor(unsigned int _X, unsigned int _Y, Color8Bit _DefaultColor);

protected:


private:

	// �̹��� �� ��ü.
	// Directx������ �޸𸮸� �ǹ��ϴ� �ڵ�
	// �׸��� �׷� �޸𸮰� �־�� �޸� ��������
	ID3D11Texture2D* Texture2D = nullptr;

	// Directx������ �̹���(�޸�) ���� ����(���⿡ �׸��� �ִ� ����)
	ID3D11RenderTargetView* RTV = nullptr;
	// directx���� �̹����� ���̴��� �����Ҽ� �ִ� ����.
	ID3D11ShaderResourceView* SRV = nullptr;

	D3D11_TEXTURE2D_DESC Desc;

	DirectX::TexMetadata Data;
	DirectX::ScratchImage Image;

	std::shared_ptr<UEngineSampler> Sampler;

	void ResCreate(const D3D11_TEXTURE2D_DESC& _Desc);

	void ResCreate(ID3D11Texture2D* _Texture);

	void ResLoad();

	void CreateRenderTargetView();
	void CreateShaderResourceView();

	void Setting(EShaderType _Type, UINT _Slot);
	void Reset(EShaderType _Type, UINT _Slot);
};

