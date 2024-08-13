#include "PreCompile.h"
#include "EngineVertexBuffer.h"
#include "EngineCore.h"
#include "EngineGraphicDevice.h"

UEngineVertexBuffer::UEngineVertexBuffer() 
{
}

UEngineVertexBuffer::~UEngineVertexBuffer() 
{
}

void UEngineVertexBuffer::ResCreate(const void* _Data, UINT _Size, UINT _Count)
{
	// ���̷�Ʈ���� � �޸𸮸� �Ҵ��Ҷ� �ʱⰪ�� �־��ְ� ������
	// �̰ɷ� �־���� �մϴ�.
	D3D11_SUBRESOURCE_DATA Data;
	Data.pSysMem = _Data;

	Size = _Size;
	Count = _Count;

	// ũ��
	BufferInfo.ByteWidth = static_cast<UINT>(Size * Count);
	// �뵵
	BufferInfo.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	// CPU���� ���ٰ����Ѱ�?
	BufferInfo.CPUAccessFlags = 0;
	// ��������Ұž�?
	// D3D11_USAGE_DEFAULT �׷���ī�忡 ��������.
	// D3D11_USAGE_DYNAMIC �ϴ� �׷���ī�� ���� ������ Ram
	BufferInfo.Usage = D3D11_USAGE_DEFAULT;


	// CreateVertexBuffer ���� �Լ��� �ƶ�
	// ���۰迭�� �Ʒ��� �Լ��� ����
	if (S_OK != GEngine->GetDirectXDevice()->CreateBuffer(&BufferInfo, &Data, &Buffer))
	{
		MsgBoxAssert("���ؽ� ���� ������ �����߽��ϴ�.");
		return;
	}
}

void UEngineVertexBuffer::Setting()
{
#ifdef _DEBUG
	if (nullptr == Buffer)
	{
		MsgBoxAssert("��������� ���� ���ؽ� ���۸� �����Ϸ��� �߽��ϴ�" + GetName());
	}
#endif

	// std::vector<ID3D11Buffer*> Buffer

	// �ѹ��� �������� ���ؽ� ���۸� �������ټ� �ֽ��ϴ�.
	//UINT StartSlot, // ���ؽ� ���۰� ������ ���õƴٸ� ���� ����� ���ۺ��� �����Ұų�? �츮�� �׷� �������� 0
	//UINT NumBuffers, // �׿������� ���ؽ� ���۰� �����? ������ �������� �ʾƼ� 1
	//ID3D11Buffer* const* ppVertexBuffers, ������ ������ �迭�� �־��ֶ�. ������ �츰 ���۸� 1�� �������
	//const UINT* pStrides, ������ ������
	//const UINT* pOffsets ���� 4����� ���� ���°���� �ұ�

	// �츰 �׳� 1���� ���ؽ� ���۸� �����ϴ� ���������Ѵ�.
	GEngine->GetDirectXContext()->IASetVertexBuffers(0, 1, &Buffer, &Size, &Offset);
}