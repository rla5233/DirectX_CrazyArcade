#include "PreCompile.h"
#include "EngineDebug.h"
#include "NetStartUpHelper.h"
#include <atomic>

UNetStartUpHelper::UNetStartUpHelper() 
{
}

UNetStartUpHelper::~UNetStartUpHelper() 
{
}

void UNetStartUpHelper::NetStartUp()
{
	static std::atomic<bool> StartUpCheck = false;

	if (true == StartUpCheck)
	{
		return;
	}

	WSAData WsaData;
	int Error = WSAStartup(MAKEWORD(2, 2), &WsaData);
	if (SOCKET_ERROR == Error)
	{
		MsgBoxAssert("��Ʈ��ũ ��ŸƮ�� �����߽��ϴ�.");
		return;
	}

	StartUpCheck = true;
}