#include "PreCompile.h"
#include "Level.h"
#include "GameMode.h"
#include "Renderer.h"
#include "Collision.h"
#include "Camera.h"
#include "EngineCore.h"
#include "EngineRenderTarget.h"
#include "EngineGraphicDevice.h"
#include "InstancingRender.h"
#include "Widget.h"
#include <EngineBase/NetObject.h>

bool ULevel::IsActorConstructer = true;

ULevel::ULevel() 
{
	// MainCamera = std::make_shared<UCamera>();

	MainCamera = SpawnActor<UCamera>("MainCamera");
	UICamera = SpawnActor<UCamera>("NewActor");
	UICamera->SetActorLocation(FVector(0.0f, 0.0f, -100.0f));
	UICamera->InputOff();

	LastTarget = UEngineRenderTarget::Create();
	// ���� �ٶ󺸴� �ֵ��� ��Ƽ� �׸��� �ִ� ����Ÿ���� ����� �;�.
	float4 Scale = GEngine->EngineWindow.GetWindowScale();
	LastTarget->CreateTexture(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, Scale, float4::Zero);
}

ULevel::~ULevel() 
{
	MainCamera = nullptr;
	UICamera = nullptr;
	GameMode = nullptr;
	Actors.clear();
	Renderers.clear();
	Collisions.clear();
	Widgets.clear();

}

void ULevel::Tick(float _DeltaTime)
{
	{
		std::lock_guard<std::mutex> Lock(FunctionLock);
		for (std::function<void()> Function : Functions)
		{
			Function();
		}
		Functions.clear();
	}

	// UNetObject::UpdateProtocol();

	// 1 �÷��̾� �׷� [][]
	// 2 ���� �׷� [][]

	Super::Tick(_DeltaTime);
	for (std::pair<const int, std::list<std::shared_ptr<AActor>>>& TickGroup : Actors)
	{
		if (false == GEngine->TimeScales.contains(TickGroup.first))
		{
			GEngine->TimeScales[TickGroup.first] = 1.0f;
		}

		float TimeScale = GEngine->TimeScales[TickGroup.first];

		std::list<std::shared_ptr<AActor>>& GroupActors = TickGroup.second;

		for (std::shared_ptr<AActor> Actor : GroupActors)
		{
			if (false == Actor->IsActive())
			{
				continue;
			}

			Actor->Tick(_DeltaTime * TimeScale);
		}
	}
}

void ULevel::Render(float _DeltaTime)
{
	MainCamera->ViewPortSetting();

	GEngine->GetEngineDevice().BackBufferRenderTarget->Setting();
	
	MainCamera->CameraTarget->Clear();
	MainCamera->CameraTarget->Setting();
	MainCamera->CameraTransformUpdate();

	for (std::pair<const int, std::list<std::shared_ptr<URenderer>>>& RenderGroup : Renderers)
	{
		std::list<std::shared_ptr<URenderer>>& GroupRenderers = RenderGroup.second;

		if (true == GroupRenderers.empty())
		{
			continue;
		}

		if (true == InstancingRenders.contains(RenderGroup.first))
		{
			std::shared_ptr<UInstancingRender> Inst = InstancingRenders[RenderGroup.first];
			// ù��° ������ ������ �ڿ� �ִ� ���ֵ��� ���� �ؽ�ó
			// ���� ���÷�
			// ���� ��� ���� ���� �������̴�.
			std::shared_ptr<URenderUnit> Unit = GroupRenderers.front();
			Inst->Resources->OtherResCopy(Unit->Resources);

			int Count = 0;
			for (std::shared_ptr<URenderer> Renderer : GroupRenderers)
			{
				// ���ʹ� �����ϴ°� �߿����� 
				// �׸��ų� �浹�Ҷ��� �����̱� ������
				if (nullptr == Renderer->GetActor()->RootComponent)
				{
					MsgBoxAssert("��Ʈ������Ʈ�� �������� ���� ���Ͱ� �ֽ��ϴ�" + Renderer->GetActor()->GetName());
					continue;
				}

				if (false == Renderer->IsActive())
				{
					continue;
				}
				Renderer->RenderingTransformUpdate(MainCamera);
				Renderer->Update(_DeltaTime);
				Inst->InstancingDataCheck(Renderer.get(), Count++);
			}

			Inst->RenderInstancing(_DeltaTime, static_cast<int>(RenderGroup.second.size()));
			continue;
		}

		for (std::shared_ptr<URenderer> Renderer : GroupRenderers)
		{
			// ���ʹ� �����ϴ°� �߿����� 
			// �׸��ų� �浹�Ҷ��� �����̱� ������
			if (nullptr == Renderer->GetActor()->RootComponent)
			{
				MsgBoxAssert("��Ʈ������Ʈ�� �������� ���� ���Ͱ� �ֽ��ϴ�" + Renderer->GetActor()->GetName());
				continue;
			}


			if (false == Renderer->IsActive())
			{
				continue;
			}

			Renderer->RenderingTransformUpdate(MainCamera);
			Renderer->Update(_DeltaTime);
			if (false == Renderer->Render(_DeltaTime))
			{
				MsgBoxAssert("�������� �����߽��ϴ�." + Renderer->GetName());
			}
		}
	}

	MainCamera->CameraTarget->Effect(_DeltaTime);

	// ��� �Ϲݿ�����Ʈ���� �������� �ϰ�

	// �𸮾��� ������ ����.
	UICamera->CameraTarget->Clear();
	UICamera->CameraTarget->Setting();
	UICamera->CameraTransformUpdate();

	WidgetInits.clear();

	for (std::pair<const int, std::list<std::shared_ptr<UWidget>>>& WidgetGroup : Widgets)
	{
		std::list<std::shared_ptr<UWidget>>& GroupRenderers = WidgetGroup.second;

		for (std::shared_ptr<UWidget> Widget : GroupRenderers)
		{
			// ���ʹ� �����ϴ°� �߿����� 
			if (false == Widget->IsActive())
			{
				continue;
			}

			Widget->Tick(_DeltaTime);
			Widget->RenderingTransformUpdate(UICamera);
			if (false == Widget->Render(_DeltaTime))
			{
				MsgBoxAssert("�������� �����߽��ϴ�." + Widget->GetName());
			}
		}
	}

	UICamera->CameraTarget->Effect(_DeltaTime);

	UEngineGraphicDevice& Device = GEngine->GetEngineDevice();

	// ����۴� �׳� ������ ����� ���ؼ� �ѹ��� �����Ǵ°� �´�.

	// 1-r g b a

	LastTarget->Clear();
	LastTarget->Merge(MainCamera->CameraTarget);
	LastTarget->Merge(UICamera->CameraTarget);
	LastTarget->Effect(_DeltaTime);

	// ���� �̼���.
	// ����Ʈ ����Ʈ
	// ȿ���� �ش�.
	// LastTarget->AddEffect<UBlurEffect>();

	Device.BackBufferRenderTarget->Copy(LastTarget);
}

void ULevel::Destroy()
{

	for (std::pair<const int, std::list<std::shared_ptr<UCollision>>>& Group : Collisions)
	{
		std::list<std::shared_ptr<UCollision>>& GroupCollisions = Group.second;

		std::list<std::shared_ptr<UCollision>>::iterator StartIter = GroupCollisions.begin();
		std::list<std::shared_ptr<UCollision>>::iterator EndIter = GroupCollisions.end();


		for (; StartIter != EndIter; )
		{
			std::shared_ptr<UCollision> CurCollision = *StartIter;

			if (false == CurCollision->GetActor()->IsDestroy())
			{
				++StartIter;
				continue;
			}

			StartIter = GroupCollisions.erase(StartIter);
		}
	}

	for (std::pair<const int, std::list<std::shared_ptr<URenderer>>>& RenderGroup : Renderers)
	{
		std::list<std::shared_ptr<URenderer>>& GroupRenderers = RenderGroup.second;

		std::list<std::shared_ptr<URenderer>>::iterator StartIter = GroupRenderers.begin();
		std::list<std::shared_ptr<URenderer>>::iterator EndIter = GroupRenderers.end();


		for (; StartIter != EndIter; )
		{
			std::shared_ptr<URenderer> CurRenderer = *StartIter;

			if (false == CurRenderer->GetActor()->IsDestroy())
			{
				++StartIter;
				continue;
			}

			StartIter = GroupRenderers.erase(StartIter);
		}
	}

	for (std::pair<const int, std::list<std::shared_ptr<AActor>>>& TickGroup : Actors)
	{
		std::list<std::shared_ptr<AActor>>& GroupActors = TickGroup.second;

		std::list<std::shared_ptr<AActor>>::iterator StartIter = GroupActors.begin();
		std::list<std::shared_ptr<AActor>>::iterator EndIter = GroupActors.end();

		for ( ; StartIter != EndIter; )
		{
			std::shared_ptr<AActor> CurActor = *StartIter;

			if (false == CurActor->IsDestroy())
			{
				++StartIter;
				continue;
			}

			CurActor->End();
			StartIter = GroupActors.erase(StartIter);
		}
	}

}

void ULevel::PushActor(std::shared_ptr<AActor> _Actor)
{
	if (nullptr == _Actor)
	{
		MsgBoxAssert("������ ���� ���͸� �߰��Ϸ��� �߽��ϴ�.");
		return;
	}
	
	_Actor->SetWorld(this);
	_Actor->BeginPlay();

	Actors[_Actor->GetOrder()].push_back(_Actor);
}

void ULevel::ConstructorActor(std::shared_ptr<AActor> _Actor, std::string_view _Name, int _Order)
{
	_Actor->SetName(_Name);
	_Actor->SetOrder(_Order);
}

void ULevel::PushRenderer(std::shared_ptr<URenderer> _Renderer)
{
	Renderers[_Renderer->GetOrder()].push_front(_Renderer);
}

void ULevel::PushCollision(std::shared_ptr<UCollision> _Collision)
{
	Collisions[_Collision->GetOrder()].push_front(_Collision);
}

void ULevel::ChangeOrderRenderer(std::shared_ptr<URenderer> _Renderer, int _PrevOrder, int _ChangeOrder)
{
	// ���������� 
	Renderers[_PrevOrder].remove(_Renderer);

	Renderers[_ChangeOrder].push_front(_Renderer);
}

void ULevel::ChangeOrderCollision(std::shared_ptr<UCollision> _Collision, int _PrevOrder, int _ChangeOrder)
{
	// ���������� 
	Collisions[_PrevOrder].remove(_Collision);

	Collisions[_ChangeOrder].push_front(_Collision);
}


void ULevel::LevelEnd(ULevel* _NextLevel)
{
	Super::LevelStart(_NextLevel);
	
	Functions.clear();
	for (std::pair<const int, std::list<std::shared_ptr<AActor>>>& TickGroup : Actors)
	{
		std::list<std::shared_ptr<AActor>>& GroupActors = TickGroup.second;

		for (std::shared_ptr<AActor> Actor : GroupActors)
		{
			Actor->LevelEnd(_NextLevel);
		}
	}
}
void ULevel::LevelStart(ULevel* _PrevLevel)
{
	Super::LevelEnd(_PrevLevel);
	
	Functions.clear();
	for (std::pair<const int, std::list<std::shared_ptr<AActor>>>& TickGroup : Actors)
	{
		std::list<std::shared_ptr<AActor>>& GroupActors = TickGroup.second;

		for (std::shared_ptr<AActor> Actor : GroupActors)
		{
			Actor->LevelStart(_PrevLevel);
		}
	}
}

void ULevel::PushWidget(std::shared_ptr<UWidget> _Widget)
{
	if (nullptr == _Widget)
	{
		MsgBoxAssert("������ ���� ������ �߰��Ϸ��� �߽��ϴ�.");
		return;
	}

	_Widget->SetWorld(this);

	WidgetInits.remove(_Widget);
	Widgets[_Widget->GetOrder()].push_back(_Widget);
}


