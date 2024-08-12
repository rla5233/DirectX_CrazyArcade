#pragma once
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

// ���� �浹�ϰڽ��ϴ�.

// Imgui�� ����ϰڴ�.
// ��ư�� �ϳ��� ����� �͵� �Ϲ������δ� �ڵ�� ���� ������� �ϸ�
// ������ ��������� ���Դϴ�.
// �翬�� ���̺귯���� �� �ֽ��ϴ�. window
// imgui�� ���� ��Ƽ�÷��� ��ĭ ���̷�Ʈ9 ���̷�Ʈ11 ���̷�Ʈ12 �̷������� ��Ƽ�÷���
// �ű�� �ڵ差�� ����. �׳� imgui�ڵ带 ������ ���ͼ� ������ �׳� ����־ �������� ����.
// �������� branch�� doking���� �������̴�.

// ���� :
class UEngineCore;
class UEngineEditorWindow;
class UEngineEditorGUI
{
	friend UEngineCore;
	friend UEngineEditorWindow;

public:
	// constrcuter destructer
	UEngineEditorGUI();
	~UEngineEditorGUI();

	// delete Function
	UEngineEditorGUI(const UEngineEditorGUI& _Other) = delete;
	UEngineEditorGUI(UEngineEditorGUI&& _Other) noexcept = delete;
	UEngineEditorGUI& operator=(const UEngineEditorGUI& _Other) = delete;
	UEngineEditorGUI& operator=(UEngineEditorGUI&& _Other) noexcept = delete;

	template<typename EditorWindowType>
	static std::shared_ptr<EditorWindowType> CreateEditorWindow(std::string_view _Name)
	{
		std::string UpperName = UEngineString::ToUpper(_Name);

		if (true == EditorWindows.contains(UpperName))
		{
			MsgBoxAssert("���� �̸��� window�� 2�� ����� �����ϴ�.");
			return nullptr;
		}

		std::shared_ptr<UEngineEditorWindow> Windows = std::make_shared<EditorWindowType>();
		WindowInit(Windows, _Name);
		EditorWindows[UpperName] = Windows;
		GEngine->EngineWindow.SetFocus();
		FocusTest = true;
		return std::dynamic_pointer_cast<EditorWindowType>(Windows);
	}

	static void WindowOn(std::string_view _Name);

	static void WindowOff(std::string_view _Name);

protected:

private:
	static void WindowInit(std::shared_ptr<UEngineEditorWindow> _Window, std::string_view _Name);

	static void GUIRender(ULevel* Level, float _DeltaTime);
	static void GUIInit();
	static void GUIRelease();

	static std::map<std::string, std::shared_ptr<UEngineEditorWindow>> EditorWindows;
	static bool FocusTest;
};

