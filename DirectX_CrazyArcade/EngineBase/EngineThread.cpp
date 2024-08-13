#include "PreCompile.h"
#include "EngineThread.h"
#include "EngineString.h"

UEngineThread::UEngineThread() 
{
}

UEngineThread::~UEngineThread() 
{
	// ���� ������ �������̾�?
	if (true == Thread.joinable())
	{
		Thread.join();
	}
}

void UEngineThread::SetFunction(std::function<void()> _Function)
{
	CallBack = _Function;
}

void UEngineThread::ThreadStartFunction(UEngineThread* _Thread)
{
	// ������ȿ��� 
	// ���⼭���� �̹� ������
	std::string Name = _Thread->GetName();

	if (Name == "")
	{
		Name = "None Thread";
	}

	std::wstring WName = UEngineString::AnsiToUniCode(Name);
	SetThreadDescription(GetCurrentThread(), WName.c_str());

	// �����е��� �Լ��� ����˴ϴ�.
	_Thread->CallBack();

	_Thread->End = true;
}

void UEngineThread::Start(std::function<void()> _Function /*= nullptr*/)
{
	if (true == Thread.joinable())
	{
		return;
	}

	if (nullptr != _Function)
	{
		SetFunction(_Function);
	}

	if (nullptr == CallBack)
	{
		return;
	}

	// Thread = std::thread(CallBack);

	Thread = std::thread(std::bind(ThreadStartFunction, this));

	
}