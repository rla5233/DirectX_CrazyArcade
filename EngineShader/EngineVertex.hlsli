// �� FVector�� �϶� ��� float4�� �����ߴ���
// hlsl���� float4���� ������ �ڷ����� �̸��� float4
// �״�� ���̴����� ����Ҽ� �ִ� ���ؽ� ����ü�� �˴ϴ�.

struct FEngineVertex
{
    // ���� ������ ��� �����ߴ����� �˷���� �������� �����մϴ�.
    float4 POSITION : POSITION;
    float4 TEXCOORD : TEXCOORD;
};