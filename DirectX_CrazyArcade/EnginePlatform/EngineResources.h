#pragma once
#include <string>
#include <map>
#include <EngineBase/EnginePath.h>
#include <EngineBase/PathObject.h>

// ���� : ���ø��� �̻��� �������� ����.
// ������ �� �ڷ������� �ٲ� ���ø� ������.
template<typename ResType>
class UEngineResources : public UPathObject
{
public:
	// constrcuter destructer
	UEngineResources() {}
	~UEngineResources() {}

	// delete Function
	UEngineResources(const UEngineResources& _Other) = delete;
	UEngineResources(UEngineResources&& _Other) noexcept = delete;
	UEngineResources& operator=(const UEngineResources& _Other) = delete;
	UEngineResources& operator=(UEngineResources&& _Other) noexcept = delete;

	static std::shared_ptr<ResType> FindRes(std::string_view _Name)
	{
		std::lock_guard<std::mutex> Lock(NameResourcesMutex);

		std::string UpperName = UEngineString::ToUpper(_Name);

		if (false == NameResources.contains(UpperName))
		{
			return nullptr;
		}
		
		return NameResources[UpperName];
	}

	static bool IsRes(std::string_view _Name)
	{
		std::string UpperName = UEngineString::ToUpper(_Name);
		return NameResources.contains(UpperName);
	}

	static std::shared_ptr<ResType> CreateResName(std::string_view _Path)
	{
		UEnginePath NewPath = UEnginePath(std::filesystem::path(_Path));
		std::string FileName = NewPath.GetFileName();
		return CreateResName(_Path, FileName);
	}

	static std::shared_ptr<ResType> CreateResName(std::string_view _Path, std::string_view _Name)
	{
		std::string UpperName = UEngineString::ToUpper(_Name);

		std::shared_ptr<ResType> NewRes = std::make_shared<ResType>();
		NewRes->SetName(_Name);
		NewRes->SetPath(_Path);

		if (true == NameResources.contains(UpperName))
		{
			MsgBoxAssert("�̹� �����ϴ� ���ҽ��� �� �ε��Ϸ��� �߽��ϴ�." + UpperName);
		}

		NameResources[UpperName] = NewRes;
		return NewRes;
	}

	static std::shared_ptr<ResType> CreateResUnName()
	{
		std::shared_ptr<ResType> NewRes = std::make_shared<ResType>();
		UnNameResources.push_back(NewRes);
		return NewRes;
	}

	////

	// anyc ��
	// thread safe�� �Լ��� �پ������� 
	static std::shared_ptr<ResType> ThreadSafeCreateResName(std::string_view _Path)
	{
		UEnginePath NewPath = UEnginePath(std::filesystem::path(_Path));
		std::string FileName = NewPath.GetFileName();
		return ThreadSafeCreateResName(_Path, FileName);
	}

	static std::shared_ptr<ResType> ThreadSafeCreateResName(std::string_view _Path, std::string_view _Name)
	{
		std::string UpperName = UEngineString::ToUpper(_Name);

		std::shared_ptr<ResType> NewRes = std::make_shared<ResType>();
		NewRes->SetName(_Name);
		NewRes->SetPath(_Path);

		{
			std::lock_guard<std::mutex> Lock(NameResourcesMutex);
			if (true == NameResources.contains(UpperName))
			{
				MsgBoxAssert("�̹� �����ϴ� ���ҽ��� �� �ε��Ϸ��� �߽��ϴ�." + UpperName);
			}

			NameResources[UpperName] = NewRes;
		}
		return NewRes;
	}

	static std::shared_ptr<ResType> ThreadSafeCreateResUnName()
	{
		std::shared_ptr<ResType> NewRes = std::make_shared<ResType>();

		{
			std::lock_guard<std::mutex> Lock(UnNameResourcesMutex);
			UnNameResources.push_back(NewRes);
		}

		return NewRes;
	}


	static void ResourcesRelease()
	{
		// Ŭ����������
		// �ƹ��� �̾ȿ� �ִ� ���ҽ����� ���¾ֵ��� ���ٸ�
		// �˾Ƽ� ������ ���̴�.
		UnNameResources.clear();
		NameResources.clear();
	}

protected:

private:
	// �翬�� �� �ڷ��� �״�� ����
	// �ؽ�ó�� �ؽ�ó �״��
	// ����Ÿ���̸� ����Ÿ�� �״��
	// ����� ���� �״�� ���� ���� ���̴�.
	// class MapNode
	// {
	//    std::shared_ptr<ResType> Value;
	// }
	static std::mutex NameResourcesMutex;
	static std::map<std::string, std::shared_ptr<ResType>> NameResources;

	static std::mutex UnNameResourcesMutex;
	static std::list<std::shared_ptr<ResType>> UnNameResources;
	// std::string Name = "NONE";
	// std::string Path = "NONE";
};

// ���ø��϶��� ������ �����ϴ�.
// ���������� ����� �߰��Ҽ��� ����?
// ���ø��̸� ������ �ذ�˴ϴ�.
// ���ø� static ������ �����ϴ� ����� ����� �Ѵ�.
template<typename ResType>
std::map<std::string, std::shared_ptr<ResType>> UEngineResources<ResType>::NameResources;

template<typename ResType>
std::mutex UEngineResources<ResType>::NameResourcesMutex;

// ���ø� static ���� �����ϴ¹�.
template<typename ResType>
std::list<std::shared_ptr<ResType>> UEngineResources<ResType>::UnNameResources;

template<typename ResType>
std::mutex UEngineResources<ResType>::UnNameResourcesMutex;
