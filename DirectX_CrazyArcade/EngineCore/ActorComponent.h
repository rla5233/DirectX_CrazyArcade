#pragma once
#include "Actor.h"
#include "TickObject.h"
#include <EngineBase/NameObject.h>
#include "WorldObject.h"

// ���� : �׳� ������ ���ϴ� �ֵ��� �ְ�
class UActorComponent : public UTickObject, public UNameObject, public UWorldObject
{
	friend AActor;

public:
	// constrcuter destructer
	UActorComponent();
	~UActorComponent();

	// delete Function
	UActorComponent(const UActorComponent& _Other) = delete;
	UActorComponent(UActorComponent&& _Other) noexcept = delete;
	UActorComponent& operator=(const UActorComponent& _Other) = delete;
	UActorComponent& operator=(UActorComponent&& _Other) noexcept = delete;

	AActor* GetActor()
	{
		return Actor;
	}

	virtual ULevel* GetWorld() override
	{
		return Actor->GetWorld();
	}

	bool IsActive() override
	{
		return UTickObject::IsActive() && Actor->IsActive();
	}

protected:

private:

	AActor* Actor = nullptr;

	void SetActor(AActor* _Actor)
	{
		Actor = _Actor;
	}

};

