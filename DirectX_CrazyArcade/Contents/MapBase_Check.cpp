#include "PreCompile.h"
#include "MapBase.h"

#include "MainPlayLevel.h"
#include "MapConstant.h"

#include "ServerTestPlayer.h"
#include "BlockBase.h"
#include "MoveBox.h"

// ������ �� �ִ� ��ġ������ ��ȯ
bool AMapBase::CanMovePos(const FVector& _NextPos, const FVector& _Dir)
{
	SetCheckPos(_NextPos, _Dir);
	float NextPosFX = NextPos.X / BlockSize;
	float NextPosFY = NextPos.Y / BlockSize;

	bool Result = false;

	// �� ���� ���϶�
	if (SizeX <= NextPoint.X || 0.0f > NextPosFX || SizeY <= NextPoint.Y || 0.0f > NextPosFY)
	{
		Result = false;
		return Result;
	}

	// �� �����϶�
	if (nullptr == TileInfo[NextPoint.Y][NextPoint.X].Block)
	{
		Result = true;

		// ���� ��ġ1 üũ
		if (true == MapRangeCheckByPoint(NextPoint1) && nullptr != TileInfo[NextPoint1.Y][NextPoint1.X].Block)
		{
			EBlockType BlockType1 = TileInfo[NextPoint1.Y][NextPoint1.X].Block->GetBlockType();
			if (EBlockType::Wall == BlockType1 || EBlockType::Box == BlockType1)
			{
				Result = false;
				return Result;
			}

			Result = SubMoveBoxCheck(NextPoint1, _Dir);
		}

		// ���� ��ġ2 üũ
		if (true == MapRangeCheckByPoint(NextPoint2) && nullptr != TileInfo[NextPoint2.Y][NextPoint2.X].Block)
		{
			EBlockType BlockType2 = TileInfo[NextPoint2.Y][NextPoint2.X].Block->GetBlockType();
			if (EBlockType::Wall == BlockType2 || EBlockType::Box == BlockType2)
			{
				Result = false;
				return Result;
			}

			Result = SubMoveBoxCheck(NextPoint2, _Dir);
		}

		return Result;
	}

	// ������Ʈ �����Ҷ�
	EBlockType BlockType = TileInfo[NextPoint.Y][NextPoint.X].Block->GetBlockType();
	if (EBlockType::Wall == BlockType || EBlockType::Box == BlockType)
	{
		Result = false;
		return Result;
	}

	// MoveBox üũ
	if (EBlockType::MoveBox == BlockType)
	{
		// SubPos üũ
		bool Sub1 = true;
		if (NextPoint != NextPoint1)
		{
			Sub1 = SubMoveBoxOnlyCheck(NextPoint1, _Dir);
		}

		bool Sub2 = true;
		if (NextPoint != NextPoint2)
		{
			Sub2 = SubMoveBoxOnlyCheck(NextPoint2, _Dir);
		}

		if (false == Sub1 || false == Sub2)
		{
			Result = false;
			return Result;
		}

		AMoveBox* MoveBox = dynamic_cast<AMoveBox*>(TileInfo[NextPoint.Y][NextPoint.X].Block);
		FPoint TwoStepPoint = NextPoint;

		if (0.0f < _Dir.X)
		{
			TwoStepPoint.X += 1;
		}
		else if (0.0f > _Dir.X)
		{
			TwoStepPoint.X -= 1;
		}
		else if (0.0f < _Dir.Y)
		{
			TwoStepPoint.Y += 1;
		}
		else if (0.0f > _Dir.Y)
		{
			TwoStepPoint.Y -= 1;
		}

		// MoveBox Move ������ ���
		if (BlockState::move == MoveBox->GetCurState())
		{
			Result = false;

			FVector PlayerPos = _NextPos;
			FVector BoxPos = MoveBox->GetActorLocation();
			BoxPos.X += BlockSize * 0.5f;
			BoxPos.Y += BlockSize * 0.5f;

			if (true == MovingBoxColCheck(MoveBox->GetMoveDir(), _Dir))
			{
				return false;
			}

			if (0.0f < _Dir.X || 0.0f > _Dir.X)
			{
				float diffX = abs(PlayerPos.X - BoxPos.X);
				if (BlockSize < diffX)
				{
					Result = true;
				}
			}
			else if (0.0f < _Dir.Y || 0.0f > _Dir.Y)
			{
				float diffY = abs(PlayerPos.Y - BoxPos.Y);
				if (BlockSize < diffY)
				{
					Result = true;
				}
			}

			return Result;
		}

		// MoveBox �̵� ��ġ�� ��� ���� ���
		if (0 > TwoStepPoint.X || SizeX <= TwoStepPoint.X
		||  0 > TwoStepPoint.Y || SizeY <= TwoStepPoint.Y
		||  nullptr != TileInfo[TwoStepPoint.Y][TwoStepPoint.X].Block
		||  false == TileInfo[TwoStepPoint.Y][TwoStepPoint.X].AllBomb.empty())
		{
			Result = false;
			return Result;
		}

		// MoveBox Idle ������ ���
		if (BlockState::idle == MoveBox->GetCurState())
		{
			MoveBox->SetMoveState(_Dir);
			Result = false;
			return Result;
		}

		Result = false;
		return Result;
	}

	return Result;
}

// �÷��̾� ���� �����϶� ������ �� �ִ� ��ġ������ ��ȯ
bool AMapBase::CanMovePosInTraped(const FVector& _NextPos, const FVector& _Dir)
{
	SetCheckPos(_NextPos, _Dir);
	float NextPosFX = NextPos.X / BlockSize;
	float NextPosFY = NextPos.Y / BlockSize;

	bool Result = false;

	// �� ���� ���϶�
	if (SizeX <= NextPoint.X || 0.0f > NextPosFX || SizeY <= NextPoint.Y || 0.0f > NextPosFY)
	{
		Result = false;
		return Result;
	}

	// �� �����϶�
	if (nullptr == TileInfo[NextPoint.Y][NextPoint.X].Block)
	{
		Result = true;

		// ���� ��ġ1 üũ
		if (true == MapRangeCheckByPoint(NextPoint1) && nullptr != TileInfo[NextPoint1.Y][NextPoint1.X].Block)
		{
			EBlockType BlockType1 = TileInfo[NextPoint1.Y][NextPoint1.X].Block->GetBlockType();
			if (EBlockType::Wall == BlockType1 || EBlockType::Box == BlockType1)
			{
				Result = false;
				return Result;
			}

			Result = SubMoveBoxOnlyCheck(NextPoint1, _Dir);
		}

		// ���� ��ġ2 üũ
		if (true == MapRangeCheckByPoint(NextPoint2) && nullptr != TileInfo[NextPoint2.Y][NextPoint2.X].Block)
		{
			EBlockType BlockType2 = TileInfo[NextPoint2.Y][NextPoint2.X].Block->GetBlockType();
			if (EBlockType::Wall == BlockType2 || EBlockType::Box == BlockType2)
			{
				Result = false;
				return Result;
			}

			Result = SubMoveBoxOnlyCheck(NextPoint2, _Dir);
		}

		return Result;
	}

	// ������Ʈ �����Ҷ�
	EBlockType BlockType = TileInfo[NextPoint.Y][NextPoint.X].Block->GetBlockType();
	if (EBlockType::Wall == BlockType || EBlockType::Box == BlockType)
	{
		Result = false;
		return Result;
	}

	// MoveBox üũ
	if (EBlockType::MoveBox == BlockType)
	{
		// SubPos üũ
		bool Sub1 = true;
		if (NextPoint != NextPoint1)
		{
			Sub1 = SubMoveBoxOnlyCheck(NextPoint1, _Dir);
		}

		bool Sub2 = true;
		if (NextPoint != NextPoint2)
		{
			Sub2 = SubMoveBoxOnlyCheck(NextPoint2, _Dir);
		}

		if (false == Sub1 || false == Sub2)
		{
			Result = false;
			return Result;
		}

		AMoveBox* MoveBox = dynamic_cast<AMoveBox*>(TileInfo[NextPoint.Y][NextPoint.X].Block);
		FPoint TwoStepPoint = NextPoint;

		if (0.0f < _Dir.X)
		{
			TwoStepPoint.X += 1;
		}
		else if (0.0f > _Dir.X)
		{
			TwoStepPoint.X -= 1;
		}
		else if (0.0f < _Dir.Y)
		{
			TwoStepPoint.Y += 1;
		}
		else if (0.0f > _Dir.Y)
		{
			TwoStepPoint.Y -= 1;
		}

		// MoveBox Move ������ ���
		if (BlockState::move == MoveBox->GetCurState())
		{
			Result = false;

			FVector PlayerPos = _NextPos;
			FVector BoxPos = MoveBox->GetActorLocation();
			BoxPos.X += BlockSize * 0.5f;
			BoxPos.Y += BlockSize * 0.5f;

			if (true == MovingBoxColCheck(MoveBox->GetMoveDir(), _Dir))
			{
				return false;
			}

			if (0.0f < _Dir.X || 0.0f > _Dir.X)
			{
				float diffX = abs(PlayerPos.X - BoxPos.X);
				if (BlockSize < diffX)
				{
					Result = true;
				}
			}
			else if (0.0f < _Dir.Y || 0.0f > _Dir.Y)
			{
				float diffY = abs(PlayerPos.Y - BoxPos.Y);
				if (BlockSize < diffY)
				{
					Result = true;
				}
			}

			return Result;
		}

		// MoveBox �̵� ��ġ�� ��� ���� ���
		if (0 > TwoStepPoint.X || SizeX <= TwoStepPoint.X
		||  0 > TwoStepPoint.Y || SizeY <= TwoStepPoint.Y
		||  nullptr != TileInfo[TwoStepPoint.Y][TwoStepPoint.X].Block
		||  false == TileInfo[TwoStepPoint.Y][TwoStepPoint.X].AllBomb.empty())
		{
			Result = false;
			return Result;
		}

		// MoveBox Idle ������ ���
		if (BlockState::idle == MoveBox->GetCurState())
		{
			Result = false;
			return Result;
		}

		Result = false;
		return Result;
	}

	return Result;
}

// MoveBox ��ġ Ȯ�� ������ �� ������ true (MoveBox ���� ��ȭ ����)
bool AMapBase::SubMoveBoxCheck(FPoint _NextPoint, const FVector& _Dir)
{
	if (nullptr == TileInfo[_NextPoint.Y][_NextPoint.X].Block)
	{
		return true;
	}

	EBlockType BlockType = TileInfo[_NextPoint.Y][_NextPoint.X].Block->GetBlockType();
	if (EBlockType::MoveBox == BlockType)
	{
		AMoveBox* MoveBox = dynamic_cast<AMoveBox*>(TileInfo[_NextPoint.Y][_NextPoint.X].Block);
		FPoint TwoStepPoint = _NextPoint;

		if (0.0f < _Dir.X)
		{
			TwoStepPoint.X += 1;
		}
		else if (0.0f > _Dir.X)
		{
			TwoStepPoint.X -= 1;
		}
		else if (0.0f < _Dir.Y)
		{
			TwoStepPoint.Y += 1;
		}
		else if (0.0f > _Dir.Y)
		{
			TwoStepPoint.Y -= 1;
		}

		// MoveBox Move ������ ���
		if (BlockState::move == MoveBox->GetCurState())
		{
			FVector PlayerPos = PlayLevel->GetPlayer()->GetActorLocation();
			FVector BoxPos = MoveBox->GetActorLocation();
			BoxPos.X += BlockSize * 0.5f;
			BoxPos.Y += BlockSize * 0.5f;

			if (true == MovingBoxColCheck(MoveBox->GetMoveDir(), _Dir))
			{
				return false;
			}

			if (0.0f < _Dir.X || 0.0f > _Dir.X)
			{
				float diffX = abs(PlayerPos.X - BoxPos.X);
				if (BlockSize < diffX)
				{
					return true;
				}
			}
			else if (0.0f < _Dir.Y || 0.0f > _Dir.Y)
			{
				float diffY = abs(PlayerPos.Y - BoxPos.Y);
				if (BlockSize < diffY)
				{
					return true;
				}
			}
		}

		if (0 > TwoStepPoint.X || SizeX <= TwoStepPoint.X
		||  0 > TwoStepPoint.Y || SizeY <= TwoStepPoint.Y
		||  nullptr != TileInfo[TwoStepPoint.Y][TwoStepPoint.X].Block
		||  false == TileInfo[TwoStepPoint.Y][TwoStepPoint.X].AllBomb.empty())
		{
			return false;
		}

		if (BlockState::idle == MoveBox->GetCurState())
		{
			MoveBox->SetMoveState(_Dir);
			return false;
		}

		return false;
	}

	return true;
}

// MoveBox ��ġ Ȯ�� ������ �� ������ true (MoveBox ���� ��ȭ ����)
bool AMapBase::SubMoveBoxOnlyCheck(FPoint _NextPoint, const FVector& _Dir)
{
	if (nullptr == TileInfo[_NextPoint.Y][_NextPoint.X].Block)
	{
		return true;
	}

	EBlockType BlockType = TileInfo[_NextPoint.Y][_NextPoint.X].Block->GetBlockType();
	if (EBlockType::MoveBox == BlockType)
	{
		AMoveBox* MoveBox = dynamic_cast<AMoveBox*>(TileInfo[_NextPoint.Y][_NextPoint.X].Block);
		FPoint TwoStepPoint = _NextPoint;

		if (0.0f < _Dir.X)
		{
			TwoStepPoint.X += 1;
		}
		else if (0.0f > _Dir.X)
		{
			TwoStepPoint.X -= 1;
		}
		else if (0.0f < _Dir.Y)
		{
			TwoStepPoint.Y += 1;
		}
		else if (0.0f > _Dir.Y)
		{
			TwoStepPoint.Y -= 1;
		}

		if (0 > TwoStepPoint.X || SizeX <= TwoStepPoint.X
		||  0 > TwoStepPoint.Y || SizeY <= TwoStepPoint.Y
		||  nullptr != TileInfo[TwoStepPoint.Y][TwoStepPoint.X].Block
		||  false == TileInfo[TwoStepPoint.Y][TwoStepPoint.X].AllBomb.empty())
		{
			return false;
		}

		if (BlockState::idle == MoveBox->GetCurState())
		{
			return false;
		}

		return false;
	}

	return true;
}

// MoveBox �����̴� ���� ���� �浹 üũ
bool AMapBase::MovingBoxColCheck(const FVector& _MoveBoxDir, const FVector& _PlayerDir)
{
	if (0.0f < _MoveBoxDir.X && 0.0f > _PlayerDir.X)
	{
		return true;
	}

	if (0.0f > _MoveBoxDir.X && 0.0f < _PlayerDir.X)
	{
		return true;
	}

	if (0.0f < _MoveBoxDir.Y && 0.0f > _PlayerDir.Y)
	{
		return true;
	}

	if (0.0f > _MoveBoxDir.Y && 0.0f < _PlayerDir.Y)
	{
		return true;
	}

	return false;
}

// üũ ��ġ ����
void AMapBase::SetCheckPos(const FVector& _NextPos, const FVector& _Dir)
{
	NextPos = _NextPos - StartPos;

	if (0.0f < _Dir.X)			// ��
	{
		NextPos.X += BlockCheckAdjPosX;

		NextPos1 = NextPos;
		NextPos1.Y += BlockCheckSubPosY;

		NextPos2 = NextPos;
		NextPos2.Y -= BlockCheckSubPosY;
	}
	else if (0.0f > _Dir.X)		// ��
	{
		NextPos.X -= BlockCheckAdjPosX;

		NextPos1 = NextPos;
		NextPos1.Y += BlockCheckSubPosY;

		NextPos2 = NextPos;
		NextPos2.Y -= BlockCheckSubPosY;
	}
	else if (0.0f < _Dir.Y)		// ��
	{
		NextPos.Y += BlockCheckAdjUpPos;

		NextPos1 = NextPos;
		NextPos1.X += BlockCheckSubPosX;

		NextPos2 = NextPos;
		NextPos2.X -= BlockCheckSubPosX;
	}
	else if (0.0f > _Dir.Y)		// ��
	{
		NextPos.Y -= BlockCheckAdjDownPos;

		NextPos1 = NextPos;
		NextPos1.X += BlockCheckSubPosX;

		NextPos2 = NextPos;
		NextPos2.X -= BlockCheckSubPosX;
	}

	NextPoint.X = static_cast<int>(NextPos.X / BlockSize);
	NextPoint.Y = static_cast<int>(NextPos.Y / BlockSize);

	NextPoint1.X = static_cast<int>(NextPos1.X / BlockSize);
	NextPoint1.Y = static_cast<int>(NextPos1.Y / BlockSize);

	NextPoint2.X = static_cast<int>(NextPos2.X / BlockSize);
	NextPoint2.Y = static_cast<int>(NextPos2.Y / BlockSize);
}