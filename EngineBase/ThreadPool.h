#pragma once
#include <vector>
#include "EngineThread.h"
#include <Windows.h>


enum class EThreadWorkType
{
	Work,
	Destroy,
};


struct FJob
{
	std::function<void()> WorkFunction;
};

// ������ Ǯ�� ����� ����� �پ��� ��� �ִµ�.

// ���� :
class UThreadPool
{
public:
	// constrcuter destructer
	UThreadPool();
	~UThreadPool();

	// delete Function
	UThreadPool(const UThreadPool& _Other) = delete;
	UThreadPool(UThreadPool&& _Other) noexcept = delete;
	UThreadPool& operator=(const UThreadPool& _Other) = delete;
	UThreadPool& operator=(UThreadPool&& _Other) noexcept = delete;

	void Initialize(const std::string_view& _ThreadName, int _ThreadCount = 0);

	void Work(std::function<void()> _Function);

	void End();

protected:

private:
	// IOCP�� ������
	// ���� ����� �����͸� ó���ϴ� �ϵ���
	// ������� ������ �ִ��� Ȯ�����ְ�
	// ������� �������� ���ۿ��ҵ� ���ְ�
	HANDLE IOCPHandle = nullptr;
	int ThreadCount = -1;

	// typedef std::atomic<bool> atomic_bool;

	std::atomic_bool IsRun = false;;

	std::atomic_int RunThreadCount = 0;

	std::vector<std::shared_ptr<UEngineThread>> Threads;

	static void ThreadPoolWorkFunction(UThreadPool* _Pool, HANDLE _IOCPHandle);
};

// ���� 100�� �ִ�.

// ������� 5�� �� �־��.

// IOCP�� �� �������� ����۾��� ���ļ� �� ����ϰ� �ֽ��ϴ�.

// IOCP�� �ڽſ��� ��ϵ� ��������� ������ ���� �����Ѵ�.

// waiting
// Release
// Paused

// ������ �� �������� ����
// [waiting][waiting][waiting][waiting][waiting]

// ���� ó���Ϸ��� ��ǻ�Ϳ� �ɾƾ� �Ѵٰ� ����.
// job qeueue [��][��][��][��][��][��][��][��][��][��]
// Waiting Queue �ٱ����� ����ϰ� �ִ� ���
// Release Queue ��ǻ�� �ڸ��� �ɾ��ִ� ���
// Paused Queue ��ǻ�� �ڸ��� �ɾ��ִµ� ������ �յ� ��� �ϳ������.

// ���� 1���� ���Ծ�.
// job qeueue [��][��][��][��][��][��][��][��][��][��]
// Waiting Queue [waiting][waiting][waiting][waiting]
// Release Queue [Release[��]]
// Paused Queue 

// ���� 1���� ���Ծ�.
// job qeueue [��][��][��][��][��][��][��][��][��][��]
// Waiting Queue [waiting][waiting][waiting]
// Release Queue [Release[��]][waiting[��]]
// Paused Queue 

// release�� �����ϴ� �������� 1���� ���� ������
// job qeueue [��][��][��][��][��][��][��][��][��][��]
// Waiting Queue [waiting][waiting][waiting] // ���� �ڸ��� ������ ���� ����
// Release Queue [waiting[��]]
// Paused Queue [Paused] 

// ���� 1�� ���Ծ�
// job qeueue [��][��][��][��][��][��][��][��][��][��]
// Waiting Queue [waiting][waiting][waiting]
// Release Queue [waiting[��]]
// Paused Queue [Paused[��]] 

// job qeueue [��][��][��][��][��][��][��][��][��][��]
// Waiting Queue [waiting][waiting][waiting]
// Release Queue [waiting[��]]
// Paused Queue [Paused] // ��ǻ�������� ���� ���� �ִ� ���°� �ɼ��� �ִ�.

// iocp�� �����ϴµ� ���� ǥ�����ڸ� 10�ʸ��� 1���� �شٰ� ġ��.
// 
// �����尡 ���� �Ѵٴ� ���� CPU�� �ھ��� �θ��� ��ǻ���� �ڸ��� �ɾƼ�
// ������ Ű�� ���η��� Ű�� ���� ���� �ϵ� ������ ��� �ؾ��ϴ����� ����.
// �����尡 ���� �ϱ� ���ؼ� ���� �غ� �ϴ� �۾��� ���ؽ�Ʈ ����Ī�̶�� �մϴ�
// ���ؽ�Ʈ ����Ī�� �Ͼ�� ���� �����带 busy�������� �մϴ�.
// 
// �����尡 �ڽ��� ����� �ִ� �����带 ���� �Ǹ� �翬�� ĳ������ �޸𸮵� ����
// �� cpu�� �ٸ� ������� ���Ҽ� �ִ� ���·� ����� �����ſ���.


