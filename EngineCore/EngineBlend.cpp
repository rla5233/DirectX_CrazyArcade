#include "PreCompile.h"
#include "EngineBlend.h"
#include "EngineCore.h"

UEngineBlend::UEngineBlend() 
{
}

UEngineBlend::~UEngineBlend() 
{
	if (nullptr != State)
	{
		State->Release();
	}
}


void UEngineBlend::ResCreate(const D3D11_BLEND_DESC& _Info)
{
	HRESULT Result = GEngine->GetDirectXDevice()->CreateBlendState(&_Info, &State);
#ifdef _DEBUG
	if (S_OK != Result)
	{
		MsgBoxAssert("���� ������ �����߽��ϴ�.");
		return;
	}
#endif
}

void UEngineBlend::Setting()
{
#ifdef _DEBUG
	if (nullptr == State)
	{
		MsgBoxAssert("��������� ���� ���带 �����Ϸ��� �߽��ϴ�" + GetName());
	}
#endif

	// ���尡 ���� ��ĥ ����
	GEngine->GetDirectXContext()->OMSetBlendState(State, Factor.Arr1D, Mask);
}