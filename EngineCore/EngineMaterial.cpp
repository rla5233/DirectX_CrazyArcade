#include "PreCompile.h"
#include "EngineMaterial.h"
#include "EngineVertexShader.h"
#include "EnginePixelShader.h"
#include "EngineRasterizer.h"
#include <EngineBase/EngineDebug.h>

UEngineMaterial::UEngineMaterial() 
{
	SetRasterizer("EngineBase");
	SetBlend("EngineBase");
}

UEngineMaterial::~UEngineMaterial() 
{
}

void UEngineMaterial::SetVertexShader(std::string_view _Name)
{
	VertexShader = UEngineVertexShader::FindRes(_Name);
	if (nullptr == VertexShader)
	{
		MsgBoxAssert("�������� �ʴ� ���ؽ� ���̴��� �����Ϸ��� �߽��ϴ�.");
		return;
	}
}

void UEngineMaterial::SetPixelShader(std::string_view _Name)
{
	PixelShader = UEnginePixelShader::FindRes(_Name);
	if (nullptr == PixelShader)
	{
		MsgBoxAssert("�������� �ʴ� �ȼ����̴��� �����Ϸ��� �߽��ϴ�.");
		return;
	}
}

void UEngineMaterial::SetRasterizer(std::string_view _Name)
{
	Rasterizer = UEngineRasterizer::FindRes(_Name);
	if (nullptr == Rasterizer)
	{
		MsgBoxAssert("�������� �ʴ� �����Ͷ������� �����Ϸ��� �߽��ϴ�.");
		return;
	}
}

void UEngineMaterial::SetBlend(std::string_view _Name)
{
	Blend = UEngineBlend::FindRes(_Name);
	if (nullptr == Blend)
	{
		MsgBoxAssert("�������� �ʴ� ���� �����Ϸ��� �߽��ϴ�.");
		return;
	}
}

//void UEngineMaterial::Setting()
//{
//	// ������ ������� �ñ⵵ �������.
//	// �������ִ°� �߿�����.
//	PixelShader->Setting();
//	VertexShader->Setting();
//	Rasterizer->Setting();
//}

void UEngineMaterial::VertexShaderSetting()
{
#ifdef _DEBUG
	if (nullptr == VertexShader)
	{
		MsgBoxAssert("���ؽ� ���̴��� �������� �ʰ� ������ �Ϸ��� �߽��ϴ�.");
		return;
	}
#endif

	VertexShader->Setting();
}

void UEngineMaterial::RasterizerSetting()
{
#ifdef _DEBUG
	if (nullptr == Rasterizer)
	{
		MsgBoxAssert("�����Ͷ������� �������� �ʰ� ������ �Ϸ��� �߽��ϴ�.");
		return;
	}
#endif

	Rasterizer->Setting();
}

void UEngineMaterial::PixelShaderSetting()
{
#ifdef _DEBUG
	if (nullptr == PixelShader)
	{
		MsgBoxAssert("�ȼ����̴��� �������� �ʰ� ������ �Ϸ��� �߽��ϴ�.");
		return;
	}
#endif

	PixelShader->Setting();
}

void UEngineMaterial::BlendSetting()
{
#ifdef _DEBUG
	if (nullptr == Blend)
	{
		MsgBoxAssert("���� �������� �ʰ� ������ �Ϸ��� �߽��ϴ�.");
		return;
	}
#endif

	Blend->Setting();

}