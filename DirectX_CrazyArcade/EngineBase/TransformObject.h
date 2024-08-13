#pragma once
#include "Transform.h"
#include <vector>

// ���� :
class UTransformObject
{
public:
	// constrcuter destructer
	UTransformObject();
	virtual ~UTransformObject();

	// delete Function
	UTransformObject(const UTransformObject& _Other) = delete;
	UTransformObject(UTransformObject&& _Other) noexcept = delete;
	UTransformObject& operator=(const UTransformObject& _Other) = delete;
	UTransformObject& operator=(UTransformObject&& _Other) noexcept = delete;

	FTransform Transform;

	void SetParent(UTransformObject* _Parent)
	{
		Parent = _Parent;
		Parent->Transform.TransformUpdate();
		Parent->Childs.push_back(this);
		Parent->ChildUpdate();
	}

	void ChildUpdate();

	void SetScale(FVector _Value)
	{
		// ���� Ʈ������ ������Ʈ�� ������
		Transform.SetScale(_Value);
		// �ڽĵ鿡�� �� ����� �����Ű�� ���Դϴ�.
		ChildUpdate();
	}

	void SetRotationDeg(FVector _Value)
	{
		Transform.SetRotationDeg(_Value);
		ChildUpdate();
	}


	void AddPosition(FVector _Value)
	{
		Transform.AddPosition(_Value);
		ChildUpdate();
	}

	FVector GetLocalPosition() const
	{
		return Transform.LocalPosition;
	}

	FVector GetWorldPosition() const
	{
		return Transform.WorldPosition;
	}

	void SetPosition(FVector _Value)
	{
		Transform.SetPosition(_Value);
		ChildUpdate();
	}

	void AddScale(FVector _Value)
	{
		Transform.AddScale(_Value);
		ChildUpdate();
	}

	FVector GetLocalScale() const
	{
		return Transform.LocalScale;
	}

	FVector GetWorldScale() const
	{
		return Transform.WorldScale;
	}

	void AddRotationDeg(FVector _Value)
	{
		Transform.AddRotationDeg(_Value);
		ChildUpdate();
	}

	FVector GetLocalRotation() const
	{
		return Transform.LocalRotation;
	}

	FVector GetWorldRotation() const
	{
		return Transform.WorldRotation;
	}

	FVector GetWorldForward()
	{
		return Transform.World.ArrVector[2].Normalize3DReturn();
	}

	FVector GetWorldUp()
	{
		return Transform.World.ArrVector[1].Normalize3DReturn();
	}

	FVector GetWorldRight()
	{
		return Transform.World.ArrVector[0].Normalize3DReturn();
	}

	FVector GetWorldBack()
	{
		return -GetWorldForward();
	}

	FVector GetWorldDown()
	{
		return -GetWorldUp();
	}

	FVector GetWorldLeft()
	{
		return -GetWorldRight();
	}




protected:
	UTransformObject* Parent = nullptr;

private:
	std::vector<UTransformObject*> Childs;

};

