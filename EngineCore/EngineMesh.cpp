#include "PreCompile.h"
#include "EngineMesh.h"
#include "EngineCore.h"

UEngineMesh::UEngineMesh() 
{
}

UEngineMesh::~UEngineMesh() 
{
}

void UEngineMesh::ResCreate(std::string_view _VertexName, std::string_view _IndexName)
{
	VertexBuffer = UEngineVertexBuffer::FindRes(_VertexName);
	IndexBuffer = UEngineIndexBuffer::FindRes(_IndexName);

	if (nullptr == VertexBuffer || nullptr == IndexBuffer)
	{
		MsgBoxAssert("�Ž� ������ �����߽��ϴ�.");
		return;
	}


}

void UEngineMesh::InputAssembler1Setting()
{
#ifdef _DEBUG
	if (nullptr == VertexBuffer)
	{
		MsgBoxAssert("���ؽ� ���۰� ���õ��� ���� �Ž��Դϴ�" + GetName());
		return;
	}
#endif

	VertexBuffer->Setting();
}

void UEngineMesh::InputAssembler2Setting()
{
#ifdef _DEBUG
	if (nullptr == IndexBuffer)
	{
		MsgBoxAssert("�ε��� ���۰� ���õ��� ���� �Ž��Դϴ�" + GetName());
		return;
	}
#endif

	IndexBuffer->Setting();

	// D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST �ﰢ������ �׷���.
	GEngine->GetDirectXContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void UEngineMesh::IndexedDraw()
{
	// 10������ ���ؽ��� �̷���� �Ž��� �ִٰ�ġ��.
	// ������ �� �ִ°Ŵ�.
	// �Ӹ�
	// ��
	// ����
	// �ٸ�
	// �� ���� ����ٺ��� �Ӹ��� ����Ҷ��� �ִµ�?
	// ���ʿ� �Ž����� �ƿ� �� �ɰ��� ����� ����.
	// �ϳ��� �Ž��ε�. �Ӹ��� ��߰ڴµ�?
	// 50�� 53�� ������ �Ӹ� ���ؽ���.
	// GEngine->GetDirectXContext()->DrawIndexed(IndexBuffer->Count, 500000, 480000);

	GEngine->GetDirectXContext()->DrawIndexed(IndexBuffer->Count, 0, 0);
}

void UEngineMesh::DrawIndexedInstanced(int _InstancingCount)
{
	GEngine->GetDirectXContext()->DrawIndexedInstanced(IndexBuffer->Count, _InstancingCount, 0, 0, 0);
}