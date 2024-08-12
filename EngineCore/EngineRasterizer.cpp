#include "PreCompile.h"
#include "EngineRasterizer.h"
#include "EngineCore.h"

UEngineRasterizer::UEngineRasterizer() 
{
}

UEngineRasterizer::~UEngineRasterizer() 
{
	if (nullptr != State)
	{
		State->Release();
	}
}

void UEngineRasterizer::ResCreate(const D3D11_RASTERIZER_DESC& _Info)
{
	HRESULT Result =  GEngine->GetDirectXDevice()->CreateRasterizerState(&_Info, &State);
#ifdef _DEBUG
	if (S_OK != Result)
	{
		MsgBoxAssert("�����Ͷ����� ������ �����߽��ϴ�.");
		return;
	} 
#endif
}

void UEngineRasterizer::Setting()
{
#ifdef _DEBUG
	if (nullptr == State)
	{
		MsgBoxAssert("��������� ���� �����Ͷ������� �����Ϸ��� �߽��ϴ�" + GetName());
	}
#endif

	GEngine->GetDirectXContext()->RSSetState(State);
}