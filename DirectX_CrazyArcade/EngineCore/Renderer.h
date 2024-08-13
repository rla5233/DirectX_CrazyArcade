#pragma once
#include "SceneComponent.h"
#include "EngineMesh.h"
#include "EngineMaterial.h"
#include "RenderUnit.h"

// ���� : public std::enable_shared_from_this<URenderer>
// shared_ptr�� this�� �����Ҽ� �ִ� ����� ���� Ŭ�����Դϴ�.

// URenderer�� ���� ������ �Ҽ� �ִ� ��Ȳ�̴�.
// ��緣������ ���Ҽ� �ִ�.
// �׷��� �ȵȴ�.
// ����� ������� �ʾҰ� Ưȭ����� ������� ����.
// Ưȭ���?

// �������� �����ϰ� �������� ���õ� �ְ� �ƴϴ�.
// ���� ���� ����
class UEngineShaderResources;
class URenderer : public USceneComponent, public std::enable_shared_from_this<URenderer>, public URenderUnit
{
	friend ULevel;
	GENERATED_BODY(USceneComponent)

public:

	// constrcuter destructer
	URenderer();
	~URenderer();

	// delete Function
	URenderer(const URenderer& _Other) = delete;
	URenderer(URenderer&& _Other) noexcept = delete;
	URenderer& operator=(const URenderer& _Other) = delete;
	URenderer& operator=(URenderer&& _Other) noexcept = delete;

	template<typename EnumType>
	void SetOrder(EnumType _Order)
	{
		SetOrder(static_cast<int>(_Order));
	}

	void SetOrder(int _Order) override;


protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void MaterialSettingEnd() override;

private:
	void RenderingTransformUpdate(std::shared_ptr<UCamera> _Camera);
};

