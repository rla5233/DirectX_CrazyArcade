#pragma once
#include <string>
#include <string_view>
#include <map>

// #include�� �̷��� ����ϴ��� ������ �� �ڵ尡 ������� ������
// �����Ϸ��� �����Ѵ�.
#include "ThirdParty\FMOD\inc\fmod.hpp"

#include "EngineResources.h"

// �̳༮�� ������ ���� ���� ����� �����帮�ڽ��ϴ�.

class UEngineSoundPlayer
{
	friend class UEngineSound;

public:
	void On()
	{
		Control->setPaused(false);
	}
	void Off()
	{
		Control->setPaused(true);
	}

	void Loop(int Count = -1)
	{
		Control->setLoopCount(Count);
	}

	UEngineSoundPlayer Replay();

	void OnOffSwitch()
	{
		bool Check = false;
		Control->getPaused(&Check);

		if (true == Check)
		{
			Control->setPaused(false);
		}
		else {
			Control->setPaused(true);
		}
	}

	void SetVolume(float _Volume);

private:
	std::string Name;
	// �̰� ���� ����� ���� ���� �Դϴ�.
	FMOD::Channel* Control = nullptr;
};

// ���� :
class UEngineSound : public UEngineResources<UEngineSound>
{
	friend class ResControl;

private:
	friend UEngineSoundPlayer;
	static float GlobalVolume;

public:
	// constrcuter destructer
	UEngineSound();
	~UEngineSound();

	// delete Function
	UEngineSound(const UEngineSound& _Other) = delete;
	UEngineSound(UEngineSound&& _Other) noexcept = delete;
	UEngineSound& operator=(const UEngineSound& _Other) = delete;
	UEngineSound& operator=(UEngineSound&& _Other) noexcept = delete;

	static void SetGlobalVolume(float _Value);

	static UEngineSoundPlayer SoundPlay(std::string_view _Name);

	static void Load(std::string_view _Path)
	{
		UEnginePath NewPath = UEnginePath(std::filesystem::path(_Path));
		std::string FileName = NewPath.GetFileName();
		return Load(_Path, FileName);
	}

	static void Load(std::string_view _Path, std::string_view _Name);
	static void Update();

protected:
	void ResLoad(std::string_view _Path);

private:
	FMOD::Sound* SoundHandle = nullptr;
};

