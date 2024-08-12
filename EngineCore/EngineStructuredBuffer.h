#pragma once
#include <EnginePlatform/EngineResources.h>
#include <map>

#include "EngineBuffer.h"
#include "EngineShader.h"
#include "EngineEnums.h"

// ���� :
class UEngineStructuredBufferSetter;
class UEngineStructuredBuffer : public UEngineResources<UEngineStructuredBuffer>, public UEngineBuffer
{
	friend UEngineStructuredBufferSetter;

public:
	// constrcuter destructer
	UEngineStructuredBuffer();
	~UEngineStructuredBuffer();

	// delete Function
	UEngineStructuredBuffer(const UEngineStructuredBuffer& _Other) = delete;
	UEngineStructuredBuffer(UEngineStructuredBuffer&& _Other) noexcept = delete;
	UEngineStructuredBuffer& operator=(const UEngineStructuredBuffer& _Other) = delete;
	UEngineStructuredBuffer& operator=(UEngineStructuredBuffer&& _Other) noexcept = delete;

	// ������ �����
	// ������ ã�Ƽ� �����մϴ�.
	static std::shared_ptr<UEngineStructuredBuffer> CreateAndFind(EShaderType _Type, std::string_view _Name, UINT _ByteSize)
	{
		if (0 >= _ByteSize)
		{
			MsgBoxAssert("����Ʈ ũ�Ⱑ 0���� ���� ������۴� ����� �����ϴ�.");
		}

		std::string UpperName = UEngineString::ToUpper(_Name);

		if (false == StructuredBuffers.contains(_Type))
		{
			// ������ ��������.
			StructuredBuffers[_Type];
		}


		if (false == StructuredBuffers[_Type].contains(UpperName))
		{
			// ������ ��������.
			StructuredBuffers[_Type][UpperName];
		}

		// Ftransform�̶�� �̸��� ���� �׷��� �־�?
		// �������� ���� �������Ƚ��ϴ�.
		std::map<int, std::shared_ptr<UEngineStructuredBuffer>>& Buffers = StructuredBuffers[_Type][UpperName];

		// Ftransform �̸��� ���� 500����Ʈ ¥�� ������۰� �־�?
		if (true == Buffers.contains(_ByteSize))
		{
			return Buffers[_ByteSize];
		}

		std::shared_ptr<UEngineStructuredBuffer> Res = CreateResUnName();
		Res->SetName(_Name);
		Res->Size = _ByteSize;
		// Res->ResCreate(_ByteSize);
		Buffers[_ByteSize] = Res;
		return Res;
	}

	void Release();
	void Resize(int _Size);

	void ChangeData(const void* _Data, UINT _Size);

protected:

private:
	int Size = 0;

	static std::map<EShaderType, std::map<std::string, std::map<int, std::shared_ptr<UEngineStructuredBuffer>>>> StructuredBuffers;

	ID3D11ShaderResourceView* SRV = nullptr; // ���̴��� �������ټ� �ִ� ����.

	UEngineSerializer Ser;

	void Setting(EShaderType _Type, UINT _Slot);
	void Reset(EShaderType _Type, UINT _Slot);
};

