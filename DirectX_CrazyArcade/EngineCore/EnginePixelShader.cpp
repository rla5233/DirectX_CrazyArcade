#include "PreCompile.h"
#include "EnginePixelShader.h"
#include <EngineBase/EngineString.h>
#include "EngineCore.h"

UEnginePixelShader::UEnginePixelShader() 
{
	Type = EShaderType::Pixel;
}

UEnginePixelShader::~UEnginePixelShader() 
{
	if (nullptr != ShaderPtr)
	{
		ShaderPtr->Release();
	}
}

void UEnginePixelShader::ResLoad(std::string_view _EntryPoint, UINT _High /*= 5*/, UINT _Low /*= 0*/)
{
	
	// ������ typedef ���Ĵٵ�
	// LPCWSTR => const wchar_t* _Path
	// LP  => �����ͳ־��ּ���
	// C => const
	// W => wchar_t
	// STR => ���ڿ�

	// LPCSTR => const char* _Path
	// LP  => �����ͳ־��ּ���
	// C => const
	// W ���� => char
	// STR => ���ڿ�
	
   //LPCWSTR pFileName, <= ���̴� ���� ���
   //CONST D3D_SHADER_MACRO* pDefines,
   //ID3DInclude* pInclude,
   //LPCSTR pEntrypoint,
   //LPCSTR pTarget,
   //UINT Flags1,
   //UINT Flags2,
   //ID3DBlob** ppCode,

	EntryName = _EntryPoint;

	std::string Path = GetPath();
	const char* PathPtr = Path.c_str();
	std::wstring WPath = UEngineString::AnsiToUniCode(PathPtr);
	const wchar_t* WPathPtr = WPath.c_str();

	CONST D3D_SHADER_MACRO* pDefines = nullptr;
	// �ּҰ��� 1�� �����͸� �־��ذǵ�.
	// ���̷�Ʈ�� �� �׳� ���̴����Ͼȿ� �ִ� include �׳� ���ڴٰ� ok
	ID3DInclude* pInclude = D3D_COMPILE_STANDARD_FILE_INCLUDE;
	const char* pEntrypoint = _EntryPoint.data();

	// ������ 
	std::string Version = "ps_" + std::to_string(_High) + "_" + std::to_string(_Low);

	// ���̴��� ������ �ֽ��ϴ�.
	// ���� 5_0���� �Ϸ��� �մϴ�.
	const char* Target = Version.c_str();

	int Flag0 = 0;

#ifdef _DEBUG
	// ��Ÿ���� �ƴ� D3D10
	Flag0 = D3D10_SHADER_DEBUG;
#endif
	// ���߿� ����� ���̴��� �˷��ٶ� ����� ��ġ�Ǽ� ���°�
	// ���°ž� ���� ���̴µ��� ���ϴ�.
	// ��ġ�Ǵ� ���� ���� ��.
	// �޸� ������ �׷���ī�尡 �����̶�� �մϴ�.
	Flag0 |= D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;

	int Flag1 = 0;

	HRESULT Result = D3DCompileFromFile(
		WPathPtr,
		pDefines,
		pInclude,
		pEntrypoint,
		Target,
		Flag0,
		Flag1,
		&ShaderCodeBlob,
		&ErrorCodeBlob
	);
	// ErrorCodeBlob������ ���� ������ ���� ����.

	if (S_OK != Result)
	{
		std::string ErrorString = reinterpret_cast<char*>(ErrorCodeBlob->GetBufferPointer());
		MsgBoxAssert(ErrorString);
		return;
	}

	Result = GEngine->GetDirectXDevice()->CreatePixelShader(
		ShaderCodeBlob->GetBufferPointer(), 
		ShaderCodeBlob->GetBufferSize(),
		nullptr,
		&ShaderPtr
		);

	if (S_OK != Result)
	{
		MsgBoxAssert("���̴� ������ �����߽��ϴ�.");
		return;
	}

	ShaderResCheck();
}

void UEnginePixelShader::Setting()
{
#ifdef _DEBUG
	if (nullptr == ShaderPtr)
	{
		MsgBoxAssert("��������� ���� �ȼ� ���̴��� �����Ϸ��� �߽��ϴ�" + GetName());
	}
#endif

	GEngine->GetDirectXContext()->PSSetShader(ShaderPtr, nullptr, 0);
}