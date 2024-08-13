#pragma once
#include <assert.h>
#include <Windows.h>
#include <string>
#include <string_view>

namespace UEngineDebug
{
	#define LeakCheck _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
	#define MsgBoxAssert(Value) \
	std::string ErrorText = std::string(Value); \
	MessageBoxA(nullptr, ErrorText.c_str(), "ġ���� ����", MB_OK); assert(false);

	#define MsgBoxLog(Value) \
	std::string ErrorText = std::string(Value); \
	MessageBoxA(nullptr, ErrorText.c_str(), "ġ���� ����", MB_OK);

	#define MsgBoxLogT(Title, Value) \
	std::string TitleText = std::string(Title); \
	std::string ErrorText = std::string(Value); \
	MessageBoxA(nullptr, ErrorText.c_str(), TitleText.c_str(), MB_OK);


	void /*EngineDebug::*/OutPutDebugText(std::string_view _DebugText);
}

