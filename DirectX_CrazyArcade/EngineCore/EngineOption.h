#pragma once
#include <EngineBase/EngineSerializer.h>
#include <EngineBase/EngineMath.h>
#include <EngineBase/EngineString.h>
#include <EngineBase/EngineDebug.h>
#include <sstream>
#include <iostream>
#include <format>

// ���� : ������ �ɼ� �����͸� �����ϴ� �뵵�� ����ü
// ���ο� ���� �̿��� ���� ������ ���װ� ������.
// �����Լ� ������ std::string ����� ���� �ȵǴµ�.
// std::string�� �ȵ��°� �ʹ� ��������?
struct FEngineOption : public UEngineSerializeObject
{
public:
	std::string WindowTitle = "Title";
	FVector WindowScale = {1280.0f, 720.0f};
	bool IsDebug = false;
	FVector ClearColor = { 0.0f, 0.0f, 1.0f };
	char FreeCameraKey = '0';

	void Serialize(UEngineSerializer& _Ser) override
	{
		{
			// c++ 20���� ���ڿ� �ֽ� ������ ���Ծ��.

			std::string DebugOptionText;

			DebugOptionText += std::format("WindowTitle : [{}]\n", WindowTitle);
			DebugOptionText += std::format("WindowScale : [{}], [{}]\n", WindowScale.iX(), WindowScale.iY());
			DebugOptionText += std::format("IsDebug : [{}]\n", IsDebug);
			DebugOptionText += std::format("ClearColor : R[{}],G[{}],B[{}]\n", ClearColor.X, ClearColor.Y, ClearColor.Z);
			DebugOptionText += std::format("FreeCameraKey : [{}]\n", FreeCameraKey);

			_Ser.WriteText(DebugOptionText);
		}
	}

	void DeSerialize(UEngineSerializer& _Ser) override
	{
		std::string OptionText = _Ser.ToString();

		std::vector<std::string> Values = UEngineString::StringCutting(OptionText, "[", "]");

		int Index = 0;
#define OPTIONINDEX Index++

		WindowTitle = Values[OPTIONINDEX];

		WindowScale.X = static_cast<float>(std::atof(Values[OPTIONINDEX].c_str()));
		WindowScale.Y = static_cast<float>(std::atof(Values[OPTIONINDEX].c_str()));

		std::string IsDebugUpper = UEngineString::ToUpper(Values[OPTIONINDEX]);

		if (IsDebugUpper == "FALSE")
		{
			IsDebug = false;
		}
		else if (IsDebugUpper == "TRUE")
		{
			IsDebug = true;
		}
		else 
		{
			MsgBoxAssert("IsDebug �ɼ��� �̻��� ���ڷ� ä���� �ֽ��ϴ�.");
		}

		ClearColor.X = static_cast<float>(std::atof(Values[OPTIONINDEX].c_str()));
		ClearColor.Y = static_cast<float>(std::atof(Values[OPTIONINDEX].c_str()));
		ClearColor.Z = static_cast<float>(std::atof(Values[OPTIONINDEX].c_str()));

		FreeCameraKey = Values[OPTIONINDEX][0];
	}
	
};

