#include "PreCompile.h"
#include "EngineCore.h"
#include <EngineBase/EngineFile.h>
#include <EngineBase/EngineDirectory.h>
#include <EnginePlatform/EngineSound.h>
#include <EngineCore/EngineTexture.h>
#include <EngineCore/EngineDebugMsgWindow.h>
#include "EngineEditorGUI.h"
#include "EngineDebug3D.h"
#include <EnginePlatform/EngineInputRecorder.h>

#include "Level.h"
#include "GameMode.h"

#include "EngineVertexBuffer.h"

ULevel* UEngineCore::CurCreateLevel;

UEngineCore::UEngineCore() 
{
}

UEngineCore::~UEngineCore() 
{
	// ������ �����Ҷ� ���� ���� �ɼ��� ���̺� �ϰ� �Ѵ�.
	UEngineDirectory Dir;
	Dir.MoveToSearchChild("Config");
	UEngineFile File = Dir.GetPathFromFile("EngineOption.EData");
	UEngineSerializer Ser;
	EngineOption.Serialize(Ser);
	File.Open(EIOOpenMode::Write, EIODataType::Text);
	File.Save(Ser);
}

UEngineCore* GEngine = nullptr;

void UEngineCore::EngineStart(HINSTANCE _Inst)
{
	// ��üũ
	LeakCheck;
	GEngine = this;

	JobWorker.Initialize("Engine Thread");

	EngineOptionInit();

	EngineWindow.Open(EngineOption.WindowTitle, WindowIconPath);
	// ����̽� �ʱ�ȭ���� ũ�Ⱑ ���������� �ػ󵵰� �̹� ���� �Ȱſ���.
	// EngineOption.WindowScale �ػ�
	// �ػ󵵴� ������ ũ��� ������ �����ϴ�.
	EngineWindow.SetWindowScale(EngineOption.WindowScale);
	EngineDevice.Initialize(EngineWindow, EngineOption.ClearColor);

	UEngineInputRecorder::Init(EngineWindow.GetHWND());

	UEngineEditorGUI::GUIInit();

	UEngineEditorGUI::CreateEditorWindow<UEngineDebugMsgWindow>("DebugMsgWindow");

	{
		UserCorePtr->Initialize();
		MainTimer.TimeCheckStart();
	} 


	UEngineWindow::WindowMessageLoop(
		std::bind(&UEngineCore::EngineFrameUpdate, this),
		std::bind(&UEngineCore::EngineEnd, this)
	);

	UEngineEditorGUI::GUIRelease();
}

void UEngineCore::EngineOptionInit()
{
	UEngineDirectory Dir;
	Dir.MoveToSearchChild("Config");

	if (false == Dir.IsFile("EngineOption.EData"))
	{
		UEngineFile File = Dir.GetPathFromFile("EngineOption.EData");
		UEngineSerializer Ser;
		EngineOption.Serialize(Ser);

		File.Open(EIOOpenMode::Write, EIODataType::Text);
		File.Save(Ser);
	}

	{
		UEngineFile File = Dir.GetPathFromFile("EngineOption.EData");
		UEngineSerializer Ser;
		File = Dir.GetPathFromFile("EngineOption.EData");
		File.Open(EIOOpenMode::Read, EIODataType::Text);
		File.Load(Ser);
		EngineOption.DeSerialize(Ser);
	}
}

void UEngineCore::EngineEnd()
{
	UserCorePtr->End();
	JobWorker.End();
	Levels.clear();
	EngineDevice.EngineResourcesRelease();
}

void UEngineCore::EngineFrameUpdate()
{
	float DeltaTime = MainTimer.TimeCheck();

	UEngineSound::Update();

	DeltaTime *= GlobalTimeScale;

	if (true == EngineWindow.IsFocus())
	{
		UEngineInput::KeyCheckTick(DeltaTime);
	}
	else
	{
		UEngineInput::AllKeyReset();
	}

	GEngine->EngineWindow.CalculateMouseUpdate(DeltaTime);

	for (size_t i = 0; i < DestroyLevelName.size(); i++)
	{
		std::string UpperName = UEngineString::ToUpper(DestroyLevelName[i]);

		std::shared_ptr<ULevel> Level = Levels[UpperName];

		Levels.erase(DestroyLevelName[i]);

		if (Level == CurLevel)
		{
			CurLevel = nullptr;
		}
	}
	DestroyLevelName.clear();

	if (nullptr != NextLevel)
	{
		if (nullptr != CurLevel)
		{
			CurLevel->LevelEnd(NextLevel.get());
			CurLevel->Destroy();
		}

		NextLevel->LevelStart(CurLevel.get());

		CurLevel = NextLevel;



		NextLevel = nullptr;
	}

	UserCorePtr->Tick(DeltaTime);
	CurLevel->Tick(DeltaTime);

	// ȭ�� �����
	EngineDevice.RenderStart();
	// ���ӿ� ��ҵ��� �׸���

	CurLevel->Render(DeltaTime);
	UDebugRenderClass::DebugRender(CurLevel.get());
	UEngineEditorGUI::GUIRender(CurLevel.get(), DeltaTime);
	EngineDevice.RenderEnd();

	CurLevel->Destroy();
}

std::shared_ptr<ULevel> UEngineCore::NewLevelCreate(std::string_view _Name, std::shared_ptr<AActor> _GameMode)
{
	std::string UpperName = UEngineString::ToUpper(_Name);

	std::shared_ptr <AGameMode> GameModePtr = std::dynamic_pointer_cast<AGameMode>(_GameMode);

	if (nullptr == GameModePtr)
	{
		MsgBoxAssert("������ ù ������Ʈ�� GameMode�� ��ӹ��� Ŭ������ �ƴմϴ�.");
		return nullptr;
	}

	std::shared_ptr<ULevel> Level = std::make_shared<ULevel>();
	CurCreateLevel = Level.get();
	Level->SetGameMode(GameModePtr);
	Level->SetName(_Name);
	Level->PushActor(_GameMode);
	Levels[UpperName] = Level;
	CurCreateLevel = nullptr;
	return Level;
}