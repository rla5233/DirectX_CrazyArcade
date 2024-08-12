#include "PreCompile.h"
#include "SpriteRenderer.h"
#include "EngineShaderResources.h"
#include "EngineSprite.h"

void USpriteRenderer::SetFrameCallback(std::string_view _AnimationName, int _Index, std::function<void()> _Function)
{
	std::string UpperName = UEngineString::ToUpper(_AnimationName);

	if (false == Animations.contains(UpperName))
	{
		MsgBoxAssert("�������� �ʴ� �ִϸ��̼ǿ� �ݹ��� �����Ҽ� �����ϴ�." + std::string(_AnimationName));
		return;
	}

	Animations[UpperName]->FrameCallback[_Index] = _Function;

}

void USpriteRenderer::SetLastFrameCallback(std::string_view _AnimationName, std::function<void()> _Function)
{
	std::string UpperName = UEngineString::ToUpper(_AnimationName);

	if (false == Animations.contains(UpperName))
	{
		MsgBoxAssert("�������� �ʴ� �ִϸ��̼ǿ� �ݹ��� �����Ҽ� �����ϴ�." + std::string(_AnimationName));
		return;
	}

	std::shared_ptr<USpriteAnimation> Animation = Animations[UpperName];
	int LastIndex = static_cast<int>(Animation->Frame.size()) - 1;
	Animations[UpperName]->FrameCallback[LastIndex] = _Function;
}

void USpriteAnimation::FrameCallBackCheck()
{
	if (false == FrameCallback.contains(CurFrame))
	{
		return;
	}

	FrameCallback[CurFrame]();
}

void USpriteAnimation::Update(float _DeltaTime)
{
	IsEnd = false;

	CurTime += _DeltaTime;

	if (CurTime > Inter[CurFrame])
	{
		CurTime -= Inter[CurFrame];
		++CurFrame;
		FrameCallBackCheck();

		if (Frame.size() <= CurFrame)
		{
			if (true == Loop)
			{
				IsEnd = true;
				CurFrame = 0;
			}
			else 
			{
				IsEnd = true;
				--CurFrame;
			}
		}
	}
}

USpriteRenderer::USpriteRenderer() 
{
	SetMesh("Rect");
	SetMaterial("2DImage");
}


USpriteRenderer::~USpriteRenderer() 
{
}

void USpriteRenderer::SetAutoSize(float _ScaleRatio, bool _AutoSize)
{
	AutoSize = _AutoSize;
	ScaleRatio = _ScaleRatio;

	if (true == AutoSize && nullptr != CurInfo.Texture)
	{
		SetSpriteInfo(CurInfo);
	}
}

void USpriteRenderer::MaterialSettingEnd()
{
	Super::MaterialSettingEnd();
	Resources->SettingTexture("Image", "EngineBaseTexture.png", "POINT");
	CurTexture = nullptr;
	Resources->SettingConstantBuffer("ResultColorValue", ColorData);
	Resources->SettingConstantBuffer("FCuttingData", CuttingDataValue);
	Resources->SettingConstantBuffer("FVertexUV", VertexUVValue);
}


void USpriteRenderer::Tick(float _DeltaTime) 
{
	Super::Tick(_DeltaTime);


	if (nullptr != CurAnimation)
	{
		CurAnimation->Update(_DeltaTime);

		FSpriteInfo Info = CurAnimation->GetCurSpriteInfo();
		SetSpriteInfo(Info);
	}
}

void USpriteRenderer::SetDir(EEngineDir _Dir)
{
	Dir = _Dir;

	if (nullptr != CurInfo.Texture)
	{
		SetSpriteInfo(CurInfo);
	}
}

void USpriteRenderer::SetSpriteInfo(const FSpriteInfo& _Info)
{
	CuttingDataValue.CuttingPosition = _Info.CuttingPosition;
	CuttingDataValue.CuttingSize = _Info.CuttingSize;
	CurTexture = _Info.Texture;

	if (true == AutoSize)
	{
		// ���� UV���
		// 0~1������ ���� ���̴�.
		float4 TexScale = _Info.Texture->GetScale();
		Transform.SetScale(TexScale * CuttingDataValue.CuttingSize * ScaleRatio);
	}

	switch (Pivot)
	{
	case EPivot::BOT:
	{
		float4 Scale = Transform.WorldScale;
		Scale.X = 0.0f;
		Scale.Y = abs(Scale.Y) * 0.5f;
		Scale.Z = 0.0f;
		CuttingDataValue.PivotMat.Position(Scale);
		break;
	}
	case EPivot::RIGHT:
	{
		float4 Scale = Transform.WorldScale;
		Scale.X = -abs(Scale.X) * 0.5f;
		Scale.Y = 0.0f;
		Scale.Z = 0.0f;
		CuttingDataValue.PivotMat.Position(Scale);
		break;
	}
	case EPivot::LEFTTOP:
	{
		float4 Scale = Transform.WorldScale;
		Scale.X = abs(Scale.Y) * 0.5f;
		Scale.Y = -abs(Scale.X) * 0.5f;
		Scale.Z = 0.0f;
		CuttingDataValue.PivotMat.Position(Scale);
		break;
	}
	case EPivot::LEFTBOTTOM:
	{
		float4 Scale = Transform.WorldScale;
		Scale.X = abs(Scale.X) * 0.5f;
		Scale.Y = abs(Scale.Y) * 0.5f;
		Scale.Z = 0.0f;
		CuttingDataValue.PivotMat.Position(Scale);
		break;
	}
	case EPivot::RIGHTBOTTOM:
	{
		float4 Scale = Transform.WorldScale;
		Scale.X = -abs(Scale.X) * 0.5f;
		Scale.Y = abs(Scale.Y) * 0.5f;
		Scale.Z = 0.0f;
		CuttingDataValue.PivotMat.Position(Scale);
		break;
	}
	case EPivot::RIGHTTOP:
	{
		float4 Scale = Transform.WorldScale;
		Scale.X = -abs(Scale.X) * 0.5f;
		Scale.Y = -abs(Scale.Y) * 0.5f;
		Scale.Z = 0.0f;
		CuttingDataValue.PivotMat.Position(Scale);
		break;
	}
	case EPivot::MAX:
	default:
	{
		CuttingDataValue.PivotMat.Identity();
	}
		break;
	}

	if (Dir != EEngineDir::MAX)
	{
		float4 Scale = Transform.GetScale();

		switch (Dir)
		{
		case EEngineDir::Left:
		{
			if (0 < Scale.X)
			{
				Scale.X = -Scale.X;
			}
			break;
		}
		case EEngineDir::Right:
		{
			if (0 > Scale.X)
			{
				Scale.X = -Scale.X;
			}
			break;
		}
		case EEngineDir::Up:
		{
			if (0 > Scale.Y)
			{
				Scale.Y = -Scale.Y;
			}
			break;
		}
		case EEngineDir::Down:
		{
			if (0 < Scale.Y)
			{
				Scale.Y = -Scale.Y;
			}
			break;
		}
		case EEngineDir::MAX:
		default:
			break;
		}

		Transform.SetScale(Scale);
	}

	CurInfo = _Info;

	// CuttingDataValue.PivotMat.Position({ 0.0f,100.0f, 0.0f });
	// Transform.World * CuttingDataValue.PivotMat;


	Resources->SettingTexture("Image", _Info.Texture, "POINT");
	SetSamplering(SamplingValue);
}

void USpriteRenderer::SetSprite(std::string_view _Name, UINT _Index/* = 0*/)
{
	std::shared_ptr<UEngineSprite> Sprite = UEngineSprite::FindRes(_Name);

	if (nullptr == Sprite)
	{
		MsgBoxAssert("�������� �ʴ� ��������Ʈ�� �������ַ��� �߽��ϴ�.");
		return;
	}

	FSpriteInfo Info = Sprite->GetSpriteInfo(_Index);
	SetSpriteInfo(Info);
}

void USpriteRenderer::SetSamplering(ETextureSampling _Value)
{
	if (nullptr == CurTexture)
	{
		MsgBoxAssert("�ؽ�ó�� �������� ���� ���¿��� ���ø����� �ٲܼ��� �����ϴ�.");
		return;
	}

	switch (_Value)
	{
	case ETextureSampling::NONE:
		break;
	case ETextureSampling::LINEAR:
	{
		Resources->SettingTexture("Image", CurTexture, "LINEAR");
		break;
	}
	case ETextureSampling::POINT:
	{
		Resources->SettingTexture("Image", CurTexture, "POINT");
		break;
	}
	case ETextureSampling::LINEARCLAMP:
	{
		Resources->SettingTexture("Image", CurTexture, "LINEARCLAMP");
		break;
	}
	case ETextureSampling::POINTCLAMP:
	{
		Resources->SettingTexture("Image", CurTexture, "POINTCLAMP");
		break;
	}
	default:
		break;
	}
}

void USpriteRenderer::CreateAnimation(
	std::string_view _AnimationName, 
	std::string_view _SpriteName, 
	float _Inter, 
	bool _Loop /*= true*/, 
	int _Start /*= -1*/, 
	int _End /*= -1*/)
{
	std::shared_ptr<UEngineSprite> FindSprite = UEngineSprite::FindRes(_SpriteName);

	if (nullptr == FindSprite)
	{
		MsgBoxAssert("�������� �ʴ� ��������Ʈ�� �ִϸ��̼��� ������� �����ϴ�.");
		return;
	}

	std::vector<int> Frame;
	std::vector<float> Inter; 

	int Start = _Start;
	int End = _End;

	if (0 > _Start)
	{
		Start = 0;
	}

	if (0 > End)
	{
		End = static_cast<int>(FindSprite->GetInfoSize()) - 1;
	}

	if (End < Start)
	{
		//MsgBoxAssert("���� ������ ����� �������� �ʽ��ϴ�.");
		for (int i = Start; End <= i; i--)
		{
			Inter.push_back(_Inter);
			Frame.push_back(i);
		}

		CreateAnimation(_AnimationName, _SpriteName, Inter, Frame, _Loop);
		return;
	}



	for (int i = Start; i < End + 1; i++)
	{
		Inter.push_back(_Inter);
		Frame.push_back(i);
	}

	CreateAnimation(_AnimationName, _SpriteName, Inter, Frame, _Loop);
}

void USpriteRenderer::ChangeAnimation(std::string_view _AnimationName, int StartFrame)
{
	std::string ChangeAnimaionName = UEngineString::ToUpper(_AnimationName);
	std::string CurAnimaionName;

	if (nullptr != CurAnimation) {
		CurAnimaionName = CurAnimation->GetName();
		CurAnimaionName = UEngineString::ToUpper(CurAnimaionName);
	}

	if (CurAnimaionName == ChangeAnimaionName)
	{
		return;
	}

	std::string UpperName = UEngineString::ToUpper(_AnimationName);

	if (false == Animations.contains(UpperName))
	{
		MsgBoxAssert("�������� �ʴ� �ִϸ��̼����� ü���� �Ҽ� �����ϴ�." + std::string(_AnimationName));
		return;
	}

	CurAnimation = Animations[UpperName];
	CurAnimation->Reset();
	CurAnimation->CurFrame = StartFrame;

	CurAnimation->FrameCallBackCheck();
}

void USpriteRenderer::CreateAnimation(std::string_view _AnimationName, std::string_view _SpriteName, std::vector<float> _Inter, std::vector<int> _Frame, bool _Loop /*= true*/)
{
	std::string UpperName = UEngineString::ToUpper(_AnimationName);

	if (true == Animations.contains(UpperName))
	{
		MsgBoxAssert("�̹� �����ϴ� �̸��� �ִϸ��̼��� ����� �����ϴ�.");
		return;
	}

	 std::shared_ptr<UEngineSprite> FindSprite = UEngineSprite::FindRes(_SpriteName);

	if (nullptr == FindSprite)
	{
		MsgBoxAssert("�������� �ʴ� ��������Ʈ�� �ִϸ��̼��� ������� �����ϴ�.");
		return;
	}

	std::shared_ptr<USpriteAnimation> NewAnimation = std::make_shared<USpriteAnimation>();

	NewAnimation->Sprite = FindSprite;
	NewAnimation->Inter = _Inter;
	NewAnimation->Frame = _Frame;
	NewAnimation->Loop = _Loop;
	NewAnimation->SetName(_AnimationName);
	NewAnimation->Reset();

	Animations[UpperName] = NewAnimation;
}

bool USpriteRenderer::IsCurAnimationEnd()
{
	return CurAnimation->IsEnd;
}