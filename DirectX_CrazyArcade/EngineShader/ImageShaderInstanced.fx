#include "EngineShaderBase.hlsli"

// ���̷�Ʈ x���� 
struct FEngineVertexInstancing
{
    float4 POSITION : POSITION;
    float4 TEXCOORD : TEXCOORD;
    // �ڵ����� �ִ� ��� ���̾ƿ��� �����ʿ䰡 �����.
    uint Index : SV_InstanceID; // �ν��Ͻ����� �׷����� �ֵ��߿� ���° �׷����� ������ ��� �ִ�.
};

struct ImageVSOutPut
{
    float4 POSITION : SV_POSITION;
    float4 TEXCOORD : TEXCOORD;
    int Index : SV_InstanceID;
};

struct FCuttingData 
{
    float4 CuttingPosition;
    float4 CuttingSize;
    float4x4 PivotMat;
};

struct ImagePSOutPut
{
    float4 COLOR : SV_Target0;
};

// ��Ʈ��ó�� ���۴� �ؽ�ó ���۸� ����մϴ�.
// �ѹ��� 1000���� �׸��Ÿ� 1000���� transform�� ���� ���ο� �� �־�� �Ѵ�
StructuredBuffer<FTransform> TransInstancing : register(t1);

// �ִϸ��̼��̳� �̷��� ��� �Ұų�.
StructuredBuffer<FCuttingData> CuttingDataInstancing : register(t2);

// �ѹ��� n���� �׸��� �ִ� ������ ������ �ϴµ�.
// 10�� ���ϸ� �Ⱦ��°� ����.

ImageVSOutPut ImageShaderInstanced_VS(FEngineVertexInstancing _Input)
{
    int Index = _Input.Index;
    
    FTransform Trans = TransInstancing[Index];
    FCuttingData Cutting = CuttingDataInstancing[Index];
    
    
    ImageVSOutPut Out = (ImageVSOutPut) 0;
    float4x4 PivotWorld = mul(Trans.World, Cutting.PivotMat);
    Out.POSITION = mul(_Input.POSITION, PivotWorld);
    Out.POSITION = mul(Out.POSITION, Trans.View);
    Out.POSITION = mul(Out.POSITION, Trans.Projection);
    
    Out.TEXCOORD.x = (_Input.TEXCOORD.x * Cutting.CuttingSize.x) + Cutting.CuttingPosition.x;
    Out.TEXCOORD.y = (_Input.TEXCOORD.y * Cutting.CuttingSize.y) + Cutting.CuttingPosition.y;
    Out.Index = Index;
    
    return Out;
}

TextureSet(Image, 0)

struct ResultColorValue 
{
    float4 PlusColor;
    float4 MulColor;
    float4 AlphaColor;
};


// �ִϸ��̼��̳� �̷��� ��� �Ұų�.
StructuredBuffer<ResultColorValue> ResultColorValueInstancing : register(t3);

ImagePSOutPut ImageShaderInstanced_PS(ImageVSOutPut _Input)
{
    int Index = _Input.Index;

    
    ResultColorValue ColorData = ResultColorValueInstancing[Index];
    
    ImagePSOutPut Out = (ImagePSOutPut) 0;
    Out.COLOR = Sampling(Image, _Input.TEXCOORD);
    Out.COLOR.xyz += ColorData.PlusColor.xyz;
    Out.COLOR.xyzw *= ColorData.MulColor.xyzw;
    return Out;
}
