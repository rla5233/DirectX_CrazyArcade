#include "PreCompile.h"
#include "EngineStructuredBuffer.h"
#include "EngineCore.h"
#include "EngineGraphicDevice.h"

std::map<EShaderType, std::map<std::string, std::map<int, std::shared_ptr<UEngineStructuredBuffer>>>> UEngineStructuredBuffer::StructuredBuffers;

UEngineStructuredBuffer::UEngineStructuredBuffer() 
{
}

UEngineStructuredBuffer::~UEngineStructuredBuffer() 
{
	Release();
}

void UEngineStructuredBuffer::Release()
{
	if (nullptr != SRV)
	{
		SRV->Release();
		SRV = nullptr;
	}
	if (nullptr != Buffer)
	{
		Buffer->Release();
		Buffer = nullptr;
	}
}

void UEngineStructuredBuffer::Setting(EShaderType _Type, UINT _Slot)
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


void UEngineStructuredBuffer::Reset(EShaderType _Type, UINT _Slot)
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

void UEngineStructuredBuffer::Resize(int _Size)
{
	if (BufferInfo.ByteWidth >= static_cast<unsigned int>(_Size))
	{
		return;
	}

	// ������ �̹� ������� �������� �־�
	Release();

	BufferInfo.ByteWidth = _Size;
	BufferInfo.StructureByteStride = Size;


	BufferInfo.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	BufferInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	BufferInfo.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	BufferInfo.Usage = D3D11_USAGE_DYNAMIC;

	if (S_OK != GEngine->GetDirectXDevice()->CreateBuffer(&BufferInfo, nullptr, &Buffer))
	{
		MsgBoxAssert("���ؽ� ���� ������ �����߽��ϴ�.");
		return;
	}

	//srv�� ����� ���� �Ѵ�.
	// �ؽ�ó���� �����°� ���

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
	// srv�� ���۷� ����ڴ�.
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	SRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	SRVDesc.BufferEx.FirstElement = 0;
	SRVDesc.BufferEx.Flags = 0;

	//                             288000  672
	SRVDesc.BufferEx.NumElements = _Size / Size;

	if (S_OK != GEngine->GetDirectXDevice()->CreateShaderResourceView(Buffer, &SRVDesc, &SRV))
	{
		MsgBoxAssert("����ȭ ������ ���̴� ���ҽ� ������ �����߽��ϴ�.");
	}

}

void UEngineStructuredBuffer::ChangeData(const void* _Data, UINT _Size)
{
	// �̽��������� �����ؾ��� �ִ�ġ�� ������ �ִ�.

	Resize(_Size);

#ifdef _DEBUG
	if (nullptr == _Data)
	{
		MsgBoxAssert(GetName() + " ������ۿ� nullptr�� �����͸� �����Ϸ��� �߽��ϴ�.");
	}

	if (0 >= _Size)
	{
		MsgBoxAssert(GetName() + " ������ۿ� 0������ �����͸� �����Ϸ��� �߽��ϴ�.");
	}
#endif

	D3D11_MAPPED_SUBRESOURCE Data = {};

	// ���۸� ��� �����ϱ� ���ؼ� ���ڴ�.
	// �׷���ī��� �̰� ������!
	// Map �ִ��� ���� ����ϴ°� ����.
	GEngine->GetDirectXContext()->Map(Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &Data);

#ifdef _DEBUG
	if (nullptr == Data.pData)
	{
		MsgBoxAssert(GetName() + " �׷���ī�尡 ������ �ź��߽��ϴ�.");
	}
#endif

	// Data.pData �׷���ī�忡 �־��ִ� �ּ�
	memcpy_s(Data.pData, BufferInfo.ByteWidth, _Data, _Size);

	// �ٽ�� �ٽ� ���
	GEngine->GetDirectXContext()->Unmap(Buffer, 0);
}