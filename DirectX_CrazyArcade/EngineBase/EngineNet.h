#pragma once
#include "EngineSerializer.h"
#include "EngineDispatcher.h"
#include "EngineProtocol.h"

// ���� : ���� ������ �߿����� �ʴ�. 

// ������ ������ ������ ����Ѵ�.
// ������ Ŭ��
class USession;
class UEngineProtocol;
class UEngineNet
{
public:
	// constrcuter destructer
	UEngineNet();
	~UEngineNet();

	// delete Function
	UEngineNet(const UEngineNet& _Other) = delete;
	UEngineNet(UEngineNet&& _Other) noexcept = delete;
	UEngineNet& operator=(const UEngineNet& _Other) = delete;
	UEngineNet& operator=(UEngineNet&& _Other) noexcept = delete;

	//virtual void Send(UEngineSerializer& _Data);
	//virtual void Recv(UEngineSerializer& _Data);

	//virtual void Send(void* _Ptr, int _Size);
	//virtual void Recv(void* _Ptr, int _Size);


	virtual void ServerOpen(int _Port, int _BackLog) {	}

	// std::string _Ip � ��ǻ�Ϳ� �����Ұų�.
	// int _Port �� ��ǻ�Ϳ� �����ִ� � ���α׷��� �����Ұų�.
	virtual void Connect(std::string _Ip, int _Port) {};

	std::atomic_bool& IsActive()
	{
		return IsActiveValue;
	}

	virtual bool IsNetConnetion() { return false; }

	void End()
	{
		IsActiveValue = false;
	}

	void SetTokenPacketFunction(std::function<void(USessionTokenPacket*)> _Function)
	{
		TokenPacketFunction = _Function;
	}

	virtual void Send(std::shared_ptr<UEngineProtocol> _Protocol) {};

	static void RecvThreadFunction(USession* _Session, UEngineNet* _Net);

	// �������� ������ ���� public���� �Ӵϴ�..

	virtual int GetSessionToken() = 0;
	virtual void SetSessionToken(int _SessionToken) = 0;

	virtual void Off()
	{
		IsActiveValue = false;
	}

	UEngineDispatcher Dispatcher;

protected:

private:
	std::function<void(USessionTokenPacket*)> TokenPacketFunction = nullptr;
	std::atomic_bool IsActiveValue = true;
};

