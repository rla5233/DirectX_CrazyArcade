#pragma once
#include "EngineMath.h"

enum class ECollisionType
{
	Point,
	CirCle,
	Rect,
	RotRect,
	Sphere,
	Box,
	RotBox,
	Max,
};

class CollisionData
{
public:
	union
	{
		// [][][]
		// []
		// ��
		DirectX::BoundingSphere Sphere;

		// [][][]
		// [][][]
		// ȸ������ ���� �簢��
		DirectX::BoundingBox AABB;

		// [][][]
		// [][][]
		// [][][][]
		// ȸ���� �簢��
		DirectX::BoundingOrientedBox OBB;
	};

	CollisionData()
		: OBB(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f))
	{

	}
};

// ���� :
class CollisionFunctionInit;
struct FTransform
{
	// 
	float4 LocalScale;
	float4 LocalRotation;
	float4 LocalPosition;
	float4 WorldScale;
	float4 WorldRotation;
	float4 WorldPosition;
	float4x4 ScaleMat;
	float4x4 RotationMat;
	float4x4 PositionMat;
	float4x4 ParentMat;
	float4x4 LocalWorld;
	float4x4 World;
	float4x4 View;
	float4x4 Projection;
	float4x4 WVP;
	//};

	friend CollisionFunctionInit;

private:
	static bool (*CollisionFunction[static_cast<int>(ECollisionType::Max)][static_cast<int>(ECollisionType::Max)])(const FTransform& _Left, const FTransform& _Right);

public:
	// constrcuter destructer
	FTransform();
	FTransform(const FVector& _Pos, const FVector& _Scale)
		: LocalPosition(_Pos), LocalScale(_Scale)
	{

	}
	~FTransform();

	// delete Function
	//FTransform(const FTransform& _Other) = delete;
	//FTransform(FTransform&& _Other) noexcept = delete;
	//FTransform& operator=(const FTransform& _Other) = delete;
	//FTransform& operator=(FTransform&& _Other) noexcept = delete;

	// bool CircleToPoint(const FTransform& _Left, const FTransform& _Right);

	static bool CircleToCircle(const FTransform& _Left, const FTransform& _Right);
	static bool CircleToRect(const FTransform& _Left, const FTransform& _Right);
	static bool CircleToRotRect(const FTransform& _Left, const FTransform& _Right);
	static bool CircleToPoint(const FTransform& _Left, const FTransform& _Right);

	static bool RectToRotRect(const FTransform& _Left, const FTransform& _Right);
	static bool RectToRect(const FTransform& _Left, const FTransform& _Right);
	static bool RectToCircle(const FTransform& _Left, const FTransform& _Right);
	static bool RectToPoint(const FTransform& _Left, const FTransform& _Right);

	static bool RotRectToRotRect(const FTransform& _Left, const FTransform& _Right);
	static bool RotRectToCirCle(const FTransform& _Left, const FTransform& _Right);
	static bool RotRectToRect(const FTransform& _Left, const FTransform& _Right);
	static bool RotRectToPoint(const FTransform& _Left, const FTransform& _Right);

	static bool PointToRect(const FTransform& _Left, const FTransform& _Right);
	static bool PointToCircle(const FTransform& _Left, const FTransform& _Right);
	static bool PointToRotRect(const FTransform& _Left, const FTransform& _Right);


public:
	CollisionData GetCollisionData() const
	{
		CollisionData Result;

		Result.OBB.Center = WorldPosition.DirectFloat3;
		// ����ũ��� �־���� �մϴ�.

		Result.OBB.Extents = (WorldScale * 0.5f).ABS3DReturn().DirectFloat3;

		// ���� ģ���� ���� ü���
		// x 30 y 20 z 60 
		// -nan -nan -nan => ������.

		// �����ڵ��� ����������� ����� �����ϰ�
		// �� ȸ���� ��ü�Ҽ� �ִ� ������ ȸ�� ����� ����� �´�.
		// �װ� �����.
		// x 30 y 20 z 60  <= �������� �ᱹ �߻��Ѵ�.
		// ����� 3�̴�.
		// x 30 y 20 z 60  => float4�� ������Ų��.

		// ���ʹϿ����� ��ȯ�ϰ� �ű⿡�� �ٽ� float4�� ��ȯ�ؼ� �Ѱ��ش�.
		Result.OBB.Orientation = WorldRotation.DegToQuaternion().DirectFloat4;
	}

	CollisionData GetCollisionData2D() const
	{
		CollisionData Result;

		Result.OBB.Center = WorldPosition.DirectFloat3;
		Result.OBB.Center.z = 0.0f;
		Result.OBB.Extents = (WorldScale * 0.5f).ABS3DReturn().DirectFloat3;
		Result.OBB.Orientation = WorldRotation.DegToQuaternion().DirectFloat4;
		return Result;
	}



	void SetScale(FVector _Value)
	{
		LocalScale = _Value;
		TransformUpdate();
	}

	void SetRotationDeg(FVector _Value)
	{
		LocalRotation = _Value;
		TransformUpdate();
	}

	void SetPosition(FVector _Value)
	{
		LocalPosition = _Value;
		TransformUpdate();
	}

	void AddScale(FVector _Value)
	{
		SetScale(LocalScale + _Value);
	}

	FVector GetScale() const
	{
		return LocalScale;
	}

	void AddRotationDeg(FVector _Value)
	{
		SetRotationDeg(LocalRotation + _Value);
	}

	FVector GetRotation() const
	{
		return LocalRotation;
	}

	FVector GetForward()
	{
		return World.ArrVector[2].Normalize3DReturn();
	}

	FVector GetUp()
	{
		return World.ArrVector[1].Normalize3DReturn();
	}

	FVector GetRight()
	{
		return World.ArrVector[0].Normalize3DReturn();
	}

	FVector GetBack()
	{
		return -GetForward();
	}

	FVector GetDown()
	{
		return -GetUp();
	}

	FVector GetLeft()
	{
		return -GetRight();
	}


	void AddPosition(FVector _Value)
	{
		SetPosition(LocalPosition + _Value);
	}

	FVector GetPosition() const
	{
		return LocalPosition;
	}

	FVector LeftTop() const
	{
		return { Left(), Top() };
	}

	FVector RightTop() const
	{
		return { Right(), Top() };
	}

	FVector LeftBottom() const
	{
		return { Left(), Bottom() };
	}

	FVector RightBottom() const
	{
		return { Right(), Bottom() };
	}

	float Left() const
	{
		return LocalPosition.X - LocalScale.hX();
	}
	float Top() const
	{
		return LocalPosition.Y - LocalScale.hY();
	}
	float Right() const
	{
		return LocalPosition.X + LocalScale.hX();
	}
	float Bottom() const
	{
		return LocalPosition.Y + LocalScale.hY();
	}

	int iLeft() const
	{
		return std::lround(Left());
	}
	int iRight() const
	{
		return std::lround(Right());
	}
	int iTop() const
	{
		return std::lround(Top());
	}
	int iBottom() const
	{
		return std::lround(Bottom());
	}

	void SetRadius(float _Radius)
	{
		LocalScale = float4::Zero;
		LocalScale.X = _Radius * 2.0f;
	}

	float GetRadius() const
	{
		return LocalScale.hX();
	}

	// �浹�� �̳༮�� ������?
	// ��ġ�� ũ�⸦ ������ �ֱ� ������.
	bool Collision(ECollisionType _ThisType, ECollisionType _OtherType, const FTransform& _Other);

	void TransformUpdate();

	void CalculateViewAndProjection(FMatrix _View, FMatrix _Projection);

protected:

private:
};

