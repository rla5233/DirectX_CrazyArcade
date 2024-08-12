#pragma once
#include <map>
#include <functional>
#include <memory>
#include "EngineDebug.h"
#include "EngineProtocol.h"

// ���� :
class UEngineDispatcher
{
public:
	// constrcuter destructer
	UEngineDispatcher();
	~UEngineDispatcher();

	// delete Function
	UEngineDispatcher(const UEngineDispatcher& _Other) = delete;
	UEngineDispatcher(UEngineDispatcher&& _Other) noexcept = delete;
	UEngineDispatcher& operator=(const UEngineDispatcher& _Other) = delete;
	UEngineDispatcher& operator=(UEngineDispatcher&& _Other) noexcept = delete;

	// UActorUpdatePacket
	template<typename PacketType>
	void AddHandler(std::function<void(std::shared_ptr<PacketType>)> _CallBack)
	{
		int Type = static_cast<int>(PacketType::Type);
		AddHandler(Type, _CallBack);
	}

	template<typename PacketType>
	void AddHandler(int Type, std::function<void(std::shared_ptr<PacketType>)> _CallBack)
	{
		if (true == PacketHandlers.contains(Type))
		{
			MsgBoxAssert("�̹� ��ϵ� �ڵ鷯�� �� ��Ͻ�ų���� �����ϴ�.");
		}

		ConvertPacketHandlers[Type] = [=](UEngineSerializer& _Ser)
			{
				std::shared_ptr<PacketType> NewPacket = std::make_shared<PacketType>();
				NewPacket->DeSerialize(_Ser);
				return NewPacket;
			};

		PacketHandlers[Type] = [=](std::shared_ptr<UEngineProtocol> _Packet)
			{
				std::shared_ptr<PacketType> ConvertPacket = std::dynamic_pointer_cast<PacketType>(_Packet);
				if (nullptr == ConvertPacket)
				{
					MsgBoxAssert("Ÿ���� ��ġ���� �ʴ� ��Ŷ�� �����߽��ϴ�.");
				}

				_CallBack(ConvertPacket);
			};
	}

	std::shared_ptr<UEngineProtocol> ConvertProtocol(int Type, UEngineSerializer& _Ser)
	{
		if (false == ConvertPacketHandlers.contains(Type))
		{
			MsgBoxAssert("ó������� �������� ���� ��Ŷ�Դϴ�.");
			return nullptr;
		}

		return ConvertPacketHandlers[Type](_Ser);
	}

	void ProcessPacket(std::shared_ptr<UEngineProtocol> _Packet)
	{
		if (nullptr == _Packet)
		{
			return;
		}

		if (false == ConvertPacketHandlers.contains(_Packet->GetPacketType()))
		{
			MsgBoxAssert("ó������� �������� ���� ��Ŷ�Դϴ�.");
			return;
		}

		return PacketHandlers[_Packet->GetPacketType()](_Packet);
	}

protected:

private:
	std::map<int, std::function<std::shared_ptr<UEngineProtocol>(UEngineSerializer& _Ser)>> ConvertPacketHandlers;

	// int Ÿ��
	std::map<int, std::function<void(std::shared_ptr<UEngineProtocol>)>> PacketHandlers;

};

