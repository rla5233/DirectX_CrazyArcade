#pragma once
#include <string>
#include <cmath>
#include <Windows.h>

#include <DirectXPackedVector.h> // ���̷�Ʈ���� ���
#include <DirectXCollision.h> 


// ���� :
class UEngineMath
{
public:
	// constrcuter destructer
	UEngineMath();
	~UEngineMath();

	// delete Function
	UEngineMath(const UEngineMath& _Other) = delete;
	UEngineMath(UEngineMath&& _Other) noexcept = delete;
	UEngineMath& operator=(const UEngineMath& _Other) = delete;
	UEngineMath& operator=(UEngineMath&& _Other) noexcept = delete;

	static const float PI;
	static const float PI2;

	// ��׸��� => ����
	static const float DToR;
	// ������ => ��׸���
	static const float RToD;

protected:

private:

};



struct float4
{
	// F����ü�� 

public:
	static const float4 Zero;
	static const float4 One;
	static const float4 Forward;
	static const float4 BackWard;
	static const float4 Left;
	static const float4 Right;
	static const float4 Up;
	static const float4 Down;

	static const float4 Red;
	static const float4 White;
	static const float4 Black;

	union
	{
		struct
		{
			float X; // 2D
			float Y; // 2D
			float Z;
			float W;
		};

		struct
		{
			float R; // 2D
			float G; // 2D
			float B;
			float A;
		};

		float Arr1D[4];
		float Arr2D[1][4];
		DirectX::XMFLOAT3 DirectFloat3;
		DirectX::XMFLOAT4 DirectFloat4;
		DirectX::XMVECTOR DirectVector;
	};

	// �����ڸ� �ѹ� ����� �Ǹ� ����Ʈ �̴ϼȶ������� �������� �ʾƼ�
	// ���� �����ϴ� ����� �� �����ؾ� �մϴ�.
	float4()
		: X(0.0f), Y(0.0f), Z(0.0f), W(1.0f)
	{

	}

	float4(long _X, long _Y)
		: X(static_cast<float>(_X)), Y(static_cast<float>(_Y)), Z(0.0f), W(1.0f)
	{

	}


	float4(int _X, int _Y)
		: X(static_cast<float>(_X)), Y(static_cast<float>(_Y)), Z(0.0f), W(1.0f)
	{

	}

	float4(unsigned int _X, unsigned int _Y)
		: X(static_cast<float>(_X)), Y(static_cast<float>(_Y)), Z(0.0f), W(1.0f)
	{

	}


	float4(float _X, float _Y)
		: X(_X), Y(_Y), Z(1.0f), W(1.0f)
	{

	}

	float4(float _X, float _Y, float _Z)
		: X(_X), Y(_Y), Z(_Z), W(1.0f)
	{

	}

	float4(int _X, int _Y, int _Z)
		: X(static_cast<float>(_X)), Y(static_cast<float>(_Y)), Z(static_cast<float>(_Z)), W(1.0f)
	{

	}

	float4(int _X, int _Y, int _Z, int _W)
		: X(static_cast<float>(_X)), Y(static_cast<float>(_Y)), Z(static_cast<float>(_Z)), W(static_cast<float>(_W))
	{

	}

	float4(float _X, float _Y, float _Z, float _W)
		: X(_X), Y(_Y), Z(_Z), W(_W)
	{

	}


public:
	void ABS3D()
	{
		X = fabs(X);
		Y = fabs(Y);
		Z = fabs(Z);
	}

	float4 ABS3DReturn()
	{
		float4 Result;
		Result.X = fabs(X);
		Result.Y = fabs(Y);
		Result.Z = fabs(Z);
		return Result;
	}

	static float4 VectorRotationZToDeg(float4 _OriginVector, float _Angle)
	{
		return VectorRotationZToRad(_OriginVector, _Angle * UEngineMath::DToR);
	}

	static float4 VectorRotationZToRad(float4 _OriginVector, float _Angle)
	{
		float4 Result;
		Result.X = (_OriginVector.X * cosf(_Angle)) - (_OriginVector.Y * sinf(_Angle));
		Result.Y = (_OriginVector.X * sinf(_Angle)) + (_OriginVector.Y * cosf(_Angle));
		Result.Z = _OriginVector.Z;
		return Result;
	}


	static float4 VectorRotationXToDeg(float4 _OriginVector, float _Angle)
	{
		return VectorRotationXToRad(_OriginVector, _Angle * UEngineMath::DToR);
	}

	static float4 VectorRotationXToRad(float4 _OriginVector, float _Angle)
	{
		float4 Result;
		Result.X = _OriginVector.X;
		Result.Y = (_OriginVector.Z * sinf(_Angle)) + (_OriginVector.Y * cosf(_Angle));
		Result.Z = (_OriginVector.Z * cosf(_Angle)) - (_OriginVector.Y * sinf(_Angle));
		return Result;
	}

	static float4 VectorRotationYToDeg(float4 _OriginVector, float _Angle)
	{
		return VectorRotationYToRad(_OriginVector, _Angle * UEngineMath::DToR);
	}

	static float4 VectorRotationYToRad(float4 _OriginVector, float _Angle)
	{
		float4 Result;
		Result.X = (_OriginVector.X * cosf(_Angle)) - (_OriginVector.Z * sinf(_Angle));
		Result.Y = _OriginVector.Y;
		Result.Z = (_OriginVector.X * sinf(_Angle)) + (_OriginVector.Z * cosf(_Angle));
		return Result;
	}


	static float4 Cross3D(float4 _Left, float4 _Right)
	{
		float4 Result;
		Result.X = (_Left.Y * _Right.Z) - (_Left.Z * _Right.Y);
		Result.Y = (_Left.Z * _Right.X) - (_Left.X * _Right.Z);
		Result.Z = (_Left.X * _Right.Y) - (_Left.Y * _Right.X);
		return Result;
	}

	static float DotProduct3D(float4 _Left, float4 _Right)
	{
		float Result = (_Left.X * _Right.X) + (_Left.Y * _Right.Y) + (_Left.Z * _Right.Z);
		return Result;
	}




	static float4 DegToDir(float _Angle)
	{
		return RadToDir(_Angle * UEngineMath::DToR);
	}
	static float4 RadToDir(float _Angle)
	{
		// Ư�� ������ ������ ���̰� 1�� ���� ���͸� �����ݴϴ�.
		return float4(cosf(_Angle), sinf(_Angle));
	}

	static float4 LerpClamp(float4 p1, float4 p2, float d1)
	{
		if (0.0f >= d1)
		{
			d1 = 0.0f;
		}

		if (1.0f <= d1)
		{
			d1 = 1.0f;
		}

		return Lerp(p1, p2, d1);
	}

	// p1 p2          d1�� ������ ����.
	static float4 Lerp(float4 p1, float4 p2, float d1)
	{
		return (p1 * (1.0f - d1)) + (p2 * d1);
	}

	float4 QuaternionToDeg() const
	{
		return QuaternionToRad() * UEngineMath::RToD;
	}

	float RightVectorToAngle2DDeg() const
	{
		return RightVectorToAngle2DRad() * UEngineMath::RToD;
	}

	float RightVectorToAngle2DRad() const
	{
		// �븻������ �Ǿ��ִ�.
		float4 ThisVector = *this;

		float4 NormalVector = ThisVector.Normalize2DReturn();

		float RadAngle = DotProduct3D(ThisVector.Normalize2DReturn(), float4::Right);

		float Angle = acosf(RadAngle);

		if (0.0f >= NormalVector.Y)
		{
			Angle = UEngineMath::PI2 - Angle;
		}

		return Angle;
	}

	// �׳� �𸮾� �ִ� ��ȯ�Լ� �賦.
	// ���� �̷а� ���� ���ؼ� �̰� �Ǵ��� �� ��
	float4 QuaternionToRad() const
	{
		float4 result;

		double sinrCosp = 2.0f * (W * Z + X * Y);
		double cosrCosp = 1.0f - 2.0f * (Z * Z + X * X);
		result.Z = static_cast<float>(atan2(sinrCosp, cosrCosp));

		double pitchTest = W * X - Y * Z;
		double asinThreshold = 0.4999995f;
		double sinp = 2.0f * pitchTest;

		if (pitchTest < -asinThreshold)
		{
			result.X = -(0.5f * UEngineMath::PI);
		}
		else if (pitchTest > asinThreshold)
		{
			result.X = (0.5f * UEngineMath::PI);
		}
		else
		{
			result.X = static_cast<float>(asin(sinp));
		}

		double sinyCosp = 2.0f * (W * Y + X * Z);
		double cosyCosp = 1.0f - 2.0f * (X * X + Y * Y);
		result.Y = static_cast<float>(atan2(sinyCosp, cosyCosp));

		return result;
	}


	float4 DegToQuaternion() const
	{
		// ��׸� ����
		float4 Result = *this;
		// ���� ����
		Result *= UEngineMath::DToR;
		// ���ʹϿ� ���� ��ȯ.
		Result.DirectVector = DirectX::XMQuaternionRotationRollPitchYawFromVector(Result.DirectVector);
		return Result;
	}

	float Size2D()
	{
		// sqrtf ������ �����ִ� �Լ�
		return std::sqrtf((X * X) + (Y * Y));
	}

	float Size3D()
	{
		// sqrtf ������ �����ִ� �Լ�
		return std::sqrtf((X * X) + (Y * Y) + (Z * Z));
	}

	void RotationXToDeg(float _Angle)
	{
		RotationXToRad(_Angle * UEngineMath::DToR);
	}

	void RotationXToRad(float _Angle)
	{
		*this = VectorRotationXToRad(*this, _Angle);
		return;
	}

	void RotationYToDeg(float _Angle)
	{
		RotationYToRad(_Angle * UEngineMath::DToR);
	}

	void RotationYToRad(float _Angle)
	{
		*this = VectorRotationYToRad(*this, _Angle);
		return;
	}




	void RotationZToDeg(float _Angle)
	{
		RotationZToRad(_Angle * UEngineMath::DToR);
	}

	void RotationZToRad(float _Angle)
	{
		*this = VectorRotationZToRad(*this, _Angle);
		return;
	}



	// �� �ڽ��� ���� 1¥���� ����Ǵ� ��.
	void Normalize2D()
	{
		float Size = Size2D();
		if (0.0f < Size && false == isnan(Size))
		{
			X /= Size;
			Y /= Size;
			Z = 0.0f;
			W = 0.0f;
		}
	}

	// ���� ��ȭ���� �ʰ� ���� 1¥���� ���� ���� ������ ���� ���͸� �����ϴ� �Լ�
	float4 Normalize2DReturn() const
	{
		float4 Result = *this;
		Result.Normalize2D();
		return Result;
	}

	void Normalize3D()
	{
		// w�� ���� �븻������
		DirectVector = DirectX::XMVector3Normalize(DirectVector);

		// float Size = Size3D();
		//if (0.0f < Size && false == isnan(Size))
		//{
		//	X /= Size;
		//	Y /= Size;
		//	Z /= Size;
		//	W = 0.0f;
		//}
	}

	// ���� ��ȭ���� �ʰ� ���� 1¥���� ���� ���� ������ ���� ���͸� �����ϴ� �Լ�
	float4 Normalize3DReturn() const
	{
		float4 Result = *this;
		Result.Normalize3D();
		return Result;
	}


	std::string ToString()
	{
		return "[X : " + std::to_string(X) + " Y : " + std::to_string(Y) + " Z : " + std::to_string(Z) + " W : " + std::to_string(W) + "]";
	}

	float4 Half2D() const
	{
		return { hX(), hY() };
	}

	bool IsZeroVector2D() const
	{
		return X == 0.0f && Y == 0.0f;
	}

	unsigned int uiX() const
	{
		return static_cast<unsigned int>(std::lround(X));
	}

	unsigned int uiY() const
	{
		return static_cast<unsigned int>(std::lround(Y));
	}

	int iX() const
	{
		return static_cast<int>(X);
	}

	int iY() const
	{
		return static_cast<int>(Y);
	}

	int iroundX() const
	{
		return std::lround(X);
	}

	int iroundY() const
	{
		return std::lround(Y);
	}



	float hX() const
	{
		return X * 0.5f;
	}

	float hY() const
	{
		return Y * 0.5f;
	}


	int ihY() const
	{
		return std::lround(hY());
	}

	int ihX() const
	{
		return std::lround(hX());
	}

	float4& operator=(const float4& _Other)
	{
		X = _Other.X;
		Y = _Other.Y;
		Z = _Other.Z;
		W = _Other.W;
		return *this;
	}


	float4 operator+(const float4& _Other) const
	{
		float4 Result = *this;
		Result.X += _Other.X;
		Result.Y += _Other.Y;
		Result.Z += _Other.Z;
		return Result;
	}

	float4& operator+=(const float4& _Other)
	{
		X += _Other.X;
		Y += _Other.Y;
		Z += _Other.Z;

		return *this;
	}

	float4 operator*(float _Value) const
	{
		float4 Result = *this;
		Result.X *= _Value;
		Result.Y *= _Value;
		Result.Z *= _Value;
		return Result;
	}

	float4& operator*=(float _Value)
	{
		X *= _Value;
		Y *= _Value;
		Z *= _Value;

		return *this;
	}

	float4& operator/=(float _Value)
	{
		X /= _Value;
		Y /= _Value;
		Z /= _Value;

		return *this;
	}

	float4 operator*(const class float4x4& _Other) const;
	float4& operator*=(const class float4x4& _Other);

	float4 operator*(const float4& _Other) const
	{
		float4 Result = *this;
		Result.X *= _Other.X;
		Result.Y *= _Other.Y;
		Result.Z *= _Other.Z;
		return Result;
	}

	float4& operator*=(const float4& _Other)
	{
		X *= _Other.X;
		Y *= _Other.Y;
		Z *= _Other.Z;

		return *this;
	}

	float4 operator-() const
	{
		float4 Result;
		Result.X = -X;
		Result.Y = -Y;
		Result.Z = -Z;
		return Result;
	}

	float4 operator-(const float4& _Other) const
	{
		float4 Result = *this;
		Result.X -= _Other.X;
		Result.Y -= _Other.Y;
		Result.Z -= _Other.Z;
		return Result;
	}

	float4& operator-=(const float4& _Other)
	{
		X -= _Other.X;
		Y -= _Other.Y;
		Z -= _Other.Z;

		return *this;
	}

	POINT ConvertToWinApiPOINT()
	{
		return { iX(),iY() };
	}
};

using FVector = float4;
using FColor = float4;

class float4x4 operator*(const class float4x4& _Left, const class float4x4& _Right);

class float4x4
{
public:
	union
	{
		struct
		{
			float v00;
			float v01;
			float v02;
			float v03;

			float v10;
			float v11;
			float v12;
			float v13;

			float v20;
			float v21;
			float v22;
			float v23;

			float v30;
			float v31;
			float v32;
			float v33;
		};

		float4 ArrVector[4];
		float Arr1D[16] = { };
		float Arr2D[4][4];

		// ���̷�Ʈ���� ����ϴ� �Լ��� ����Ϸ��� ���̷�Ʈ���� �����ϴ� vector�� ����ؾ� �Ѵ�.
		// ���̷�Ʈ �Լ��� �� ������?
		// ���ο��� SIMD������ ����Ѵ�.
		// CPU���� �׷���ī��� ���� �ѹ��� XYZW�� ���ÿ� ����ó�� �ϴ� ���
		DirectX::XMMATRIX DirectMatrix;

	};


	float4x4& operator=(const float4x4& _Value)
	{
		// Arr1D�� �����ض�
		// sizeof(float) * 16 ũ���
		// _Value.Arr1D�� �����ϴ� ������ 
		// sizeof(float) * 16 ũ�⸸ŭ
		memcpy_s(Arr1D, sizeof(float) * 16, _Value.Arr1D, sizeof(float) * 16);

		return *this;
	}

	float4x4 operator*(const float4x4& _Value)
	{
		return ::operator*(*this, _Value);
	}

	// ũ�� ����̶�� �մϴ�.
	void Scale(float4 _Value)
	{
		Identity();

		DirectMatrix = DirectX::XMMatrixScalingFromVector(_Value.DirectVector);

		//Arr2D[0][0] = _Value.X;
		//Arr2D[1][1] = _Value.Y;
		//Arr2D[2][2] = _Value.Z;
		//Arr2D[3][3] = 1.0f;
	}

	void Position(float4 _Value)
	{
		Identity();
		DirectMatrix = DirectX::XMMatrixTranslationFromVector(_Value.DirectVector);

		//Arr2D[3][0] = _Value.X;
		//Arr2D[3][1] = _Value.Y;
		//Arr2D[3][2] = _Value.Z;
	}

	void Identity()
	{
		// ���Լ� ����� ��������.
		// Arr1D �ּҰ���ġ���� 100
		// sizeof(float) * 16 ũ�⸸ŭ�� 164 ���� ������
		// 0���� ä����.
		// memset(Arr1D, 0, sizeof(float) * 16);
		//Arr2D[0][0] = 1.0f;
		//Arr2D[1][1] = 1.0f;
		//Arr2D[2][2] = 1.0f;
		//Arr2D[3][3] = 1.0f;

		DirectMatrix = DirectX::XMMatrixIdentity();
	}

	float4 LeftVector()
	{
		return -ArrVector[0].Normalize3DReturn();
	}

	float4 RightVector()
	{
		return ArrVector[0].Normalize3DReturn();
	}

	float4 UpVector()
	{
		return ArrVector[1].Normalize3DReturn();
	}

	float4 DownVector()
	{
		return -ArrVector[1].Normalize3DReturn();
	}

	float4 ForwardVector()
	{
		return ArrVector[2].Normalize3DReturn();
	}

	float4 BackVector()
	{
		return -ArrVector[2].Normalize3DReturn();
	}

	void Decompose(float4& _Scale, float4& _Rotation, float4& _Position)
	{
		// _Rotation => �̰� ���ʹϿ����� ���´�.

		DirectX::XMMatrixDecompose(&_Scale.DirectVector, &_Rotation.DirectVector, &_Position.DirectVector, DirectMatrix);
	}

	void RotationDeg(const float4 _Value)
	{
		Identity();
		//float4x4 X;
		//X.RotationXDeg(_Value.X);
		//float4x4 Y;
		//Y.RotationYDeg(_Value.Y);
		//float4x4 Z;
		//Z.RotationZDeg(_Value.Z);
		// *this = X * Y * Z;

		float4x4 X;
		float4x4 Y;
		float4x4 Z;

		X.RotationXDeg(_Value.X);
		Y.RotationYDeg(_Value.Y);
		Z.RotationZDeg(_Value.Z);

		*this = X * Y * Z;

		return;
	}

	void RotationXDeg(float _Angle)
	{
		RotationXRad(_Angle * UEngineMath::DToR);
	}

	void RotationXRad(float _Angle)
	{
		Identity();
		DirectMatrix = DirectX::XMMatrixRotationX(_Angle);
		//Arr2D[1][1] = cosf(_Angle);
		//Arr2D[1][2] = sinf(_Angle);
		//Arr2D[2][1] = -sinf(_Angle);
		//Arr2D[2][2] = cosf(_Angle);
	}

	void RotationYDeg(float _Angle)
	{
		RotationYRad(_Angle * UEngineMath::DToR);
	}

	void RotationYRad(float _Angle)
	{
		Identity();
		DirectMatrix = DirectX::XMMatrixRotationY(_Angle);

		//Arr2D[0][0] = cosf(_Angle);
		//Arr2D[0][2] = -sinf(_Angle);
		//Arr2D[2][0] = sinf(_Angle);
		//Arr2D[2][2] = cosf(_Angle);
	}

	void RotationZDeg(float _Angle)
	{
		RotationZRad(_Angle * UEngineMath::DToR);
	}

	void RotationZRad(float _Angle)
	{
		// �ڽý���
		Identity();
		DirectMatrix = DirectX::XMMatrixRotationZ(_Angle);

		//Arr2D[0][0] = cosf(_Angle);
		//Arr2D[0][1] = sinf(_Angle);
		//Arr2D[1][0] = -sinf(_Angle);
		//Arr2D[1][1] = cosf(_Angle);
	}

	void Transpose()
	{
		DirectMatrix = DirectX::XMMatrixTranspose(DirectMatrix);


		//float4x4 Result = *this;
		//for (size_t y = 0; y < 4; y++)
		//{
		//	for (size_t x = 0; x < 4; x++)
		//	{
		//		Result.Arr2D[y][x] = Arr2D[x][y];
		//	}
		//}

		//*this = Result;
	}

	// ��ġ���
	float4x4 TransposeReturn()
	{
		float4x4 Result = *this;
		Result.DirectMatrix = DirectX::XMMatrixTranspose(DirectMatrix);


		//for (size_t y = 0; y < 4; y++)
		//{
		//	for (size_t x = 0; x < 4; x++)
		//	{
		//		Result.Arr2D[y][x] = Arr2D[x][y];
		//	}
		//}
		return Result;
	}

	// const float4 _EyePos �ٶ󺸴� ����� ��ġ
	// const float4 _EyeDir �ٶ󺸴� ����� �ٶ󺸴� ����
	// const float4 _EyeUp �ٶ󺸴� ����� �ٶ󺸴� ����� ������ ����.
	void LookToLH(const float4 _EyePos, const float4 _EyeDir, const float4 _EyeUp)
	{
		// ī�޶� ȸ���� �ݴ�������� ȸ�����Ѿ� �մϴ�.
		// ȸ������� ����� ���� �մϴ�.

		DirectMatrix = DirectX::XMMatrixLookToLH(_EyePos.DirectVector, _EyeDir.DirectVector, _EyeUp.DirectVector);

		//// ȸ������� ���Ҽ� �ִ� �����
		//// �ڽý��ڸ� ���ؼ� ���ϴ� ����� �ִ�.
		//// ȸ���� ����
		//// ������ 3���� ������ ������ �� => ȸ�����
		//// ȸ������� �Ƿ���
		//// 3���� ���Ͱ� ������ �����̾�� ��.

		//// ũ������ ������ ���� ���ؼ�
		//FVector Up = _EyeUp.Normalize3DReturn();
		//FVector Forward = _EyeDir.Normalize3DReturn();
		//FVector Right = FVector::Cross3D(Up, Forward);
		//Up.W = 0.0f;
		//Forward.W = 0.0f;
		//Right.W = 0.0f;

		//// �����ؼ� ������ �κ��� ���ϰ�
		//View.ArrVector[0] = Right;
		//View.ArrVector[1] = Up;
		//View.ArrVector[2] = Forward;
		//View.Transpose();


		//float4 NegEyePos = -_EyePos;
		//NegEyePos.W = 1.0f;

		//// �ᱹ ���� ī�޶��� ��ġ�� ȸ�����Ѽ� ����� �Ѵ�.
		//// float4 Result = NegEyePos * View;
		//// �̶� ������ ���˴ϴ�.
		//View.ArrVector[3].X = float4::DotProduct3D(Right, NegEyePos);
		//View.ArrVector[3].Y = float4::DotProduct3D(Up, NegEyePos);
		//View.ArrVector[3].Z = float4::DotProduct3D(Forward, NegEyePos);

		//*this = View;

		return;
	}


	void OrthographicLH(float _Width, float _Height, float _Near, float _Far)
	{
		// _Near ���� ������ ���̴� ���
		// _Far ���� �ָ����� ���̴� ���

		Identity();

		DirectMatrix = DirectX::XMMatrixOrthographicLH(_Width, _Height, _Near, _Far);

		// N   F
		// 100 1000
		// Range 900
		//float fRange = 1.0f / (_Far - _Near);

		//Arr2D[0][0] = 2.0f / _Width;
		//Arr2D[1][1] = 2.0f / _Height;
		//Arr2D[2][2] = fRange;
		//// �����Ϳ� Z�� ������ ��ġ�� ����
		//Arr2D[3][2] = -fRange * _Near;

	}

	// float _Width, float _Height ȭ�� ũ��� �־��� �ʿ䰡 ����.
	// 1280 720 == 12.8 7.2
	void PerspectiveFovDeg(float _FovAngle, float _Width, float _Height, float _Near, float _Far)
	{
		PerspectiveFovRad(_FovAngle * UEngineMath::DToR, _Width, _Height, _Near, _Far);
	}

	void PerspectiveFovRad(float _FovAngle, float _Width, float _Height, float _Near, float _Far)
	{
		Identity();

		DirectMatrix = DirectX::XMMatrixPerspectiveFovLH(_FovAngle, _Width / _Height, _Near, _Far);


		//// w�� z�� ������ ���� ��
		//Arr2D[2][3] = 1.0f;

		//// �� w���� 1�ȴ��� ���� �������
		//Arr2D[3][3] = 0.0f;
		//
		//// _FovAngle y����
		//float Ratio = _Width / _Height;
		//float DivFov = _FovAngle / 2.0f;

		//	// x�� ���� �ȴ�.
		//Arr2D[0][0] = 1.0f / (tanf(DivFov) * Ratio);

		//Arr2D[1][1] = 1.0f / tanf(DivFov);
		//Arr2D[2][2] = _Far / (_Far - _Near);

		//Arr2D[3][2] = -(_Near * _Far) / (_Far - _Near);
	}

	float4x4 InverseReturn()
	{
		float4x4 Result = *this;
		Result.DirectMatrix = DirectX::XMMatrixInverse(nullptr, DirectMatrix);
		return Result;
	}

	// ����� ũ��� Ȯ��
	void ViewPort(float _Width, float _Height, float _Left, float _Right, float _ZMin, float _ZMax)
	{
		Identity();

		Arr2D[0][0] = _Width * 0.5f;
		Arr2D[1][1] = -_Height * 0.5f;
		Arr2D[2][2] = _ZMax != 0.0f ? 1.0f : _ZMin / _ZMax;

		Arr2D[3][0] = Arr2D[0][0] + _Left;
		Arr2D[3][1] = -Arr2D[1][1] + _Right;
		Arr2D[3][2] = _ZMax != 0.0f ? 0.0f : _ZMin / _ZMax;
		Arr2D[3][3] = 1.0f;
	}


	float4x4()
	{
		Identity();
	}
};

using FMatrix = float4x4;

class Color8Bit
{
	// ���ǿ����� ������
	// �������� ġ�� �ټ����� ��ο��
	// ������ ġ�� �ټ����� ���
	// ��ǻ�ʹ� ���� ������� ����մϴ�.
public:
	static const Color8Bit Black;
	static const Color8Bit Red;
	static const Color8Bit Green;
	static const Color8Bit Blue;
	static const Color8Bit Yellow;
	static const Color8Bit White;
	static const Color8Bit Magenta;
	static const Color8Bit Orange;

	static const Color8Bit BlackA;
	static const Color8Bit RedA;
	static const Color8Bit GreenA;
	static const Color8Bit BlueA;
	static const Color8Bit YellowA;
	static const Color8Bit WhiteA;
	static const Color8Bit MagentaA;
	static const Color8Bit OrangeA;
	static const Color8Bit CyanA;

	union
	{
		struct
		{
			unsigned char R;
			unsigned char G;
			unsigned char B;
			unsigned char A;
		};

		unsigned char Arr1D[4] = { 0,0,0,255 };
		unsigned int Color;
	};

	Color8Bit()
	{

	}

	Color8Bit(
		unsigned char _R,
		unsigned char _G,
		unsigned char _B,
		unsigned char _A
	)
		:R(_R), G(_G), B(_B), A(_A)
	{

	}

	bool operator==(Color8Bit _Color)
	{
		return Color == _Color.Color;
	}

	Color8Bit ZeroAlphaColor() const
	{
		return Color8Bit{ R,G,B,0 };
	}
};
