#include "PreCompile.h"
#include "EngineClient.h"
#include "EngineDebug.h"

UEngineClient::UEngineClient()
{
}

UEngineClient::~UEngineClient()
{
}

void UEngineClient::ServerOpen(int _Port, int _BackLog)
{
	MsgBoxAssert("Ŭ���̾�Ʈ�� �����Ǿ��ִ� �ν��Ͻ��� ������ �ٽ� �����Ϸ��� �߽��ϴ�. �Ұ����մϴ�.");
}

void UEngineClient::Connect(std::string _Ip, int _Port)
{
	Session.Create();
	Session.SetAdd(_Ip, _Port);
	if (false == Session.Connect())
	{
		MsgBoxAssert("Ŭ���̾�Ʈ�� ������ ���ӿ� �����߽��ϴ�. ������ �������� ��Ʈ�� ��Ȯ���� �����ǵ� ��Ȯ���� Ȯ���ϼ���.");
		Session.SetServerConnected(false);
		return;
	}
	Session.SetServerConnected(true);
	RecvThread.SetName("Client Recv Thread");
	RecvThread.Start(std::bind(UEngineNet::RecvThreadFunction, &Session, this));
}

void UEngineClient::Send(std::shared_ptr<UEngineProtocol> _Protocol)
{
	if (false == Session.IsTokenInit())
	{
		return;
	}
	Session.Send(_Protocol);


}