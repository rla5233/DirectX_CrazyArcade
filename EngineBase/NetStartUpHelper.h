#pragma once

// ��Ʈ��ũ��°�
// ��Ʈ��ũ��°�
// ���� ��밡 ��ǻ�Ϳ��� ���� �����͸� ������.
// ��밡 ������ �����͸� ��������.

// ���� :
class UNetStartUpHelper
{
public:
	// constrcuter destructer
	UNetStartUpHelper();
	~UNetStartUpHelper();

	// delete Function
	UNetStartUpHelper(const UNetStartUpHelper& _Other) = delete;
	UNetStartUpHelper(UNetStartUpHelper&& _Other) noexcept = delete;
	UNetStartUpHelper& operator=(const UNetStartUpHelper& _Other) = delete;
	UNetStartUpHelper& operator=(UNetStartUpHelper&& _Other) noexcept = delete;

	static void NetStartUp();
protected:

private:

};

