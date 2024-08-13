#pragma once
#include "EngineEnums.h"
#include <EngineBase/Transform.h>
#include "RenderUnit.h"

class UDebugRenderInfo
{
public:
	FTransform Trans;
	float4 Color;
	URenderUnit Unit;
};

class UEngineCore;
class UDebugRenderClass
{
	// �Լ� firend 
	friend UEngineCore;

public:


	static void DebugRender(ULevel* _Level);
};

namespace UEngineDebug
{
	//                   �ڽ���                        ���⿡                   �̻����� �׷���.
	void DrawDebugRender(EDebugRenderType _DebugText, FTransform& _Transform, float4 _Color);
}

