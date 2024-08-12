#include "PreCompile.h"
#include "EngineConstantBuffer.h"
#include "EngineCore.h"

std::map<EShaderType, std::map<std::string, std::map<int, std::shared_ptr<UEngineConstantBuffer>>>> UEngineConstantBuffer::ConstantBuffers;

UEngineConstantBuffer::UEngineConstantBuffer() 
{
}

UEngineConstantBuffer::~UEngineConstantBuffer() 
{
}

void UEngineConstantBuffer::ResCreate(UINT _ByteSize)
{
	BufferInfo.ByteWidth = _ByteSize;
	BufferInfo.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	// CPU���� ��������� ������ ���� ����ڴ�.
	BufferInfo.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	// �ٲ��.
	// ���� �߸� �����Ѱ� �����ϴ�.
	// D3D11_USAGE_DEFAULT
	BufferInfo.Usage = D3D11_USAGE_DYNAMIC;

	if (S_OK != GEngine->GetDirectXDevice()->CreateBuffer(&BufferInfo, nullptr, &Buffer))
	{
		MsgBoxAssert("���ؽ� ���� ������ �����߽��ϴ�.");
		return;
	}
}

void UEngineConstantBuffer::Setting(EShaderType _Type, UINT _Slot)
{
	switch (_Type)
	{
	case EShaderType::Vertex:
		GEngine->GetDirectXContext()->VSSetConstantBuffers(_Slot, 1, &Buffer);
		break;
	case EShaderType::Pixel:
		GEngine->GetDirectXContext()->PSSetConstantBuffers(_Slot, 1, &Buffer);
		break;
	case EShaderType::NONE:
	default:
		MsgBoxAssert("ó���� �Ϸ���� ���� ������� ���̴� ���� Ÿ���Դϴ�");
		break;
	}
}

void UEngineConstantBuffer::Reset(EShaderType _Type, UINT _Slot)
{
	ID3D11Buffer* Buffer = nullptr;

	switch (_Type)
	{
	case EShaderType::Vertex:
		GEngine->GetDirectXContext()->VSSetConstantBuffers(_Slot, 1, &Buffer);
		break;
	case EShaderType::Pixel:
		GEngine->GetDirectXContext()->PSSetConstantBuffers(_Slot, 1, &Buffer);
		break;
	case EShaderType::NONE:
	default:
		MsgBoxAssert("ó���� �Ϸ���� ���� ������� ���̴� ���� Ÿ���Դϴ�");
		break;
	}
}

void UEngineConstantBuffer::ChangeData(const void* _Data, UINT _Size)
{
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
	memcpy_s(Data.pData, BufferInfo.ByteWidth, _Data, BufferInfo.ByteWidth);
	
	// �ٽ�� �ٽ� ���
	GEngine->GetDirectXContext()->Unmap(Buffer, 0);

}