#pragma once
#include "EnginePath.h"
#include <list>

class UEngineFile;
// ���� :
class UEngineDirectory : public UEnginePath
{
public:
	// constrcuter destructer
	UEngineDirectory();
	UEngineDirectory(const UEnginePath& _Path);
	~UEngineDirectory();

	// delete Function
	//UEngineDirectory(const UEngineDirectory& _Other) = delete;
	//UEngineDirectory(UEngineDirectory&& _Other) noexcept = delete;
	//UEngineDirectory& operator=(const UEngineDirectory& _Other) = delete;
	//UEngineDirectory& operator=(UEngineDirectory&& _Other) noexcept = delete;

	void MoveToSearchChild(std::string_view _Path);

	std::string GetFolderName() const
	{
		return GetFileName();
	}


	// AAA.Png;
	// AAA.pNg;

	/// <summary>
	/// ������ ���δ� ã���ִ� �Լ�
	/// </summary>
	/// <param name="_Ext">Ȯ���ڵ� �ҹ��� �빮�� ������ ����.</param>
	/// <param name="_Rescursive">���� �������� �� ��������.</param>
	/// <returns></returns>
	std::vector<UEngineFile> GetAllFile(std::vector<std::string> _Ext = std::vector<std::string>(), bool _Recursive = false);

	// std::list<UEnginePath> AllPath(bool _Recursive = false);

	// �̷��� �������� �ʴ� �밡�� ���
	std::vector<UEngineDirectory> GetAllDirectory(bool _Recursive = false);

	UEngineFile GetPathFromFile(std::string_view FileName);

	bool IsFile(std::string_view _FileName);

protected:

private:
	void AllDirectoryRecursive(const std::string_view _Path, std::vector<UEngineDirectory>& _Result, bool _Recursive = false);
	void AllFileRecursive(const std::string_view _Path, std::vector<UEngineFile>& _Result, std::vector<std::string> _Ext = std::vector<std::string>(), bool _Recursive = false);
};

