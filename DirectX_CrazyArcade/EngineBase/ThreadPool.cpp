#include "PreCompile.h"
#include "ThreadPool.h"
#include "EngineDebug.h"

UThreadPool::UThreadPool() 
{
}

UThreadPool::~UThreadPool() 
{
	End();
}

void UThreadPool::Initialize(const std::string_view& _ThreadName, int _ThreadCount/* = 0*/)
{
	ThreadCount = _ThreadCount;

	// �� �׳� ������ ������ �����޶�� �ű���.
	// ������ �� �ھ� * 2 + 1
	// cpu �� * (cpu ��ǥ ��뷮) * (1+��� �ð�/���� �ð�) + tomcat
	// �׳� ��Ȳ�� ���� ������ ���� ����� �˴ϴ�. ����
	// �������� �ھ� ������ŭ ����ڽ��ϴ�.
	if (0 >= ThreadCount)
	{
		SYSTEM_INFO Info;
		GetSystemInfo(&Info);
		ThreadCount = Info.dwNumberOfProcessors;
	}


	// IOCP�� ���ڴ�. => IOCP�� ����ڴ�.
	// CreateIoCompletionPort 
	// 1. IOCP�� �����.
	// 2. io��Ʈ�� �� ����Ѵ�.<= ���� ����ϴ� ���� 

	// �̷��� ���� iocp�� ����ڴٴ� ���� �˴ϴ�.
	IOCPHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, ThreadCount);

	if (nullptr == IOCPHandle)
	{
		MsgBoxAssert("iocp �ڵ� ������ �����߽��ϴ�.");
	}

	Threads.resize(ThreadCount);

	IsRun = true;

	RunThreadCount = ThreadCount;

	for (size_t i = 0; i < Threads.size(); i++)
	{
		Threads[i] = std::make_shared<UEngineThread>();
		Threads[i]->SetName(std::string(_ThreadName) + std::to_string(i));
		Threads[i]->Start(std::bind(ThreadPoolWorkFunction, this, IOCPHandle));
	}

	int a = 0;
}

void UThreadPool::ThreadPoolWorkFunction(UThreadPool* _Pool, HANDLE _IOCPHandle)
{
	DWORD Byte; // ���ƿ� ��Ŷ�̳� ������ ũ��
	ULONG_PTR Ptr; // �������ְ� ���� � ���ϵ��� �аų� ���� ������ 8����Ʈ ������ ������ �ټ� �ִ�.
	LPOVERLAPPED OverLapped = nullptr; // ������ ��������½� ���� �󸶳� �о�鿴������ ���� ������ ������ �ֽ��ϴ�.


	while (_Pool->IsRun)
	{
		// ���� ����� ����� ���� ���� �ֵ��� ���� Sleep()
		// ����������� ���ؼ� ���������.
		// INFINITE ��ٸ��� �ð� 
		// �����ð����� �Ͼ �������� �Ͼ
		// ���Ѵ� �ð� ������ �Ͼ.
		// �츮�� ���� �ð������� �Ͼ
		GetQueuedCompletionStatus(_IOCPHandle, &Byte, &Ptr, &OverLapped, INFINITE);
		// GetQueuedCompletionStatus(_IOCPHandle, &Byte, &Ptr, &OverLapped, 2000);

		EThreadWorkType Type = static_cast<EThreadWorkType>(Byte);

		if (Type == EThreadWorkType::Destroy)
		{
			break;
		}

		FJob* NewJob = reinterpret_cast<FJob*>(Ptr);
		NewJob->WorkFunction();
		delete NewJob;
		NewJob = nullptr;
	}

	--_Pool->RunThreadCount;
}

void UThreadPool::Work(std::function<void()> _Function)
{
	// 8����Ʈ ������ 
	
	// ������ iocp�� ����� �Լ�
	// IOCPHandle �� �����ް� �ִ� �ֵ��߿� �ϳ� �Ͼ
	// PostQueuedCompletionStatus(IOCPHandle, 999, reinterpret_cast<__int64>(312321));

	FJob* NewJob = new FJob();
	NewJob->WorkFunction = _Function;
	PostQueuedCompletionStatus(IOCPHandle, static_cast<BYTE>(EThreadWorkType::Work), reinterpret_cast<__int64>(NewJob), nullptr);
}

void UThreadPool::End()
{
	IsRun = false;

	while(RunThreadCount)
	{
		PostQueuedCompletionStatus(IOCPHandle, static_cast<BYTE>(EThreadWorkType::Destroy), 3333333, nullptr);
	}
	
}