#pragma once
#include <EngineBase/TransformObject.h>
#include "RenderUnit.h"
#include "TickObject.h"
#include "EngineStruct.h"

// ���⼭ ��� �Ұųİ� �߿��ϴ�.

// ���� :

// �ڱ� �ڽĿ�����Ʈ�� �̷��͵��� �߰��Ҽ� �ֽ��ϴ�.
// �θ��ڽ� ���踦 �̸��� �־�� �Ѵ�.

// ȭ�鿡 UIó�� �����Ǿ ���̴� �ֵ��� �������ϴ� ����̴�.
// �Ϲ������� �̷� UI���� ������Ʈ���� ������ ��ũ����ǥ�踦 �������� ��´�.
// �׷��� �츮�� �׳� �Ϲ� ����ó�� �ϰڽ��ϴ�.
class ULevel;
class UWidget : 
	public UTransformObject, 
	public URenderUnit, 
	public UWorldObject, 
	public UTickObject, 
	public UNameObject,
	public std::enable_shared_from_this<UWidget>
{
public:
	GENERATED_BODY(URenderUnit)

	friend AActor;
	friend UWidget;
	friend ULevel;
	// constrcuter destructer
	UWidget();
	virtual ~UWidget();

	// delete Function
	UWidget(const UWidget& _Other) = delete;
	UWidget(UWidget&& _Other) noexcept = delete;
	UWidget& operator=(const UWidget& _Other) = delete;
	UWidget& operator=(UWidget&& _Other) noexcept = delete;

	template<typename EnumType>
	void AddToViewPort(EnumType _Order)
	{
		AddToViewPort(static_cast<int>(_Order));
	}

	void AddToViewPort(int _Order);

	void SetHover(std::function<void()> _Hover)
	{
		Hover = _Hover;
	}

	void SetDown(std::function<void()> _Down)
	{
		Down = _Down;
	}

	void SetUp(std::function<void()> _Up)
	{
		Up = _Up;
	}

	void SetPress(std::function<void()> _Press)
	{
		Press = _Press;
	}

	void SetUnHover(std::function<void()> _UnHover)
	{
		UnHover = _UnHover;
	}

	template<typename WidgetType>
	WidgetType* CreateWidget(ULevel* _Level, std::string_view _Name)
	{
		std::shared_ptr<UWidget> NewWidget = std::make_shared<WidgetType>();

		WidgetInit(NewWidget, _Name);

		return dynamic_cast<WidgetType*>(NewWidget.get());
	}

	void SetupAttachment(UWidget* _Parent);

	bool Render(float _DeltaTime) override;

	FVector GetWidgetLocation();
	FVector GetWidgetScale3D();

	void SetWidgetScale3D(FVector _Value);
	void SetWidgetRotation(FVector _Value);
	void SetWidgetLocation(FVector _Value);

	void AddWidgetScale3D(FVector _Value);
	void AddWidgetRotation(FVector _Value);
	void AddWidgetLocation(FVector _Value);

protected:
	void MaterialSettingEnd() override;
	void Tick(float _DeltaTime) override;

	void Reset()
	{
		IsHover = false;
	}

private:
	void RenderingTransformUpdate(std::shared_ptr<UCamera> _Camera);
	void WidgetInit(std::shared_ptr<UWidget>, std::string_view _Name);

	bool IsHover = false;
	UWidget* WidgetParent = nullptr;
	std::list<std::shared_ptr<UWidget>> ChildWidgets;

	std::function<void()> UnHover;
	std::function<void()> Hover;
	std::function<void()> Down;
	std::function<void()> Press;
	std::function<void()> Up;
};

