#include "PreCompile.h"
#include "TextWidget.h"
#include "EngineCore.h"

UTextWidget::UTextWidget() 
{
}

UTextWidget::~UTextWidget() 
{
}

void UTextWidget::SetFont(std::string_view _FontName)
{
	Font = UEngineFont::FindRes(_FontName);

	if (nullptr == Font)
	{
		MsgBoxAssert("�ε����� ���� ��Ʈ�� �����Ϸ��� �߽��ϴ�." + std::string(_FontName));
	}
}

bool UTextWidget::Render(float _DeltaTime)
{
	float4 Pos = GetWorldPosition();

	Pos *= GetWorld()->GetUICamera()->GetView();
	Pos *= GetWorld()->GetUICamera()->GetProjection();
	Pos *= GetWorld()->GetUICamera()->GetViewPort();

	Font->TextDraw(Pos, Text, Scale, Color, Flag);
	GEngine->GetDirectXContext()->GSSetShader(nullptr, nullptr, 0);

	return true;
}