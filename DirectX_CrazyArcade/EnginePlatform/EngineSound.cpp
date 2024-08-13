#include "PreCompile.h"
#include "EngineSound.h"
#include <EngineBase\EngineString.h>
#include <EngineBase\EngineDebug.h>

// lib�� ����ϴ¹�

// �ƿ� ������� �Ѵ�. ���尡 �Ǳ� ���� �ƿ� ������� �Ѵ�.
// �̶� ���� ����ϴ°� ifdef


#ifdef _DEBUG
// Debug �϶���
#pragma comment(lib, "fmodL_vc.lib")
#else
// Release �϶��� 
#pragma comment(lib, "fmod_vc.lib")
#endif

float UEngineSound::GlobalVolume = 1.0f;

void UEngineSoundPlayer::SetVolume(float _Volume)
{
	Control->setVolume(_Volume * UEngineSound::GlobalVolume);
}

UEngineSoundPlayer UEngineSoundPlayer::Replay()
{
	*this = UEngineSound::SoundPlay(Name);
	return *this;
}

// FMOD�� ���õ� ���带 �ε��Ҽ� �ִ� ����.
FMOD::System* SoundSystem = nullptr;

class ResControl
{
public:
	ResControl()
	{
		if (FMOD_RESULT::FMOD_OK != FMOD::System_Create(&SoundSystem))
		{
			MsgBoxAssert("���� �ý��� ������ �����߽��ϴ�.");
			return;
		}

		if (FMOD_RESULT::FMOD_OK != SoundSystem->init(32, FMOD_DEFAULT, nullptr))
		{
			MsgBoxAssert("���� �ý��� ������ �����߽��ϴ�.");
			return;
		}
	}
	~ResControl()
	{
		if (nullptr != SoundSystem)
		{
			SoundSystem->release();
			SoundSystem = nullptr;
		}
	}
};

ResControl Inst;


UEngineSound::UEngineSound()
{
}

UEngineSound::~UEngineSound()
{
}


void UEngineSound::ResLoad(std::string_view _Path)
{
	// FMOD_DEFAULT; => �ݺ������ �ȵȴ�.
	SoundSystem->createSound(_Path.data(), FMOD_LOOP_NORMAL, nullptr, &SoundHandle);
	if (nullptr == SoundHandle)
	{
		MsgBoxAssert("���� �ε忡 �����߽��ϴ�." + std::string(_Path));
		return;
	}

	// SoundSystem->playSound(SoundHandle, nullptr, false, nullptr);
}

void UEngineSound::SetGlobalVolume(float _Value)
{
	GlobalVolume = _Value;

	if (GlobalVolume <= 0.0f)
	{
		GlobalVolume = 0.0f;
	}

	if (GlobalVolume >= 1.0f)
	{
		GlobalVolume = 1.0f;
	}
}

UEngineSoundPlayer UEngineSound::SoundPlay(std::string_view _Name)
{
	std::string UpperName = UEngineString::ToUpper(_Name);

	if (false == IsRes(UpperName.c_str()))
	{
		MsgBoxAssert("�ε����� ���带 ����Ϸ��� �߽��ϴ�." + UpperName);
		return UEngineSoundPlayer();
	}

	std::shared_ptr <UEngineSound> FindSound = FindRes(UpperName);

	UEngineSoundPlayer Result;
	Result.Name = _Name;
	SoundSystem->playSound(FindSound->SoundHandle, nullptr, false, &Result.Control);
	Result.Control->setLoopCount(0);
	Result.SetVolume(1.0f);

	if (nullptr == Result.Control)
	{
		MsgBoxAssert("���� ����� ���� ���� ������ �������� ���߽��ϴ�.");
		return Result;
	}


	return Result;
}

void UEngineSound::Update()
{
	// �������Ӹ��� ȣ��Ǿ�� �Ѵ�.
	// �׷��� FMOD ���̺귯���� �ټ��� ���带 �������ش�.
	if (nullptr == SoundSystem)
	{
		MsgBoxAssert("���� �ý��ۿ� ġ������ ������ �ֽ��ϴ�.");
	}

	SoundSystem->update();
}

void UEngineSound::Load(std::string_view _Path, std::string_view _Name)
{
	std::string UpperName = UEngineString::ToUpper(_Name);

	if (true == IsRes(UpperName))
	{
		MsgBoxAssert("�̹� �ε��� ���ҽ��� �� �ε� �Ϸ��� �߽��ϴ�.");
		return;
	}

	std::shared_ptr<UEngineSound> NewSound = CreateResName(_Path, _Name);
	NewSound->ResLoad(_Path);
}