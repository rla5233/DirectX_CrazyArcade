
#define TextureSet(Name, Slot) Texture2D Name : register(t##Slot##); SamplerState Name##_Sampler : register(s##Slot##);

// ##�� ġȯ���Ŀ� �����Ƕ�
#define Sampling(Name, TEXCOORD) Name##.Sample(##Name##_Sampler, ##TEXCOORD##.xy);

// HLSL�� �����ΰ�
// c++�� �������� ���� ġȯ�˴ϴ�.
// #define Sampling(Name) ##Name##Value 
// Sampling(MyEngine) => ##Name##Value ???
// 1. ##Name##Value
// 2. ##MyEngine##Value
// 3. MyEngineValue
// ������ ���µ� NameValue��� ������ �����Ҽ� �����ϴ�.