#pragma once
#include "Session.h"
#include "EngineSerializer.h"

// ���� :
class UEngineProtocol;
class UTCPSession : public USession
{
public:
	// constrcuter destructer
	UTCPSession();
	UTCPSession(SOCKET Socket);
	~UTCPSession();

	void Create() override;
	void Bind(int _Port) override;

	int Send(std::shared_ptr<UEngineProtocol> _Packet) override;

	int Send(UEngineSerializer& _Ser) override;
	int Send(void* Data, int Size) override;

	bool IsServerConnected() {
		return ServerConnected;
	}

	void SetServerConnected(bool _Connection) {
		ServerConnected = _Connection;
	}

protected:

private:
	bool ServerConnected = false;
	// ip�ּҵ� �̳༮�� �����Ѵ�.

};

