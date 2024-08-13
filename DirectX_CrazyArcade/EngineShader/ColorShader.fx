#include "EngineShaderBase.hlsli"

// �ȼ����̴��� ������� 0���� �޶� �ȴ�.
struct ImageVSOutPut
{
    float4 POSITION : SV_POSITION;
};

ImageVSOutPut ColorShader_VS(FEngineVertex _Input)
{
        // �� ��ﶧ�� �� �ȵ� ����� �ʺ����� ������ �׸��ΰ� ������Ѵ�.
        // �׳� ����üó�� �ʱ�ȭ �ϴ°� �ȵǴµ�.
    ImageVSOutPut Out = (ImageVSOutPut) 0;
    
    Out.POSITION = _Input.POSITION;
    // �ڵ��������Ҷ� 
    // ������۸� ���̴��ȿ��� ������� ������
    // ������۸� ���ٰ� �������� �ʴ´�.
    // ���̴��� �ǹ̾��� �ڵ带 �볳���� �ʽ��ϴ�.
    // float4x4 POS = mul(_Input.POSITION, WVP);
    
    // int a = 1 + 1;
    
    // �������� ����� ������ ��ġ�� �ڵ带 �ľ߸� ���� ������۰� �ִٰ� ���� �մϴ�.
    Out.POSITION = mul(_Input.POSITION, WVP);
   
    
    //Out.POSITION *= World;
    //Out.POSITION *= View;
    //Out.POSITION *= Projection;
    
   // hlsl ������(swizzling) ����.
   // Out.POSITION.xyz = _Input.POSITION.xyz * 2.0f;
   // Out.POSITION.w = 1.0f;
    return Out;
}


// �ȼ����̴��� ������� 0���� �޶� �ȴ�.
cbuffer OutPutColor : register(b0)
{
    float4 Color;
};


struct ImagePSOutPut
{
    float4 COLOR : SV_Target0;
};

ImagePSOutPut ColorShader_PS(ImageVSOutPut _Input)
{
        // �� ��ﶧ�� �� �ȵ� ����� �ʺ����� ������ �׸��ΰ� ������Ѵ�.
        // �׳� ����üó�� �ʱ�ȭ �ϴ°� �ȵǴµ�.
    ImagePSOutPut Out = (ImagePSOutPut) 0;
    // Out.COLOR = Color;
    Out.COLOR = Color;
    
    return Out;
}
