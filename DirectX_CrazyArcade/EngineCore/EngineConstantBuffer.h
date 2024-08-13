#pragma once
#include <EnginePlatform/EngineResources.h>
#include <map>

#include "EngineBuffer.h"
#include "EngineShader.h"
#include "EngineEnums.h"

class UEngineConstantBufferSetter;
// ���� :
class UEngineConstantBuffer : public UEngineResources<UEngineConstantBuffer>, public UEngineBuffer
{
	friend UEngineConstantBufferSetter;

public:
	// constrcuter destructer
	UEngineConstantBuffer();
	~UEngineConstantBuffer();

	// delete Function
	UEngineConstantBuffer(const UEngineConstantBuffer& _Other) = delete;
	UEngineConstantBuffer(UEngineConstantBuffer&& _Other) noexcept = delete;
	UEngineConstantBuffer& operator=(const UEngineConstantBuffer& _Other) = delete;
	UEngineConstantBuffer& operator=(UEngineConstantBuffer&& _Other) noexcept = delete;

	// ������ �����
	// ������ ã�Ƽ� �����մϴ�.
	static std::shared_ptr<UEngineConstantBuffer> CreateAndFind(EShaderType _Type, std::string_view _Name, UINT _ByteSize)
	{
		if (0 >= _ByteSize)
		{
			MsgBoxAssert("����Ʈ ũ�Ⱑ 0���� ���� ������۴� ����� �����ϴ�.");
		}

		std::string UpperName = UEngineString::ToUpper(_Name);

		if (false == ConstantBuffers.contains(_Type))
		{
			// ������ ��������.
			ConstantBuffers[_Type];
		}


		if (false == ConstantBuffers[_Type].contains(UpperName))
		{
			// ������ ��������.
			ConstantBuffers[_Type][UpperName];
		}

		// Ftransform�̶�� �̸��� ���� �׷��� �־�?
		// �������� ���� �������Ƚ��ϴ�.
		std::map<int, std::shared_ptr<UEngineConstantBuffer>>& Buffers = ConstantBuffers[_Type][UpperName];

		// Ftransform �̸��� ���� 500����Ʈ ¥�� ������۰� �־�?
		if (true == Buffers.contains(_ByteSize))
		{
			return Buffers[_ByteSize];
		}

		std::shared_ptr<UEngineConstantBuffer> Res = CreateResUnName();
		Res->SetName(_Name);
		Res->ResCreate(_ByteSize);
		Buffers[_ByteSize] = Res;
		return Res;
	}

	void ChangeData(const void* _Data, UINT _Size);

protected:

private:
	static std::map<EShaderType, std::map<std::string, std::map<int, std::shared_ptr<UEngineConstantBuffer>>>> ConstantBuffers;

	void ResCreate(UINT _ByteSize);

	void Setting(EShaderType _Type, UINT _Slot);
	void Reset(EShaderType _Type, UINT _Slot);

};

