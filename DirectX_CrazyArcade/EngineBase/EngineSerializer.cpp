#include "PreCompile.h"
#include "EngineSerializer.h"
#include "EngineDebug.h"

UEngineSerializer::UEngineSerializer()
{
}

UEngineSerializer::~UEngineSerializer()
{
}

void UEngineSerializer::BufferResize(int _Size)
{
	Data.resize(_Size);
}

void UEngineSerializer::Paste(int Offset, const void* _Data, size_t _Size)
{
	if (Offset + _Size > Data.size())
	{
		MsgBoxAssert("���� ������ �Ѿ���ϴ�.");
	}

	memcpy_s(&Data[Offset], _Size, _Data, _Size);
}

void UEngineSerializer::Write(const void* _Data, size_t _Size)
{
	if (WriteOffset + _Size >= Data.size())
	{
		Data.resize(Data.capacity() * 2 + _Size);
	}

	// &Data[WriteOffset] �ּӰ���
	// _Size ũ�⸸ŭ
	// _Data �ּҰ��� �ִ� �����͸� 
	// _Size ũ�⸸ŭ �����ض�.
	// &Data[WriteOffset]<= _Data
	memcpy_s(&Data[WriteOffset], _Size, _Data, _Size);
	WriteOffset += static_cast<int>(_Size);
}

void UEngineSerializer::WriteText(const std::string& _Text)
{
	Write(_Text.c_str(), _Text.size());
}


void UEngineSerializer::Read(void* _Data, size_t _Size)
{
	memcpy_s(_Data, _Size, &Data[ReadOffset], _Size);
	ReadOffset += static_cast<int>(_Size);
}

std::string UEngineSerializer::ToString()
{
	return static_cast<char*>(&Data[0]);
}

void UEngineSerializer::operator<<(UEngineSerializeObject& _Data)
{
	_Data.Serialize(*this);
}

void UEngineSerializer::operator<<(UEngineSerializeObject* _Data)
{
	_Data->Serialize(*this);
}

void UEngineSerializer::operator>>(UEngineSerializeObject& _Data)
{
	_Data.DeSerialize(*this);
}

void UEngineSerializer::operator>>(UEngineSerializeObject* _Data)
{
	_Data->DeSerialize(*this);
}

void UEngineSerializer::Reset()
{
	WriteOffset = 0;
	ReadOffset = 0;
}

void UEngineSerializer::ResetRead()
{
	ReadOffset = 0;
}

void UEngineSerializer::AddReadOffset(int _Value)
{
	ReadOffset += _Value;
}

void UEngineSerializer::ResetWrite()
{
	WriteOffset = 0;
}

void UEngineSerializer::DataToReadOffsetPush()
{
	// 50����Ʈ
	// 32����Ʈ�� ó��(read)�߾� 
	// 18����Ʈ�� �� ������ �о������.

	//               100           50
	int ReMainSize = WriteOffset - ReadOffset;

	if (ReadOffset == Data.size())
	{
		WriteOffset = 0;
		ReadOffset = 0;
		return;
	}

	memcpy_s(&Data[0], ReMainSize, &Data[ReadOffset], ReMainSize);
	// 100        50
	WriteOffset = ReMainSize;
	ReadOffset = 0;
	//                 50          50                  50
}