// ���̴��� �⺻������
// hlsl�̶�� ���� �ٸ� �� ����մϴ�.
// C++�� �����ٸ� �״� ������� ������
// ������ ������ ����Ҽ� �ֽ��ϴ�.
// ������ ������ ���������� ���Ŀ� ����ؼ� �ۼ������ �մϴ�.

// �Լ��� �̸��� ������� �ص� �˴ϴ�.
// �׷��� ��� ��Ģ�� ����� ��������
// �� ��Ģ�� ȸ�縶�� �ٸ���
// �����е��� ���ʿ� �����̳� �׷��Ƚ� ���α׷��ӷ� ���ٸ�
// �� ���̴� ���ʿ��� ���ؽ��� _VS
// �� ���̴� ���ʿ��� �ȼ��̸� _PS
// ��Ģ�� ��������ϴ�. <= ȸ�縶�� �Ź� �޶����� �ִ�.

#include "EngineShaderBase.hlsli"

// ��ǲ���̾ƿ��� ����
// ��ǲ���̾ƿ��� 2������ ������ �մϴ�.
// ���� ���� ���ؽ� ���۰� Ư�� �ø�ƽ ������ �ִٴ°��� ������ ����ִ� �뵵.
// 2��°�� � ���ؽ� ���̴��� � �ø�ƽ�� �� �ִ����� ���� ������ ����մϴ�.


// ������ ����ü�� ������ �Ѵ�.
struct ImageVSOutPut
{
    float4 POSITION : SV_POSITION;
    float4 TEXCOORD : TEXCOORD;
};

// ���� ���⿡�ٰ� ��Ʈ��Ʈ�� �ִ´ٰ� �̰� ���̴����� 
// CPU�ʿ��� �־��ֱ�� �� �����ͷ� �������� �ʾƿ�
// CPU���� ���� ����̳� ������ ����� ����
// �־��ְ� �ʹٸ�
// ������۶�� �ϴ� �������̽��� ���� ������ �̿��ؾ߸�
// ���̷�Ʈ�� �־��ټ��� �ֽ��ϴ�.
// hlsl���� struct �̷� �����Ͱ� �����ž�. ��� ���Ǹ� �����ټ� �ְ�


//struct FTransform
//{
//    float4 Scale;
//    float4 Rotation;
//    float4 Position;
//    float4x4 ScaleMat;
//    float4x4 RotationMat;
//    float4x4 PositionMat;
//    float4x4 World;
//    float4x4 View;
//    float4x4 Projection;
//    float4x4 WVP;
//};

// ���ؽ� ���̴��� �־��ټ� ��

cbuffer FCuttingData : register(b2)
{
    //       0, 0
    float4 CuttingPosition;
    //      0.5 0.5
    float4 CuttingSize;
    float4x4 PivotMat;
};

cbuffer FVertexUV : register(b3)
{
    //       0, 0
    float4 PlusUV;
};


struct ImagePSOutPut
{
    float4 COLOR : SV_Target0;
};


ImageVSOutPut WidgetImageShader_VS(FEngineVertex _Input)
{
    ImageVSOutPut Out = (ImageVSOutPut) 0;
    
    // World.
    
    float4x4 PivotWorld = mul(World, PivotMat);
    // float4x4 PivotWorld = World;
    
    Out.POSITION = mul(_Input.POSITION, PivotWorld);
    Out.POSITION = mul(Out.POSITION, View);
    Out.POSITION = mul(Out.POSITION, Projection);
    
    Out.TEXCOORD.x = (_Input.TEXCOORD.x * CuttingSize.x) + CuttingPosition.x;
    Out.TEXCOORD.y = (_Input.TEXCOORD.y * CuttingSize.y) + CuttingPosition.y;
    
    Out.TEXCOORD.x += CuttingSize.x * PlusUV.x;
    Out.TEXCOORD.y += CuttingSize.y * PlusUV.y;
    
    
    
    // 00,    1. 0
    
    
    // 01,   1 1
    
       // Rect�� �����ϴ� �༮�̴�.
    // 0.5 0.5,    1. 0.5
    
    
    // 0.5 1,    1 1

    return Out;
}

// �ؽ�ó�� ������ۿ� ������ �������� �ʽ��ϴ�.
// b0 buffer 0�� ����
// t0 texture 0�� ����
// s0 Sampler 0�� ����

// �𸮾� �����̳�
// ����Ƽ�� OpenGL�ε� ���ΰ�
// �׷��� �ڱ�鸸�� ���̴� �� �� ����ϴ�.
// �𸮾� ���̴� ������.
// HLSL => OpenGL shader ���� �����ϴ� ��ɵ� �����մϴ�.







TextureSet(Image, 0)
//Texture2D Image : register(t0); 
//SamplerState Image_Sampler : register(s0);

cbuffer ResultColorValue : register(b10)
{
    float4 PlusColor;
    float4 MulColor;
    float4 AlphaColor;
};


ImagePSOutPut WidgetImageShader_PS(ImageVSOutPut _Input)
{
        // �� ��ﶧ�� �� �ȵ� ����� �ʺ����� ������ �׸��ΰ� ������Ѵ�.
        // �׳� ����üó�� �ʱ�ȭ �ϴ°� �ȵǴµ�.
    ImagePSOutPut Out = (ImagePSOutPut) 0;
    
    // Name##.Sample(##Name##_Sampler, TEXCOORD.xy);
    
    // Rect�� �����ϴ� �༮�̴�.
    // 00,    10
    
    
    // 01,    11
    Out.COLOR = Sampling(Image, _Input.TEXCOORD);
    Out.COLOR.xyz += PlusColor.xyz;
    Out.COLOR.xyzw *= MulColor.xyzw;
    // #define Sampling(Name, TEXCOORD) Name##.Sample(##Name##_Sampler, TEXCOORD.xy);
    // Image.Sample(Image_Sampler, _Input.TEXCOORD.xy);
    
    return Out;
}
