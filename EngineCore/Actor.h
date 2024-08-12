#pragma once
#include <EngineBase/Transform.h>

#include <vector>
#include "TickObject.h"
#include "WorldObject.h"
#include "Level.h"

// ���� : ���Ͱ� Ʈ�������� �����°� �ƴ϶�
class ULevel;
class UActorComponent;
class USceneComponent;
class AActor : public UTickObject, public UWorldObject, public UNameObject
{
	friend ULevel;
	GENERATED_BODY(UTickObject)

public:
	// constrcuter destructer
	AActor();
	~AActor();

	// delete Function
	AActor(const AActor& _Other) = delete;
	AActor(AActor&& _Other) noexcept = delete;
	AActor& operator=(const AActor& _Other) = delete;
	AActor& operator=(AActor&& _Other) noexcept = delete;

	// ������ �����ڿ����� ȣ��ɼ� �ְ� ����� ���´�.
	// �𸮾� �����ҷ��� �׳� ��.
	template<typename ComponentType>
	ComponentType* CreateDefaultSubObject(std::string_view _Name)
	{
		if (false == ULevel::IsActorConstructer)
		{
			MsgBoxAssert("�𸮾󿡼��� �����ڿ����ۿ� ������Ʈ�� �����Ҽ� �����ϴ�.");
		}

		std::shared_ptr<UActorComponent> NewComponent = std::make_shared<ComponentType>();

		PushComponent(NewComponent, _Name);

		return dynamic_cast<ComponentType*>(NewComponent.get());
	}

	FTransform& GetActorTransform();
	void SetActorTransform(const FTransform& _Transform);

	FVector GetActorForwardVector();
	FVector GetActorRightVector();
	FVector GetActorUpVector();

	FVector GetActorLocation();
	FVector GetActorScale3D();

	void SetActorScale3D(FVector _Value);
	void SetActorRotation(FVector _Value);
	void SetActorLocation(FVector _Value);

	void AddActorScale3D(FVector _Value);
	void AddActorRotation(FVector _Value);
	void AddActorLocation(FVector _Value);

	inline USceneComponent* GetRoot() const
	{
		if (nullptr == RootComponent)
		{
			MsgBoxAssert("���� ��Ʈ�� �������� �ʾҽ��ϴ�.");
		}

		return RootComponent;
	}

	void SetRoot(USceneComponent* _Root)
	{
		if (nullptr != RootComponent)
		{
			MsgBoxAssert("�̹� ��Ʈ�� ������ �����Դϴ�.");
		}

		RootComponent = _Root;
	}

	bool IsDestroy()
	{
		return IsDestroyValue;
	}

	void Destroy()
	{
		IsDestroyValue = true;
	}

	virtual void End() {};

	// �ȺƸ� �Ȱ��� ������� ���� �������̽�
	template<typename WidgetType>
	WidgetType* CreateWidget(ULevel* _Level, std::string_view _Name)
	{
		std::shared_ptr<UWidget> NewWidget = std::make_shared<WidgetType>();

		WidgetInit(NewWidget, _Name);

		return dynamic_cast<WidgetType*>(NewWidget.get());
	}

	template<typename Class>
	std::vector<std::shared_ptr<Class>> GetComponentToClass()
	{
		std::vector<std::shared_ptr<Class>> FindVector;

		for (size_t i = 0; i < Components.size(); i++)
		{
			std::shared_ptr<Class> ClassType = std::dynamic_pointer_cast<Class>(Components[i]);

			if (nullptr != ClassType)
			{
				FindVector.push_back(ClassType);
			}
		}

		return FindVector;
	}



protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	// �𸮾��� �� ���Ͱ� Ʈ�������� ������ ���ϰ� ������?
	// GameMode ������� �鿡�� Ʈ�������� �ʿ� ���� �ֵ鵵 ���ƿ�.
	// ���Ͷ� �� ��� �������� ��� ������ ���ֽø� �ϸ� �ȴ�.
	// �ű⿡ �ʿ��� ��ɸ� ����� �Ǳ� ������ 
	// ������Ʈ ������� ��.
	// ������� �������� ����� �ִ� ������ ����°�.
	USceneComponent* RootComponent = nullptr;
	std::vector<std::shared_ptr<UActorComponent>> Components;

	void PushComponent(std::shared_ptr<UActorComponent> _Component, std::string_view _Name);
	/////////////////////// ��ǲ





public:
	void InputOff();
	void InputOn();

	// �̳༮�� ����
	// UEngineInput <= �׻��� Ű������

	// ���Ϳ����� ��ǲ ó���� �����ϰ� �Ǿ����ϴ�.
	bool IsDoubleClick(int _Key, float _ClickTime);
	bool IsDown(int _Key);
	float GetPressTime(int _Key);
	bool IsPress(int _Key);
	bool IsUp(int _Key);
	bool IsFree(int _Key);
	bool IsAnykeyDown();
	bool IsAnykeyPress();
	bool IsAnykeyUp();
	bool IsAnykeyFree();

	static void OnlyInput(AActor* _this);
	static void OnlyInputStop();

private:
	bool IsDestroyValue = false;
	// set�� ������ ������ �ڷᱸ���μ�
	// Value ���� ���Դϴ�.
	static std::set<AActor*> InputActors;
	static std::set<AActor*> PrevInputActors;


	void WidgetInit(std::shared_ptr<UWidget> _Widget, std::string_view _Name);

};

