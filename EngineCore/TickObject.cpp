#include "PreCompile.h"
#include "TickObject.h"

UTickObject::UTickObject() 
{
}

UTickObject::~UTickObject() 
{
}

void UTickObject::BeginPlay()
{

}

void UTickObject::Tick(float _DeltaTime)
{
	if (false == IsActive())
	{
		return;
	}

	for (size_t i = 0; i < CallBacks.size(); i++)
	{
		if (nullptr == CallBacks[i])
		{
			continue;
		}

		CallBacks[i]->Time -= _DeltaTime;
		if (0 >= CallBacks[i]->Time)
		{
			if (nullptr == CallBacks[i]->CallBack)
			{
				MsgBoxAssert("�Լ��� �������� ���� ������ �ݹ����� ����Ϸ��� �߽��ϴ�.");
			}
			
			CallBacks[i]->CallBack();
			CallBacks[i] = nullptr;
		}
	}
}