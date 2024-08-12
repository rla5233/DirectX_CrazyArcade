#include "PreCompile.h"
#include "EngineServer.h"
#include <functional>
#include "EngineProtocol.h"
#include "NetObject.h"
#include "list"
#include "Session.h"

UEngineServer::UEngineServer()
{
}

UEngineServer::~UEngineServer()
{
}

void UEngineServer::AcceptThreadFunction(UEngineServer* Server, SOCKET _AcceptSocket)
{
	int AddressLen = sizeof(SOCKADDR_IN);

	while (true == Server->IsActive())
	{
		SOCKADDR_IN ClientAdd;
		memset(&ClientAdd, 0, sizeof(ClientAdd));
		SOCKET ClientSocket = accept(_AcceptSocket, (sockaddr*)&ClientAdd, &AddressLen);

		// �����Դϴ�.
		if (SOCKET_ERROR == ClientSocket || INVALID_SOCKET == ClientSocket)
		{
			return;
		}

		std::shared_ptr<UTCPSession> NewSession = std::make_shared<UTCPSession>(ClientSocket);

		// ��ū�� ������ ���� ����� ��������.
		// ������ �� ����� �̴ϴ�.


		// ���� ��ū�� 

		int InitToken = 0;


		for (std::shared_ptr<USession> Session : Server->Sessions) {
			Server->SessionArr[Session->GetSessionToken()] = true;
		}
		for (int SessionToken = 1; SessionToken < Server->SessionArr.size(); ++SessionToken) {
			if (false == Server->SessionArr[SessionToken]) {
				USessionTokenPacket NewPacket;
				NewPacket.SetSessionToken(SessionToken);
				NewPacket.SetObjectToken(SessionToken * 1000);
				UEngineSerializer Ser = NewPacket.GetSerialize();
				NewSession->Send(Ser);
				InitToken = SessionToken;
				break;
			}
		}

		for (int i = 0; i < Server->SessionArr.size(); ++i) {
			Server->SessionArr[i] = false;
		}


		// Ŭ���̾�Ʈ 1���� ���ú꾲���带 ��������� ��밡 �����ߴٸ�
		// �׳༮�� ���� �غ� �ȰŴ�.
		// ƼŰŸ���� �������ؾ� �Ѵ�.
		std::shared_ptr<UEngineThread> ClientRecvThread = std::make_shared<UEngineThread>();
		ClientRecvThread->SetName("Server Recv Thread " + std::to_string(InitToken));
		ClientRecvThread->Start(std::bind(UEngineNet::RecvThreadFunction, NewSession.get(), Server));

		Server->SessionRecvs.push_back(ClientRecvThread);
		Server->Sessions.push_back(NewSession);

	}
}

void UEngineServer::SessionDestroy(USession* _Session) {
	std::vector<std::shared_ptr<USession>> RemoveSessions;
	for (std::shared_ptr<USession> Session : Sessions) {
		if (Session.get() == _Session) {
			RemoveSessions.push_back(Session);
		}
	}
	int Size =  static_cast<int>(RemoveSessions.size());
	for (int i = 0; i < Size; ++i) {
		Sessions.remove(RemoveSessions[i]);
	}
}

void UEngineServer::ServerOpen(int _Port, int _BackLog /*= 512*/)
{
	Port = _Port;

	AcceptSession.Create();
	AcceptSession.Bind(_Port);
	AcceptSession.SetSessionToken(USession::GetNewSessionToken());

	int BackLog = 1;
	if (SOCKET_ERROR == listen(AcceptSession.GetSocket(), BackLog))
	{
		assert(false);
	}
	// �����ڰ� ������ �����ڸ� ������ �ϴµ� ������ �ؾ��Ѵ�.
	AcceptThread.SetName("AcceptThread");
	AcceptThread.Start(std::bind(AcceptThreadFunction, this, AcceptSession.GetSocket()));
}

void UEngineServer::Send(std::shared_ptr<UEngineProtocol> _Protocol)
{
	// �������� ��쿡�� ��ε�ĳ��Ʈ�� �ؾ��Ѵ�.

	UEngineSerializer Ser = _Protocol->GetSerialize();

	for (std::shared_ptr<USession> User : Sessions)
	{
		if (false == User->IsTokenInit())
		{
			continue;
		}

		if (User->GetSessionToken() == -1) {
			return;
		}

		if (User->GetSessionToken() == _Protocol->GetSessionToken())
		{
			continue;
		}

		User->Send(Ser);
	}

	// �ٸ� ��� �����ڿ��� �� ���������� �� ������.
}
