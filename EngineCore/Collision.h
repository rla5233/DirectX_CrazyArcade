#pragma once
#include "SceneComponent.h"
#include <set>

// ���� :

// double ���۷��� ī��Ʈ ��������?
// �⺻ �ڷ��� ���۷����� ���ο� ����� ����.
// std::shared_ptr<double>

// Ŭ������ ����ϱ�? ���ο� int�ϳ� ��� �ִ°�
// ����� ���� �ƴϴ�.
// enable_shared_from_this ���ο� ���۷��� ī��Ʈ�� �ɹ������� ������ �ȴ�.
// ����� ptr�� �����ȴٸ� �� this�� ��򰡷� �����ϴ� ������ 
// ���۷��� ī��Ʈ�� �����ϴ� �����Դϴ�.
// std::enable_shared_from_this<UCollision>

class UCollision : public USceneComponent, public std::enable_shared_from_this<UCollision>
{
	GENERATED_BODY(USceneComponent)

public:
	// constrcuter destructer
	UCollision();
	~UCollision();

	// delete Function
	UCollision(const UCollision& _Other) = delete;
	UCollision(UCollision&& _Other) noexcept = delete;
	UCollision& operator=(const UCollision& _Other) = delete;
	UCollision& operator=(UCollision&& _Other) noexcept = delete;

	// �������� �и��� �ϱ� ���ؼ� ���� ���� ������ �Լ��ε��� �̸��� �ٲ㼭 �ٽ� �����.
	// ���� � �׷쿡 ���� �ݸ������� ���ϱ� ���ؼ�
	template<typename EnumType>
	void SetCollisionGroup(EnumType _Index)
	{
		SetCollisionGroup(static_cast<int>(_Index));
	}

	void SetCollisionGroup(int _Index);

	// �浹�ϸ� �ϰ� ������ ����?
	template<typename EnumType>
	bool CollisionEnter(EnumType _TargetGroup, std::function<void(std::shared_ptr<UCollision>)> _Function = nullptr)
	{
		return CollisionEnter(static_cast<int>(_TargetGroup), _Function);
	}

	// ���� �浹 ������ 1��
	bool CollisionEnter(int _TargetGroup, std::function<void(std::shared_ptr<UCollision>)> _Function = nullptr);

	template<typename EnumType>
	bool CollisionStay(EnumType _TargetGroup, std::function<void(std::shared_ptr<UCollision>)> _Function = nullptr)
	{
		return CollisionStay(static_cast<int>(_TargetGroup), _Function);
	}


	// �ݸ��� �浹���̸� ���
	bool CollisionStay(int _TargetGroup, std::function<void(std::shared_ptr<UCollision>)> _Function = nullptr);

	template<typename EnumType>
	bool CollisionExit(EnumType _TargetGroup, std::function<void(std::shared_ptr<UCollision>)> _Function = nullptr)
	{
		return CollisionExit(static_cast<int>(_TargetGroup), _Function);
	}


	// �浹�� �������� 1��
	bool CollisionExit(int _TargetGroup, std::function<void(std::shared_ptr<UCollision>)> _Function = nullptr);

	bool Collision(int _TargetGroup, 
		std::function<void(std::shared_ptr<UCollision>)> _Enter = nullptr, 
		std::function<void(std::shared_ptr<UCollision>)> _Stay = nullptr, 
		std::function<void(std::shared_ptr<UCollision>)> _Exit = nullptr);

	void SetCollisionType(ECollisionType _CollisionType)
	{
		CollisionType = _CollisionType;
	}

	template<typename EnumType>
	void SetOrder(EnumType _Order)
	{
		SetOrder(static_cast<int>(_Order));
	}

	void SetOrder(int _Order) override;

	void SetActive(bool _Value) override
	{
		Super::SetActive(_Value);
		FirstCheck.clear();
		OtherCheck.clear();
		ExitCheck.clear();
	}

protected:
	void BeginPlay() override;
	void Tick(float _Delta) override;

private:
	ECollisionType CollisionType = ECollisionType::CirCle;

	// ��ȯ������ ����.
	std::set<UCollision*> FirstCheck;
	std::set<UCollision*> OtherCheck;
	std::set<UCollision*> ExitCheck;
};

