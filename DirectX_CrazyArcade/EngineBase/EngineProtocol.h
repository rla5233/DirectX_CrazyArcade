#pragma once
#include "EngineSerializer.h"
#include <memory>

// ���� :
class UNetObject;
class UEngineProtocol : public std::enable_shared_from_this<UEngineProtocol>, public UEngineSerializeObject
{
	friend UNetObject;

protected:
	int PacketSize = 0; // ���� �������� ���ɼ� �ۿ� ����.
	int PacketType = 0; // ���� �����̴�. �׷��� int Enum�� �ɼ� ����.
	int SessionToken = 0; // ���� ���� ��Ŷ�� �޾��� �ֵ�
	int ObjectToken = 0; // ���� ���� ��Ŷ�� �޾ƾ��� ������Ʈ

	UEngineSerializer GetSerializer()
	{

	}

public:
	int GetPacketSize()
	{
		return PacketSize;
	}

	template<typename EnumType>
	EnumType GetPacketType()
	{
		return static_cast<EnumType>(PacketType);
	}

	int GetPacketType()
	{
		return PacketType;
	}
	int GetObjectToken()
	{
		if (ObjectToken == 1) {
			int a = 0;
		}
		return ObjectToken;
	}
	int GetSessionToken()
	{
		return SessionToken;
	}

	void SetSessionToken(int _Token)
	{
		SessionToken = _Token;
	}

	void SetObjectToken(int _ObjectToken)
	{
		ObjectToken = _ObjectToken;
	}

	template<typename PType>
	void SetType(PType _Type)
	{
		PacketType = static_cast<int>(_Type);
	}

	void SizeCal()
	{

	}

	UEngineSerializer GetSerialize()
	{
		// �ø�������� �����δ� ��Ŷ ����� �˼��� �����.
		UEngineSerializer Ser;
		Serialize(Ser);
		PacketSize = Ser.GetWriteOffset();
		Ser.Paste(0, &PacketSize, sizeof(PacketSize));
		return Ser;
	}


	void Serialize(UEngineSerializer& _Ser) override
	{
		// [PacketSize][PacketType][Token][][][][][][][][][][][][][][][][][][][][][][][]
		_Ser << PacketSize;
		_Ser << PacketType;
		_Ser << SessionToken;
		_Ser << ObjectToken;
	}

	void DeSerialize(UEngineSerializer& _Ser) override
	{
		_Ser >> PacketSize;
		_Ser >> PacketType;
		_Ser >> SessionToken;
		_Ser >> ObjectToken;
	}
};

class USessionTokenPacket : public UEngineProtocol
{
public:
	USessionTokenPacket()
	{
		SetType(-2);
	}
};

#define ProtocolHeader(EnumType, TypeName) \
public: static const EnumType Type = EnumType##::##TypeName; public: U##TypeName##() { SetType(##EnumType##::##TypeName); }