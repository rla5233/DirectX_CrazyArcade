#include "PreCompile.h"
#include "EngineShaderResources.h"
#include "EngineConstantBuffer.h"
#include "EngineStructuredBuffer.h"
#include "EngineTexture.h"
#include "EngineSampler.h"


UEngineShaderResources::~UEngineShaderResources()
{
	int a = 0;
}
/// UEngineConstantBufferSetter
void UEngineConstantBufferSetter::Setting()
{
	// ������۸� �����Ѵ�.

	if (nullptr == CPUData)
	{
		MsgBoxAssert(Res->GetName() + " ������ۿ� ������ ������ �ʾҽ��ϴ�. ���ּ���...");
		return;
	}

	Res->ChangeData(CPUData, BufferSize);

	Res->Setting(Type, Slot);
}

void UEngineConstantBufferSetter::Reset()
{
	Res->Reset(Type, Slot);
}


void UEngineStructuredBufferSetter::PushData(const void* _Data, UINT _Size)
{
	Ser.Write(_Data, _Size);
}

void UEngineStructuredBufferSetter::Setting()
{
	// 672 * 3000

	if (0 == Ser.WriteSize())
	{
		MsgBoxAssert(Res->GetName() + " ����ȭ ���ۿ� ũ�Ⱑ 0�Դϴ�.");
		return;
	}

	Res->ChangeData(Ser.DataPtr(), Ser.WriteSize());

	Res->Setting(Type, Slot);
}
void UEngineStructuredBufferSetter::Reset()
{
	Res->Reset(Type, Slot);
	Ser.ResetWrite();
}


void UEngineTextureSetter::Setting()
{
#ifdef _DEBUG
	if (nullptr == Res)
	{
		MsgBoxAssert(GetName() + " �ؽ�ó�� ���������� �ʾҽ��ϴ�.");
	}
#endif

	Res->Setting(Type, Slot);
}

void UEngineTextureSetter::Reset()
{
#ifdef _DEBUG
	if (nullptr == Res)
	{
		MsgBoxAssert(GetName() + " �ؽ�ó�� ���������� �ʾҽ��ϴ�.");
	}
#endif

	Res->Reset(Type, Slot);
}


void UEngineSamplerSetter::Setting()
{
#ifdef _DEBUG
	if (nullptr == Res)
	{
		MsgBoxAssert(GetName() + " ���÷��� ���������� �ʾҽ��ϴ�.");
	}
#endif

	Res->Setting(Type, Slot);
}

void UEngineSamplerSetter::Reset()
{
#ifdef _DEBUG
	if (nullptr == Res)
	{
		MsgBoxAssert(GetName() + " ���÷��� ���������� �ʾҽ��ϴ�.");
	}
#endif

	Res->Reset(Type, Slot);
}


///

void UEngineShaderResources::ShaderResourcesCheck(EShaderType _Type, std::string_view _EntryName, ID3DBlob* _ShaderCode)
{
	// std::string_view _EntryName <= ���� ���̴��� ���� ���ҽ��� �ִ��� Ȯ���Ϸ���

	if (nullptr == _ShaderCode)
	{
		MsgBoxAssert("�������� ������ ���̴����� ���ҽ� üũ�� �Ϸ��� �߽��ϴ�");
		return;
	}

	ID3DBlob* ShaderCode = _ShaderCode;
	// ���̷�Ʈ x�� �ڵ��˻� ��ɰ� �Լ��� �������ݴϴ�.
	// �츰 �̿��ϱ⸸ �ϸ� �˴ϴ�.

	// Reflection <= ���� ���� �ɰ��̴�.
	// �������� ������� ���մϴ�. ���α׷��ֿ���

	ID3D11ShaderReflection* CompileInfo = nullptr;

	// ���̴��� �����ϵ� ������ �м��� ����ȭ �ؼ� �츮���� �����մϴ�.
	if (S_OK != D3DReflect(ShaderCode->GetBufferPointer(), ShaderCode->GetBufferSize(), IID_ID3D11ShaderReflection, reinterpret_cast<void**>(&CompileInfo)))
	{
		MsgBoxAssert("���̴� ���������� �����߽��ϴ�");
		return;
	}

	D3D11_SHADER_DESC Info = {};

	CompileInfo->GetDesc(&Info);


	// Info.BoundResources ���̴� �ȿ��� ����� �� ���ҽ� ����
	// ���δ� ���ļ� �Դϴ�.
	for (UINT i = 0; i < Info.BoundResources; i++)
	{
		D3D11_SHADER_INPUT_BIND_DESC ResDesc;

		CompileInfo->GetResourceBindingDesc(i, &ResDesc);

		D3D_SHADER_INPUT_TYPE Type = ResDesc.Type;

		std::string UpperName = UEngineString::ToUpper(ResDesc.Name);

		switch (Type)
		{
		case D3D_SIT_CBUFFER:
		{
			// ��������� ������ ������ �˷���
			ID3D11ShaderReflectionConstantBuffer* BufferInfo = CompileInfo->GetConstantBufferByName(ResDesc.Name);

			D3D11_SHADER_BUFFER_DESC ConstantBufferDesc = {};

			BufferInfo->GetDesc(&ConstantBufferDesc);

			_EntryName;

			// ������۴� �̸��� �߿��Ѱ� �ƴ϶�
			// ����Ʈ�� �߿���.
			std::shared_ptr<UEngineConstantBuffer> Buffer = UEngineConstantBuffer::CreateAndFind(_Type, ResDesc.Name, ConstantBufferDesc.Size);

			UEngineConstantBufferSetter& NewSetter = ConstantBuffers[_Type][UpperName];
			NewSetter.SetName(ResDesc.Name);
			NewSetter.Type = _Type;
			NewSetter.Slot = ResDesc.BindPoint;
			NewSetter.BufferSize = ConstantBufferDesc.Size;
			NewSetter.Res = Buffer;
			break;
		}
		case D3D_SIT_TEXTURE:
		{
			ResDesc.Name;
			UEngineTextureSetter& NewSetter = Textures[_Type][UpperName];
			NewSetter.SetName(ResDesc.Name);
			NewSetter.Type = _Type;
			NewSetter.Slot = ResDesc.BindPoint;
			break;
		}
		case D3D_SIT_SAMPLER:
		{
			ResDesc.Name;
			UEngineSamplerSetter& NewSetter = Samplers[_Type][UpperName];
			NewSetter.SetName(ResDesc.Name);
			NewSetter.Type = _Type;
			NewSetter.Slot = ResDesc.BindPoint;
			break;
		}
		case D3D_SIT_STRUCTURED:
		{
			ID3D11ShaderReflectionConstantBuffer* BufferInfo = CompileInfo->GetConstantBufferByName(ResDesc.Name);
			D3D11_SHADER_BUFFER_DESC ConstantBufferDesc = {};
			BufferInfo->GetDesc(&ConstantBufferDesc);
			_EntryName;

			ResDesc.Name;
			UEngineStructuredBufferSetter& NewSetter = StructuredBuffers[_Type][UpperName];
			NewSetter.SetName(ResDesc.Name);
			NewSetter.Type = _Type;
			NewSetter.Slot = ResDesc.BindPoint;
			NewSetter.BufferSize = ConstantBufferDesc.Size;

			std::shared_ptr<UEngineStructuredBuffer> Buffer = UEngineStructuredBuffer::CreateAndFind(_Type, ResDesc.Name, NewSetter.BufferSize);
			NewSetter.Res = Buffer;

			break;
		}
		default:
			MsgBoxAssert("ó���Ҽ� ���� Ÿ���Դϴ�.");
			break;
		}


		int a = 0;
	}

	CompileInfo->Release();

	int a = 0;
}

void UEngineShaderResources::SettingConstantBuffer(std::string_view _Name, const void* _Data, UINT _Size)
{
	std::string UpperName = UEngineString::ToUpper(_Name);
	if (false == IsConstantBuffer(UpperName))
	{
		MsgBoxAssert("���������� �ʴ� ������ۿ� �����Ϸ��� �߽��ϴ�." + UpperName);
		return;
	}


	for (std::pair<const EShaderType, std::map<std::string, UEngineConstantBufferSetter>>& Pair : ConstantBuffers)
	{
		std::map<std::string, UEngineConstantBufferSetter>& ResMap = Pair.second;

		if (false ==  ResMap.contains(UpperName))
		{
			continue;
		}

		UEngineConstantBufferSetter& Setter = ResMap[UpperName];

		if (Setter.BufferSize != _Size)
		{
			MsgBoxAssert(Setter.Res->GetName() + "�� ����Ʈ ũ�Ⱑ �ٸ��ϴ�." + std::to_string(Setter.BufferSize) + " Vs " + std::to_string(_Size));
		}

		Setter.CPUData = _Data;
	}
}

bool UEngineShaderResources::IsConstantBuffer(std::string_view _Name)
{
	std::string UpperName = UEngineString::ToUpper(_Name);

	for (std::pair<const EShaderType, std::map<std::string, UEngineConstantBufferSetter>>& Pair : ConstantBuffers)
	{
		std::map<std::string, UEngineConstantBufferSetter>& ResMap = Pair.second;

		if (true == ResMap.contains(UpperName))
		{
			return true;
		}
	}

	return false;
}

UEngineStructuredBufferSetter* UEngineShaderResources::GetStructuredBuffer(std::string_view _ResName)
{
	std::string UpperName = UEngineString::ToUpper(_ResName);

	for (std::pair<const EShaderType, std::map<std::string, UEngineStructuredBufferSetter>>& Pair : StructuredBuffers)
	{
		std::map<std::string, UEngineStructuredBufferSetter>& ResMap = Pair.second;

		if (true == ResMap.contains(UpperName))
		{
			return &ResMap[UpperName];
		}
	}

	return nullptr;

}

void UEngineShaderResources::SettingAllShaderResources()
{
	for (std::pair<const EShaderType, std::map<std::string, UEngineConstantBufferSetter>>& Pair : ConstantBuffers)
	{
		std::map<std::string, UEngineConstantBufferSetter>& ResMap = Pair.second;

		for (std::pair<const std::string, UEngineConstantBufferSetter>& Setter : ResMap)
		{
			Setter.second.Setting();
		}
	}

	for (std::pair<const EShaderType, std::map<std::string, UEngineTextureSetter>>& Pair : Textures)
	{
		std::map<std::string, UEngineTextureSetter>& ResMap = Pair.second;

		for (std::pair<const std::string, UEngineTextureSetter>& Setter : ResMap)
		{
			Setter.second.Setting();
		}
	}

	for (std::pair<const EShaderType, std::map<std::string, UEngineSamplerSetter>>& Pair : Samplers)
	{
		std::map<std::string, UEngineSamplerSetter>& ResMap = Pair.second;

		for (std::pair<const std::string, UEngineSamplerSetter>& Setter : ResMap)
		{
			Setter.second.Setting();
		}
	}

	for (std::pair<const EShaderType, std::map<std::string, UEngineStructuredBufferSetter>>& Pair : StructuredBuffers)
	{
		std::map<std::string, UEngineStructuredBufferSetter>& ResMap = Pair.second;

		for (std::pair<const std::string, UEngineStructuredBufferSetter>& Setter : ResMap)
		{
			Setter.second.Setting();
		}
	}

}


void UEngineShaderResources::ResetAllShaderResources()
{
	for (std::pair<const EShaderType, std::map<std::string, UEngineConstantBufferSetter>>& Pair : ConstantBuffers)
	{
		std::map<std::string, UEngineConstantBufferSetter>& ResMap = Pair.second;

		for (std::pair<const std::string, UEngineConstantBufferSetter>& Setter : ResMap)
		{
			Setter.second.Reset();
		}
	}

	for (std::pair<const EShaderType, std::map<std::string, UEngineTextureSetter>>& Pair : Textures)
	{
		std::map<std::string, UEngineTextureSetter>& ResMap = Pair.second;

		for (std::pair<const std::string, UEngineTextureSetter>& Setter : ResMap)
		{
			Setter.second.Reset();
		}
	}

	for (std::pair<const EShaderType, std::map<std::string, UEngineSamplerSetter>>& Pair : Samplers)
	{
		std::map<std::string, UEngineSamplerSetter>& ResMap = Pair.second;

		for (std::pair<const std::string, UEngineSamplerSetter>& Setter : ResMap)
		{
			Setter.second.Reset();
		}
	}

	for (std::pair<const EShaderType, std::map<std::string, UEngineStructuredBufferSetter>>& Pair : StructuredBuffers)
	{
		std::map<std::string, UEngineStructuredBufferSetter>& ResMap = Pair.second;

		for (std::pair<const std::string, UEngineStructuredBufferSetter>& Setter : ResMap)
		{
			Setter.second.Reset();
		}
	}

}

void UEngineShaderResources::SettingTexture(std::string_view _TexName, std::string_view _ImageName, std::string_view _SamperName)
{
	std::shared_ptr<UEngineTexture> FindTexture = UEngineTexture::FindRes(_ImageName);
	SettingTexture(_TexName, FindTexture, _SamperName);
}

void UEngineShaderResources::SettingTexture(std::string_view _TexName, std::shared_ptr<UEngineTexture> _Texture, std::string_view _SamperName)
{
	std::shared_ptr<UEngineTexture> FindTexture = _Texture;
	std::shared_ptr<UEngineSampler> FindSampler = UEngineSampler::FindRes(_SamperName);

	if (nullptr == FindTexture)
	{
		MsgBoxAssert("�������� �ʴ� �ؽ�ó�� �����Ϸ��� �߽��ϴ�.");
		return;
	}

	if (nullptr == FindSampler)
	{
		MsgBoxAssert("�������� �ʴ� ���÷��� �����Ϸ��� �߽��ϴ�.");
		return;
	}

	std::string UpperName = UEngineString::ToUpper(_TexName);
	std::string SmpUpperName = UpperName + "_SAMPLER";

	for (std::pair<const EShaderType, std::map<std::string, UEngineTextureSetter>>& Pair : Textures)
	{
		std::map<std::string, UEngineTextureSetter>& TexMap = Pair.second;
		std::map<std::string, UEngineSamplerSetter>& SmpMap = Samplers[Pair.first];

		// ���÷��� �ؽ�ó�� �ѽ��� �ƴϸ� ������ü�� ���� �ʴ±��� 
		if (false == TexMap.contains(UpperName))
		{
			continue;
		}

		if (false == SmpMap.contains(SmpUpperName))
		{
			MsgBoxAssert("�ؽ�ó�� �ѽ��� ���÷��� �������� �ʽ��ϴ�");
			continue;
		}

		UEngineTextureSetter& TexSetter = TexMap[UpperName];
		UEngineSamplerSetter& SmpSetter = SmpMap[SmpUpperName];

		TexSetter.Res = FindTexture;
		SmpSetter.Res = FindSampler;
	}
}

void UEngineShaderResources::Reset()
{
	ConstantBuffers.clear();
	Textures.clear();
	Samplers.clear();
}

void UEngineShaderResources::OtherResCopy(std::shared_ptr<UEngineShaderResources> _Resource)
{
	// ��� ���ҽ��� �� ���ҽ��� ���ؼ� ������ ������ �� ���ҽ��� �����ؿ´�.
	for (std::pair<const EShaderType, std::map<std::string, UEngineTextureSetter>> Pair : _Resource->Textures)
	{
		if (false == Textures.contains(Pair.first))
		{
			continue;
		}

		std::map<std::string, UEngineTextureSetter>& OtherSetters = Pair.second;
		std::map<std::string, UEngineTextureSetter>& ThisSetters = Textures[Pair.first];

		for (std::pair<const std::string, UEngineTextureSetter>& SetterPair : OtherSetters)
		{
			if (false == ThisSetters.contains(SetterPair.first))
			{
				continue;
			}

			ThisSetters[SetterPair.first].Res = SetterPair.second.Res;
		}
	}

	for (std::pair<const EShaderType, std::map<std::string, UEngineSamplerSetter>> Pair : _Resource->Samplers)
	{
		if (false == Textures.contains(Pair.first))
		{
			continue;
		}

		std::map<std::string, UEngineSamplerSetter>& OtherSetters = Pair.second;
		std::map<std::string, UEngineSamplerSetter>& ThisSetters = Samplers[Pair.first];

		for (std::pair<const std::string, UEngineSamplerSetter>& SetterPair : OtherSetters)
		{
			if (false == ThisSetters.contains(SetterPair.first))
			{
				continue;
			}

			ThisSetters[SetterPair.first].Res = SetterPair.second.Res;
		}
	}

	for (std::pair<const EShaderType, std::map<std::string, UEngineConstantBufferSetter>> Pair : _Resource->ConstantBuffers)
	{
		if (false == Textures.contains(Pair.first))
		{
			continue;
		}

		std::map<std::string, UEngineConstantBufferSetter>& OtherSetters = Pair.second;
		std::map<std::string, UEngineConstantBufferSetter>& ThisSetters = ConstantBuffers[Pair.first];

		for (std::pair<const std::string, UEngineConstantBufferSetter>& SetterPair : OtherSetters)
		{
			if (false == ThisSetters.contains(SetterPair.first))
			{
				continue;
			}

			ThisSetters[SetterPair.first].Res = SetterPair.second.Res;
		}
	}


	for (std::pair<const EShaderType, std::map<std::string, UEngineStructuredBufferSetter>> Pair : _Resource->StructuredBuffers)
	{
		if (false == Textures.contains(Pair.first))
		{
			continue;
		}

		std::map<std::string, UEngineStructuredBufferSetter>& OtherSetters = Pair.second;
		std::map<std::string, UEngineStructuredBufferSetter>& ThisSetters = StructuredBuffers[Pair.first];

		for (std::pair<const std::string, UEngineStructuredBufferSetter>& SetterPair : OtherSetters)
		{
			if (false == ThisSetters.contains(SetterPair.first))
			{
				continue;
			}

			ThisSetters[SetterPair.first].Res = SetterPair.second.Res;
		}
	}

	
}