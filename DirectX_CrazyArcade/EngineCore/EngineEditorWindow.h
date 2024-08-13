#pragma once
#include <EngineBase/NameObject.h>
#include "imgui.h"

// �츰 �̰͸� ����մϴ�.
// �����е��� ������ â�� ���� �;�.

// ���� :
class ULevel;
class UEngineEditorGUI;
class UEngineEditorWindow : public UNameObject
{
	friend UEngineEditorGUI;

public:
	// constrcuter destructer
	UEngineEditorWindow();
	// ���� �����Լ� => Ŭ������ �߻�Ŭ������ �ȴ�.
	// ���������Լ� ����� 1���� �̿��ϰ� �ʹٸ� �Ҹ��� pure�� �ɸ� �ȴ�.
	~UEngineEditorWindow() = 0;

	// delete Function
	UEngineEditorWindow(const UEngineEditorWindow& _Other) = delete;
	UEngineEditorWindow(UEngineEditorWindow&& _Other) noexcept = delete;
	UEngineEditorWindow& operator=(const UEngineEditorWindow& _Other) = delete;
	UEngineEditorWindow& operator=(UEngineEditorWindow&& _Other) noexcept = delete;

	void On()
	{
		IsActive = true;
	}

	void Off()
	{
		IsActive = false;
	}

protected:
	virtual void Init() {}
	virtual void Tick(ULevel* Level, float _Delta);
	// ���� �����Լ� => Ŭ������ �߻�Ŭ������ �ȴ�.
	// 1. ��üȭ ��ų�� ����. => ���������Լ��� ������ �Ǿ��������� �ְ� �������� �ִ�.
	// 2. �ڽĿ��� �����Ҽ� �Լ��� ���� ������ ����. 
	virtual void OnGui(ULevel* Level, float _Delta) = 0	{}

private:
	std::string Title;
	bool IsActive = true;

	void Begin();
	void End();
};

