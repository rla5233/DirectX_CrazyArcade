#include "PreCompile.h"
#include "EngineTexture.h"
#include "EngineCore.h"

// ���̺귯���� release�ų� debug�ų� �̸��� ���Ƽ�
// ���� #ifdef _DEBUG�� ������ �ʿ䰡 ����.
#pragma comment (lib, "DirectXTex.lib")

UEngineTexture::UEngineTexture()
{
}

UEngineTexture::~UEngineTexture()
{
	if (nullptr != SRV)
	{
		SRV->Release();
	}

	if (nullptr != RTV)
	{
		RTV->Release();
	}

	if (nullptr != Texture2D)
	{
		Texture2D->Release();
	}
}

void UEngineTexture::ResCreate(const D3D11_TEXTURE2D_DESC& _Desc)
{
	Desc = _Desc;

	GEngine->GetDirectXDevice()->CreateTexture2D(&Desc, nullptr, &Texture2D);

	if (Desc.BindFlags & D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET)
	{
		CreateRenderTargetView();
	}
	if (Desc.BindFlags & D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET)
	{
		CreateShaderResourceView();
	}
}

void UEngineTexture::ResCreate(ID3D11Texture2D* _Texture)
{
	Texture2D = _Texture;

	Texture2D->GetDesc(&Desc);

	CreateRenderTargetView();
}

void UEngineTexture::CreateShaderResourceView()
{
	if (nullptr != SRV)
	{
		return;
	}

	if (nullptr == Texture2D)
	{
		MsgBoxAssert("��������� ���� �ؽ�ó�� ����Ÿ�ٺ並 �����Ϸ��� �߽��ϴ�.");
		return;
	}

	// ���� �ؽ�ó �޸𸮿��� �̹����� �����Ҽ� �ִ� ������ �����
	// ���̷�Ʈ���� ������ �Ϸ��� �ٺ������� 2���� ���õǾ� �ֽ��ϴ�.
	// �޸� => Device
	// ������ => Context
	HRESULT Result = GEngine->GetDirectXDevice()->CreateShaderResourceView(Texture2D, nullptr, &SRV);

	if (S_OK != Result)
	{
		MsgBoxAssert("���̴� ���ҽ� �� ������ �����߽��ϴ�.");
		return;
	}
}

void UEngineTexture::CreateRenderTargetView()
{
	if (nullptr != RTV)
	{
		return;
	}

	if (nullptr == Texture2D)
	{
		MsgBoxAssert("��������� ���� �ؽ�ó�� ����Ÿ�ٺ並 �����Ϸ��� �߽��ϴ�.");
		return;
	}

	// ���� �ؽ�ó �޸𸮿��� �̹����� �����Ҽ� �ִ� ������ �����
	// ���̷�Ʈ���� ������ �Ϸ��� �ٺ������� 2���� ���õǾ� �ֽ��ϴ�.
	// �޸� => Device
	// ������ => Context
	HRESULT Result = GEngine->GetDirectXDevice()->CreateRenderTargetView(Texture2D, nullptr, &RTV);

	if (S_OK != Result)
	{
		MsgBoxAssert("����Ÿ�� �� ������ �����߽��ϴ�.");
		return;
	}
}

void UEngineTexture::ResLoad()
{
	// 8196 ��������?
	// �ʹ� ũ�� �ȵɼ��� �ִ�.
	UEnginePath File = GetEnginePath();

	std::string Ext = UEngineString::ToUpper(File.GetExtension());

	std::wstring wPath = UEngineString::AnsiToUniCode(File.GetFullPath());

	if (Ext == ".DDS")
	{
		// Image �̹��� �ε� �����
		if (S_OK != DirectX::LoadFromDDSFile(wPath.c_str(), DirectX::DDS_FLAGS_NONE, &Data, Image))
		{
			MsgBoxAssert("DDS ���Ϸε忡 �����߽��ϴ�.");
		};
	}
	else if (Ext == ".TGA")
	{
		if (S_OK != DirectX::LoadFromTGAFile(wPath.c_str(), DirectX::TGA_FLAGS_NONE, &Data, Image))
		{
			MsgBoxAssert("TGA ���Ϸε忡 �����߽��ϴ�.");
		};
	}
	else
	{
		// Png jpg ����� else
		if (S_OK != DirectX::LoadFromWICFile(wPath.c_str(), DirectX::WIC_FLAGS_NONE, &Data, Image))
		{
			MsgBoxAssert("PNG ���Ϸε忡 �����߽��ϴ�.");
		};
	}

	// HRESULT Result = GEngine->GetDirectXDevice()->CreateShaderResourceView(Texture2D, nullptr, &SRV);

	// ���� ����� ������� ���� �Լ��� �ڽ��� �ε��� �����͸� ������ ���̴� ���ҽ� �並 ����� �ְ� ó���� �����ϴ�.
	// DirectX::CreateShaderResourceView 
	if (S_OK != DirectX::CreateShaderResourceView(
		GEngine->GetDirectXDevice(),
		Image.GetImages(),
		Image.GetImageCount(),
		Image.GetMetadata(),
		&SRV
	))
	{
		MsgBoxAssert("�ؽ�ó ���̴� ���ñ��� ������ �����߽��ϴ�" + GetName());
		return;
	}

	Desc.Width = static_cast<UINT>(Data.width);
	Desc.Height = static_cast<UINT>(Data.height);

}

void UEngineTexture::Setting(EShaderType _Type, UINT _Slot)
{
	switch (_Type)
	{
	case EShaderType::Vertex:
		GEngine->GetDirectXContext()->VSSetShaderResources(_Slot, 1, &SRV);
		break;
	case EShaderType::Pixel:
		GEngine->GetDirectXContext()->PSSetShaderResources(_Slot, 1, &SRV);
		break;
	case EShaderType::NONE:
	default:
		MsgBoxAssert("Ÿ���� �Һи��� �ؽ�ó �����Դϴ�.");
		break;
	}
}

void UEngineTexture::Reset(EShaderType _Type, UINT _Slot)
{
	ID3D11ShaderResourceView* NullptrSRV = nullptr;

	switch (_Type)
	{
	case EShaderType::Vertex:
		GEngine->GetDirectXContext()->VSSetShaderResources(_Slot, 1, &NullptrSRV);
		break;
	case EShaderType::Pixel:
		GEngine->GetDirectXContext()->PSSetShaderResources(_Slot, 1, &NullptrSRV);
		break;
	case EShaderType::NONE:
	default:
		MsgBoxAssert("Ÿ���� �Һи��� �ؽ�ó �����Դϴ�.");
		break;
	}
}


Color8Bit UEngineTexture::GetColor(unsigned int _X, unsigned int _Y, Color8Bit _DefaultColor)
{
	if (_X < 0)
	{
		return _DefaultColor;
	}

	if (_Y < 0)
	{
		return _DefaultColor;
	}


	if (_X >= GetScale().uiX())
	{
		return _DefaultColor;
	}

	if (_Y >= GetScale().uiY())
	{
		return _DefaultColor;
	}

	// �̹����� ������ ������ �پ��ϴ�.
	// RGBA

	// 4����Ʈ �÷�?
	DXGI_FORMAT Fmt = Image.GetMetadata().format;

	// �ε��� �̹����� �ȼ��浹�� �ɰ��̴�.
	// 1����Ʈ ¥�� �ڷ���
	unsigned char* Ptr = Image.GetPixels();

	// 22
	// 
	// [4][4]
	// [4][4]

	// Ptr
	// Ptr
	// Ptr += 12;
	// _X, _Y
	
	//  1        2              1
	// ((_Y * GetScale().ix()) + _X) * 4 ;
	// 3 * 4

	// |
	// [][][][] [][][][]
	// [][][][] [][][][]

	// DXGI_FORMAT_B8G8R8A8_UNORM ����1�ȼ��� 4����Ʈ

	switch (Fmt)
	{
	case DXGI_FORMAT_B8G8R8A8_UNORM:
	case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
	{
		Color8Bit Result;
		Ptr += ((_Y * GetScale().iX()) + _X) * 4;
		Result.B = Ptr[0];
		Result.G = Ptr[1];
		Result.R = Ptr[2];
		Result.A = Ptr[3];
		return Result;
	}
	default:
		MsgBoxAssert("���� ó���Ҽ� ���� GetPixel �����Դϴ�");
		break;
	}


	return Color8Bit();
}