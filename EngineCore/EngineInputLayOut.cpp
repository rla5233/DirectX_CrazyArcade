#include "PreCompile.h"
#include "EngineInputLayOut.h"
#include "EngineCore.h"
#include "EngineVertex.h"
#include "EngineVertexBuffer.h"
#include "EngineVertexShader.h"

UEngineInputLayOut::UEngineInputLayOut() 
{
}

UEngineInputLayOut::~UEngineInputLayOut() 
{
	if (nullptr != LayOut)
	{
		LayOut->Release();
	}
}

// ���̴��� ������ ������ ���ð̴ϴ�. ABC ���̴��� �ִ�.
// ���ؽ��� �������� ���ü� �ִ�. DEG ���ؽ��� �ִٰ� Ĩ�ô�.
// A D�� �������� ��������� ��ǲ ���̾ƿ��� A D�� �������� ������� ������ ȯ�濡���� ����� ��������.

void UEngineInputLayOut::ResCreate(std::shared_ptr<UEngineVertexBuffer> _Buffer, std::shared_ptr<UEngineVertexShader> _Shader)
{
	if (nullptr == _Buffer)
	{
		MsgBoxAssert("��ǲ���̾ƿ��� ���鶧 ���۰� nullptr �̾����ϴ�.");
	}

	// ���ؽ� ����ü�� ��� �ִ�
	// ���ּҰ��� ���ؽ� ���۰� ������ �ִ�.
	// UEngineVertexBuffer
	UVertexLayOutInfo* Ptr = _Buffer->LayOutInfoPtr;
	std::vector<D3D11_INPUT_ELEMENT_DESC>& Desc = Ptr->Descs;

	if (nullptr == _Shader)
	{
		MsgBoxAssert("��ǲ���̾ƿ��� ���鶧 ���̴��� nullptr �̾����ϴ�.");
	}

	// ���ؽ� ���̴��� ��� �ִ�.
	// UEngineVertexShader
	ID3DBlob* VertexShaderCodeBlob = _Shader->ShaderCodeBlob;

	HRESULT Result = GEngine->GetDirectXDevice()->CreateInputLayout(
		&Desc[0], 
		static_cast<UINT>(Desc.size()), 
		VertexShaderCodeBlob->GetBufferPointer(), 
		VertexShaderCodeBlob->GetBufferSize(),
		&LayOut
	);

	if (S_OK != Result)
	{
		MsgBoxAssert("��ǲ ���̾ƿ� ������ �����߽��ϴ�.");
		return;
	}
}

void UEngineInputLayOut::Setting()
{
#ifdef _DEBUG
	if (nullptr == LayOut)
	{
		MsgBoxAssert("����������� ���� ��ǲ���̾ƿ��� �����Ҽ��� �����ϴ�.");
	}
#endif

	GEngine->GetDirectXContext()->IASetInputLayout(LayOut);
}