#include "PreCompile.h"
#include "Session.h"
#include "EngineDebug.h"
#include "EngineSerializer.h"
#include "EngineProtocol.h"

std::atomic<int> USession::CurSessionToken = 0;

USession::USession()
{
}

USession::USession(SOCKET _Socket)
	: Socket(_Socket)
{
}

USession::~USession()
{
	// ���� ����
	End();
}

void USession::Create()
{
	MsgBoxAssert("���� ������ ���� �ʾҽ��ϴ�.");
	// Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

void USession::Bind(int _Port)
{
	MsgBoxAssert("���� ������ ���� �ʾҽ��ϴ�.");
}

void USession::SetAdd(std::string _Ip, int _Port)
{
	Address.sin_family = AF_INET;
	Address.sin_port = htons(_Port);

	IP = _Ip;

	if (SOCKET_ERROR == inet_pton(AF_INET, IP.c_str(), &Address.sin_addr))
	{
		MsgBoxAssert("�ּ� �Է¿� �����߽��ϴ�.");
		return;
	}

}

bool USession::Connect()
{
	if (SOCKET_ERROR == connect(Socket, (const sockaddr*)&Address, sizeof(SOCKADDR_IN)))
	{
		MsgBoxAssert("Ŀ��Ʈ�� �����߽��ϴ�.");
		return false;
	}

	return true;
}

int USession::Send(std::shared_ptr<UEngineProtocol> _Packet)
{
	UEngineSerializer Ser = _Packet->GetSerialize();
	return Send(Ser);
}

int USession::Send(UEngineProtocol& _Packet)
{
	UEngineSerializer Ser = _Packet.GetSerialize();
	return Send(Ser);
}

int USession::Send(UEngineSerializer& _Ser)
{
	return Send(_Ser.DataPtr(), _Ser.WriteSize());
}

void USession::End()
{
	if (Socket != 0)
	{
		closesocket(Socket);
	}
}

