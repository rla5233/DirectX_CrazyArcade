#pragma once
#include "BlockBase.h"

// ���� : Box Block
class ABox : public ABlockBase
{
	GENERATED_BODY(ABlockBase)
public:
	// constrcuter destructer
	ABox();
	~ABox();

	// delete Function
	ABox(const ABox& _Other) = delete;
	ABox(ABox&& _Other) noexcept = delete;
	ABox& operator=(const ABox& _Other) = delete;
	ABox& operator=(ABox&& _Other) noexcept = delete;

public:
	inline void SetSpawnItemType(EItemType _ItemType)
	{
		ItemType = _ItemType;
	}
	
protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
		
	void StateInit() override;

protected:
	EItemType ItemType = EItemType::None;

};

