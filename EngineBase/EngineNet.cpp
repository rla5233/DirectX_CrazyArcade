#include "PreCompile.h"
#include "EngineNet.h"
#include "NetStartUpHelper.h"
#include "Session.h"
#include "EngineClient.h"
#include "EngineDebug.h"
#include "EngineProtocol.h"
#include "EngineServer.h"

UEngineNet::UEngineNet()
{
	UNetStartUpHelper::NetStartUp();
	IsActiveValue = true;
}

UEngineNet::~UEngineNet()
{
	IsActiveValue = false;
}


void UEngineNet::RecvThreadFunction(USession* _Session, UEngineNet* _Net)
{
	UEngineSerializer Ser;
	Ser.BufferResize(1024);
	//Ser.BufferResize(2048);

	UEngineDispatcher& Dis = _Net->Dispatcher;

	bool IsClient = false;

	if (nullptr != dynamic_cast<UEngineClient*>(_Net))
	{
		IsClient = true;
	}

	while (_Net->IsActiveValue)
	{
		// ������Ʈ ������Ʈ ��Ŷ�� ũ���?
		// 18
		// 30����Ʈ
		int Size = recv(_Session->GetSocket(), Ser.DataCharPtrToWriteOffset(), Ser.RemainSize(), 0);

		if (SOCKET_ERROR == Size)
		{
			return;
		}

		if (-1 == Size)
		{
			return;
		}

		// 30 + 20
		Ser.AddWriteOffset(Size);

		// �츰 16����Ʈ�� ���� ���� ������ �ƹ��� �ǹ̰� ���⶧��
		// ������ 16����Ʈ �̻��� ���������� ����Ѵ�.
		if (16 > Ser.GetWriteOffset())
		{
			continue;
		}

		UEngineProtocol Protocol;
		Protocol.UEngineProtocol::DeSerialize(Ser);

		if (Protocol.GetPacketType() == -2)
		{
			_Session->TokenInitOn();
			_Session->SetSessionToken(Protocol.GetSessionToken());
			if (nullptr != _Net->TokenPacketFunction)
			{
				_Net->TokenPacketFunction(reinterpret_cast<USessionTokenPacket*>(&Protocol));
			}
			if (true == IsClient)
			{
				// Ŭ���̾�Ʈ �϶��� ����
				_Session->Send(Protocol);
			}
			Ser.Reset();
			continue;
		}

		if (Protocol.GetPacketType() == -2)
		{
			MsgBoxAssert("���ѹ��� ��ȯ�Ǿ���� ��ū��Ŷ�� �� ��ȯ�Ǿ����ϴ�.");
		}


		if (Protocol.GetPacketType() == -3) {
			if (Protocol.GetSessionToken() == _Session->GetSessionToken())
				_Session->End();

			if (false == IsClient) {
				if (UEngineServer* Server = dynamic_cast<UEngineServer*>(_Net)) {
					Server->SessionDestroy(_Session);
				}
			}
		}


		Ser.AddReadOffset(-16);

		// 30����Ʈ

		//       32                     30
		if (Protocol.GetPacketSize() > Ser.WriteSize())
		{
			continue;
		}

		if (Ser.GetReadOffset() > 1024)
		{
			MsgBoxAssert("ReadOffset�� 1024�� �Ѿ����ϴ�.");
			return;
		}

		while (true)
		{
			std::shared_ptr<UEngineProtocol> NewProtocal = Dis.ConvertProtocol(Protocol.GetPacketType(), Ser);
			Dis.ProcessPacket(NewProtocal);

			if (Ser.BufferSize() == Ser.GetReadOffset())
			{
				Ser.Reset();
				break;
			}

			if (Ser.GetWriteOffset() == Ser.GetReadOffset())
			{
				// ����ϰ� �о���.
				Ser.Reset();
				break;
			}


			int WriteOffset = Ser.GetWriteOffset();
			int ReadOffset = Ser.GetReadOffset();
			int RemainOffset = WriteOffset - ReadOffset;

			if (4 > RemainOffset)
			{
				Ser.DataToReadOffsetPush();
				break;
			}

			int Size = *(reinterpret_cast<int*>(Ser.DataCharPtrToReadOffset()));
			// ������ŭ �� �о���.

			if (Size > RemainOffset)
			{
				Ser.DataToReadOffsetPush();
				break;
			}

			Protocol.UEngineProtocol::DeSerialize(Ser);
			Ser.AddReadOffset(-16);
		}
	}
}