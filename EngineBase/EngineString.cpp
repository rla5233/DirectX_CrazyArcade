#include "PreCompile.h"
#include "EngineString.h"
#include <Windows.h>
#include "EngineDebug.h"

UEngineString::UEngineString() 
{
}

UEngineString::~UEngineString() 
{
}

std::wstring UEngineString::AnsiToUniCode(std::string_view _View)
{
	if (0 == _View.size())
	{
		return L"";
	}

	int Size = MultiByteToWideChar(CP_ACP, 0, _View.data(), static_cast<int>(_View.size()), nullptr, 0);

	if (0 == Size)
	{
		MsgBoxAssert("���ڿ� ��ȯ�� �����߽��ϴ�.");
		return L"";
	}

	std::wstring Result;
	Result.resize(Size);

	Size = MultiByteToWideChar(CP_ACP, 0, _View.data(), static_cast<int>(_View.size()), &Result[0], Size);

	if (0 == Size)
	{
		MsgBoxAssert("���ڿ� ��ȯ�� �����߰ų� ũ�Ⱑ 0�� ���ڿ��� �־�����ϴ�");
		return L"";
	}

	return Result;
}

std::string UEngineString::UniCodeToAnsi(std::wstring_view _View)
{
	if (0 == _View.size())
	{
		return "";
	}

	int Size = WideCharToMultiByte(CP_ACP, 0, _View.data(), static_cast<int>(_View.size()), nullptr, 0, 0, 0);

	if (0 == Size)
	{
		MsgBoxAssert("���ڿ� ��ȯ�� �����߽��ϴ�.");
		return "";
	}

	std::string Result;
	Result.resize(Size);

	Size = WideCharToMultiByte(CP_ACP, 0, _View.data(), static_cast<int>(_View.size()), &Result[0], Size, 0, 0);

	if (0 == Size)
	{
		MsgBoxAssert("���ڿ� ��ȯ�� �����߰ų� ũ�Ⱑ 0�� ���ڿ��� �־�����ϴ�");
		return "";
	}

	return Result;
}

std::string UEngineString::ToUpper(std::string_view View)
{
	std::string Name = View.data();

	for (char& _Ch : Name)
	{
		_Ch = std::toupper(_Ch);
	}

	return Name;
}

std::vector<std::string> UEngineString::StringCutting(std::string& _Value, std::vector<std::string> _CutText)
{
	std::vector<std::string> Result;

	size_t StartFindOffset = 0;
	size_t EndFindOffset = 0;

	while (true)
	{
		std::vector<size_t> Count;

		for (size_t i = 0; i < _CutText.size(); i++)
		{
			EndFindOffset = _Value.find_first_of(_CutText[i], StartFindOffset);

			if (std::string::npos != EndFindOffset)
			{
				Count.push_back(EndFindOffset);
				// break;
			}
		}

		EndFindOffset = std::string::npos;
		for (size_t i = 0; i < Count.size(); i++)
		{
			if (EndFindOffset >= Count[i])
			{
				EndFindOffset = Count[i];
			}
		}

		if (EndFindOffset == std::string::npos)
		{
			break;
		}

		std::string ParseString = _Value.substr(StartFindOffset, EndFindOffset - StartFindOffset);
		Result.push_back(ParseString);

		StartFindOffset = EndFindOffset + 1;
	}

	return Result;
}

std::vector<std::string> UEngineString::StringCutting(std::string& _Value, std::string_view _Start, std::string_view _End)
{
	std::vector<std::string> Result;

	size_t StartFindOffset = 0;

	while (true)
	{
		// _Start���� �����ؼ� �ڷ� �̵��ϸ鼭 1��¥�� ã�ƶ� find_first_of �տ��� ����
		// StartFindOffset <= 0 
		// StartFindOffset <= _Value.find_first_of(_Start, StartFindOffset); 14
		// [ ã�Ҵ�. 14���ڿ� �־���.
		StartFindOffset = _Value.find_first_of(_Start, StartFindOffset);

		if (std::string::npos == StartFindOffset)
		{
			break;
		}

		// _Start���� �����ؼ� �ڷ� �̵��ϸ鼭 1��¥�� ã�ƶ� find_first_of �տ��� ����
		// 14 ���� ã�ƶ� _End
		// 14 ]
		// EndFindOffset 14���ٴ� ū ���� ���´�.
		size_t EndFindOffset = _Value.find_first_of(_End, StartFindOffset);

		// std::string::npos 1896215023153153 size_t �ִ밪�̿���
		if (std::string::npos == EndFindOffset)
		{
			break;
		}

		// [ ��ĭ �ڿ� �����״ϱ� StartFindOffset + 1
		// EndFindOffset - StartFindOffset - 1 = 2
		// [ ��ġ�������� 2��¥ �߶�׼��䰡 �˴ϴ�.
		std::string ParseString = _Value.substr(StartFindOffset + 1, EndFindOffset - StartFindOffset - 1);
		Result.push_back(ParseString);
		StartFindOffset = EndFindOffset;
	}

	return Result;
}