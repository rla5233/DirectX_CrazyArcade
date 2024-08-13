#pragma once
#include <WinSock2.h>

// ���� : ������ �����ӿ�ũ�� ���� �� ������ ���ִ� �ֵ��� �ִ�.
// ��κ� ��Ƽ�÷����̴�.
class UEngineProtocol;
class UEngineSerializer;
class USession
{
public:
	// constrcuter destructer
	USession();
	USession(SOCKET Socket);
	virtual ~USession();

	virtual void Create() = 0;
	virtual void Bind(int _Port) = 0;
	virtual void SetAdd(std::string _Ip, int _Port);

	static int GetNewSessionToken()
	{
		return CurSessionToken++;
	}

	int GetCurSessionToken()
	{
		return CurSessionToken;
	}
	SOCKET GetSocket()
	{
		return Socket;
	}

	SOCKADDR_IN GetAddress()
	{
		return Address;
	}

	SOCKADDR_IN& GetAddressRef()
	{
		return Address;
	}


	int GetSessionToken()
	{
		return SessionToken;
	}


	void SetSessionToken(int _SessionToken)
	{
		SessionToken = _SessionToken;
	}

	bool Connect();

	virtual int Send(UEngineProtocol& _Packet);
	virtual int Send(std::shared_ptr<UEngineProtocol> _Packet);
	virtual int Send(UEngineSerializer& _Ser);
	virtual int Send(void* _Data, int _Size) = 0;

	int Recv(UEngineSerializer& _Recv);

	bool IsTokenInit()
	{
		return TokenInit;
	}

	void TokenInitOn()
	{
		TokenInit = true;
	}

	void End();

protected:
	SOCKET Socket = 0;
	SOCKADDR_IN Address = {};
	std::string IP = "127.0.0.1";
	int Port = 0;
	int SessionToken = -1;
	bool TokenInit = false;

private:
	static std::atomic<int> CurSessionToken;
};

