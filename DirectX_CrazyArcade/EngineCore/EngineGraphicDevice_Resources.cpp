#include "PreCompile.h"
#include "EngineCore.h"
#include "EngineGraphicDevice.h"

#include "EngineVertex.h"
#include "EngineMesh.h"
#include "EngineTexture.h"
#include "EngineVertexShader.h"
#include "EnginePixelShader.h"
#include "EngineRasterizer.h"
#include "EngineBlend.h"
#include "EngineMaterial.h"
#include "EngineSprite.h"

#include "EngineRenderTarget.h"

void UEngineGraphicDevice::EngineResourcesRelease()
{
	// ������ �ڵ����� �������� ���ҽ����� �� ���� ���⼭ Ŭ��� ���� ������?
	// ������ ����Ǵ� ������ �ؽ�ó�� ��δ� �����Ѵ�.

	UEngineSound::ResourcesRelease();
	UEngineSprite::ResourcesRelease();
	UEngineRenderTarget::ResourcesRelease();
	UEngineTexture::ResourcesRelease();
	UEngineSampler::ResourcesRelease();
	UEngineVertexShader::ResourcesRelease();
	UEnginePixelShader::ResourcesRelease();

	// Mesh
	UEngineVertexBuffer::ResourcesRelease();
	UEngineIndexBuffer::ResourcesRelease();
	UEngineMesh::ResourcesRelease();

	// ��Ƽ����
	UEngineVertexShader::ResourcesRelease();
	UEnginePixelShader::ResourcesRelease();
	UEngineRasterizer::ResourcesRelease();
	UEngineBlend::ResourcesRelease();


	UEngineMaterial::ResourcesRelease();
}

// ��ǲ����� 1�� ��ǲ����� 2�� ���ҽ����� ������ �̴ϼȶ�����
void MeshInit()
{
	FEngineVertex::Info.AddInputLayOut("POSITION", DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT);
	FEngineVertex::Info.AddInputLayOut("TEXCOORD", DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT);

	{
		std::vector<FEngineVertex> VertexData;
		VertexData.resize(4);

		{
			// 00   10
			// 0    1
			// 01   11
			// 3    2

			VertexData[0] = { {-0.5f, 0.5f, 0.0f, 1.0f} , {0.0f, 0.0f} };
			VertexData[1] = { {0.5f, 0.5f, 0.0f, 1.0f} , {1.0f, 0.0f} };
			VertexData[2] = { {0.5f, -0.5f, 0.0f, 1.0f}, {1.0f, 1.0f} };
			VertexData[3] = { {-0.5f, -0.5f, 0.0f, 1.0f}, {0.0f, 1.0f} };
			std::shared_ptr<UEngineVertexBuffer> VertexBuffer = UEngineVertexBuffer::Create("Rect", VertexData);
		}

		// �ﰢ���� ��� �׸����� ���� ����.
		std::vector<UINT> IndexData = { 0, 1, 2, 0, 2, 3 };

		std::shared_ptr<UEngineIndexBuffer> IndexBuffer = UEngineIndexBuffer::Create("Rect", IndexData);

		UEngineMesh::Create("Rect");

		{
			VertexData[0] = { {-1.0f, 1.0f, 0.0f, 1.0f} , {0.0f, 0.0f} };
			VertexData[1] = { {1.0f, 1.0f, 0.0f, 1.0f} , {1.0f, 0.0f} };
			VertexData[2] = { {1.0f, -1.0f, 0.0f, 1.0f}, {1.0f, 1.0f} };
			VertexData[3] = { {-1.0f, -1.0f, 0.0f, 1.0f}, {0.0f, 1.0f} };
			std::shared_ptr<UEngineVertexBuffer> VertexBuffer = UEngineVertexBuffer::Create("FullRect", VertexData);
		}

		UEngineMesh::Create("FullRect", "FullRect", "Rect");
	}

	{
		FEngineVertex V;
		std::vector<FEngineVertex> VBVector;
		std::vector<UINT> IBVector;

		float Radius = 0.5f;
		// �ϱ������� �����մϴ�.
		V.POSITION = float4(0.0f, Radius, 0.0f, 1.0f);
		V.TEXCOORD = float4(0.5f, 0.0f);
		// �븻 ���� Ȥ�� �������Ͷ�� �Ҹ���
		// �鿡 ������ ���͸� �ǹ��ϰ� �ȴ�.
		// ���� �ݻ��Ҷ� �ʼ�.
		//V.NORMAL = float4(0.0f, Radius, 0.0f, 1.0f);
		//V.NORMAL.Normalize();
		//V.NORMAL.W = 0.0f;
		//V.TANGENT = float4(1.0f, 0.0f, 0.0f, 0.0f);
		//V.BINORMAL = float4(0.0f, 0.0f, 1.0f, 0.0f);

		VBVector.push_back(V);

		UINT iStackCount = 16; // ���� ���� �����Դϴ�.
		UINT iSliceCount = 16; // ���κ��� ����

		float yRotAngle = UEngineMath::PI / (float)iStackCount;
		float zRotAngle = (UEngineMath::PI * 2) / (float)iSliceCount;

		// UV�� ���μ��� ���ݰ��� ���Ѵ�.
		float yUvRatio = 1.0f / (float)iStackCount;
		float zUvRatio = 1.0f / (float)iStackCount;

		for (UINT y = 1; y < iStackCount; ++y)
		{
			// �� ���ݿ� ���� ������
			float phi = y * yRotAngle;
			for (UINT z = 0; z < iSliceCount + 1; ++z)
			{
				float theta = z * zRotAngle;
				V.POSITION = float4{
					Radius * sinf(y * yRotAngle) * cosf(z * zRotAngle),
					Radius * cosf(y * yRotAngle),
					Radius * sinf(y * yRotAngle) * sinf(z * zRotAngle),
					1.0f // ��ġ ũ�� ���� ������ �ֱ� ���ؼ�
				};

				// V.Pos *= GameEngineRandom::RandomFloat(-0.9f, 0.1f);

				V.TEXCOORD = float4(yUvRatio * z, zUvRatio * y);
				//V.NORMAL = V.POSITION.NormalizeReturn();
				//V.NORMAL.W = 0.0f;

				//V.TANGENT.X = -Radius * sinf(phi) * sinf(theta);
				//V.TANGENT.Y = 0.0f;
				//V.TANGENT.Z = Radius * sinf(phi) * cosf(theta);
				//V.TANGENT = V.TANGENT.NormalizeReturn();
				//V.TANGENT.W = 0.0f;

				//V.BINORMAL = float4::Cross3D(V.TANGENT, V.NORMAL);
				//V.BINORMAL = V.BINORMAL.NormalizeReturn();
				//V.BINORMAL.W = 0.0f;


				VBVector.push_back(V);
			}
		}

		// ������
		V.POSITION = float4(0.0f, -Radius, 0.0f, 1.0f);
		V.TEXCOORD = float4(0.5f, 1.0f);
		//V.NORMAL = float4(0.0f, -Radius, 0.0f, 1.0f);
		//V.NORMAL.Normalize();
		//V.NORMAL.W = 0.0f;
		//V.TANGENT = float4(-1.0f, 0.0f, 0.0f, 0.0f);
		//V.BINORMAL = float4(0.0f, 0.0f, -1.0f, 0.0f);
		VBVector.push_back(V);

		// �ε��� ���۸� ����ϴ�.
		IBVector.clear();

		// �ϱ����� �̷�� ���� ����°�.
		for (UINT i = 0; i < iSliceCount; i++)
		{
			// ������ ������ �ϱ���
			IBVector.push_back(0);
			IBVector.push_back(i + 2);
			IBVector.push_back(i + 1);
		}

		for (UINT y = 0; y < iStackCount - 2; y++)
		{
			for (UINT z = 0; z < iSliceCount; z++)
			{
				IBVector.push_back((iSliceCount + 1) * y + z + 1);
				IBVector.push_back((iSliceCount + 1) * (y + 1) + (z + 1) + 1);
				IBVector.push_back((iSliceCount + 1) * (y + 1) + z + 1);

				IBVector.push_back((iSliceCount + 1) * y + z + 1);
				IBVector.push_back((iSliceCount + 1) * y + (z + 1) + 1);
				IBVector.push_back((iSliceCount + 1) * (y + 1) + (z + 1) + 1);

			}
		}

		// ���������� ������ �ε���
		UINT iBotIndex = (UINT)VBVector.size() - 1;
		for (UINT i = 0; i < iSliceCount; i++)
		{
			// ������ ������ �ϱ���
			IBVector.push_back(iBotIndex);
			IBVector.push_back(iBotIndex - (i + 2));
			IBVector.push_back(iBotIndex - (i + 1));
		}

		UEngineVertexBuffer::Create("Sphere", VBVector);
		UEngineIndexBuffer::Create("Sphere", IBVector);

		UEngineMesh::Create("Sphere");
	}



	// ��ǲ���̾ƿ� ����
	// GEngine->GetDirectXDevice()->CreateInputLayout()


}

// �ȼ� �� ���ؽ����̴��� �̴ϼȶ�����
void ShaderInit()
{
	UEngineDirectory Dir;
	Dir.MoveToSearchChild("EngineShader");
	UEngineShader::AutoCompile(Dir);
}

void SettingInit()
{
		//D3D11_FILL_MODE FillMode;
		//D3D11_CULL_MODE CullMode;
		//BOOL FrontCounterClockwise;
		//INT DepthBias;
		//FLOAT DepthBiasClamp;
		//FLOAT SlopeScaledDepthBias;
		//BOOL DepthClipEnable;
		//BOOL ScissorEnable;
		//BOOL MultisampleEnable;
		//BOOL AntialiasedLineEnable;

	{
		D3D11_RASTERIZER_DESC Desc = {};

		// ������ �׷���
		Desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		// ������ �׷���.
		// Desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;

		// �ո��̰� �޸��̰� �� �׷���.
		// �츮 �������� ��
		// �ո� �׸�����

		// Desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
		// �޸� �׸�����
		Desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		// �ո�� �޸�
		// �ð�����̸� �޸����� ���ϴ�.

		Desc.AntialiasedLineEnable = TRUE;
		Desc.DepthClipEnable = TRUE;

		// �����Ͷ����� ����
		UEngineRasterizer::Create("EngineBase", Desc);
	}

	{
		D3D11_RASTERIZER_DESC Desc = {};
		Desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
		Desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		Desc.AntialiasedLineEnable = TRUE;
		Desc.DepthClipEnable = TRUE;
		UEngineRasterizer::Create("Debug", Desc);
	}


	{
		D3D11_SAMPLER_DESC Desc = {};

		// �ɼǹٲٸ鼭 ����帮�ڽ��ϴ�.
		// ����
		Desc.AddressW = Desc.AddressV = Desc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;

		// MIP MAP
		// MIN_MAG_MIP ������ Ŭ���� �������� 
		// MIP ������ ������
		// MAG ������ Ŭ��
		// MIN Ȯ���Ҷ�
		// ������ POINT�� ���ø�����
		// POINT ������ ���� �ʰ� ������ �����ض�.
		// Liner �� ������ �ϰ� �����ض�.
		Desc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT;

		// �Ӹ��� ���信 ���ؼ� �����ؾ��Ѵ�.
		Desc.MipLODBias = 0.0f; // �������� �ʴ´�.
		Desc.MaxAnisotropy = 1;
		// Ư�� ���� �񱳸� ���ؼ� ������ ����ؾ��Ҷ� ���������� ���Ǵ� �ɼ��ε�.
		// �̰� �׳� ������ ������ ���� ����ض�.
		Desc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS;

		Desc.MinLOD = -FLT_MAX;
		Desc.MaxLOD = FLT_MAX;

		// ����

		UEngineSampler::Create("POINT", Desc);
	}

	{
		D3D11_SAMPLER_DESC Desc = {};

		Desc.AddressW = Desc.AddressV = Desc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		Desc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		Desc.MipLODBias = 0.0f; // �������� �ʴ´�.
		Desc.MaxAnisotropy = 1;
		Desc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS;
		Desc.MinLOD = -FLT_MAX;
		Desc.MaxLOD = FLT_MAX;
		UEngineSampler::Create("LINEAR", Desc);
	}


	{
		D3D11_SAMPLER_DESC Desc = {};

		Desc.AddressW = Desc.AddressV = Desc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
		Desc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT;
		Desc.MipLODBias = 0.0f; // �������� �ʴ´�.
		Desc.MaxAnisotropy = 1;
		Desc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS;
		Desc.MinLOD = -FLT_MAX;
		Desc.MaxLOD = FLT_MAX;
		UEngineSampler::Create("POINTCLAMP", Desc);
	}


	{
		D3D11_SAMPLER_DESC Desc = {};

		Desc.AddressW = Desc.AddressV = Desc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
		Desc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		Desc.MipLODBias = 0.0f; // �������� �ʴ´�.
		Desc.MaxAnisotropy = 1;
		Desc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS;
		Desc.MinLOD = -FLT_MAX;
		Desc.MaxLOD = FLT_MAX;
		UEngineSampler::Create("LINEARCLAMP", Desc);
	}



	{
		D3D11_BLEND_DESC Desc = {};

		// true ����ó���� ������ �ɰ��ε�.
		// �̰� ���귮�� ���Ƽ� false�� ó���� �̴ϴ�.
		// directx�� �⺻�������� ó���ϴµ� �ƹ�
		Desc.AlphaToCoverageEnable = false;

		// SV_Target0 0��Ÿ�ٿ� ����ϰڴ�.
		// false == 0���������� ��� ����
		// true == ���� �־��� ���� 0���������� ��� ����
		Desc.IndependentBlendEnable = false;

		// float4 DestColor = 0011;
		// float4 SrcColor = 0000;
		
		// float4 DestFilter = 0011;
		// float4 SrcFilter = 0000;
		// Result = DestColor * DestFilter (+) SrcColor * SrcFilter;

		// D3D11_BLEND_OP_ADD             +
		// D3D11_BLEND_OP_SUBTRACT        -


		Desc.RenderTarget[0].BlendEnable = true;
		Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		// Result = DestColor * DestFilter (+) SrcColor * SrcFilter;
		Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

		// Result = DestColor * DestFilter (+) SrcColor * (SrcFilter);
		// SRCColor = 000(0)
		// 1110 * 0000 = 0000
		// SrcFilter = 0000
		// 
		Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;

		// DestColor = 001(1)
		// SRCColor = 000(1)
		// DestFilter =  1 - 1
		// DestColor *DestFilter ; 
		
		Desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

		Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

		Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;

		Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

		//              D3D11_BLEND_INV_SRC_ALP                      D3D11_BLEND_SRC_ALPHA
		// 0011    *       0000             +      100(0.5) *        1111
		// (0011 * 0. 5 0.5 0.5 0.5 ) + (1001 * 0.5 0.5 0.5 0.5 ) => 1001

		//      R   G    B   A
		//      0   0   0.5  1
		//  +   0.5 0    0   1
		//      0.5 0   0.5  2


		UEngineBlend::Create("EngineBase", Desc);
	}


	{
		D3D11_BLEND_DESC Desc = {};
		Desc.AlphaToCoverageEnable = false;
		Desc.IndependentBlendEnable = false;
		Desc.RenderTarget[0].BlendEnable = true;
		Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		Desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

		UEngineBlend::Create("Overlay", Desc);
	}


}

void MaterialInit()
{

	{
		std::shared_ptr<UEngineMaterial> Mat = UEngineMaterial::Create("2DImage");
		Mat->SetPixelShader("ImageShader.fx");
		Mat->SetVertexShader("ImageShader.fx");
	}

	{
		std::shared_ptr<UEngineMaterial> Mat = UEngineMaterial::Create("Widget2DImage");
		Mat->SetPixelShader("WidgetImageShader.fx");
		Mat->SetVertexShader("WidgetImageShader.fx");
	}


	{
		std::shared_ptr<UEngineMaterial> Mat = UEngineMaterial::Create("2DImageInstancing");
		Mat->SetPixelShader("ImageShaderInstanced.fx");
		Mat->SetVertexShader("ImageShaderInstanced.fx");
	}


	{
		std::shared_ptr<UEngineMaterial> Mat = UEngineMaterial::Create("Debug");
		Mat->SetPixelShader("DebugShader.fx");
		Mat->SetVertexShader("DebugShader.fx");
		Mat->SetRasterizer("Debug");
	}

	{
		std::shared_ptr<UEngineMaterial> Mat = UEngineMaterial::Create("TargetCopy");
		Mat->SetPixelShader("TargetCopyShader.fx");
		Mat->SetVertexShader("TargetCopyShader.fx");
	}

	{
		std::shared_ptr<UEngineMaterial> Mat = UEngineMaterial::Create("Blur");
		Mat->SetPixelShader("BlurEffectShader.fx");
		Mat->SetVertexShader("BlurEffectShader.fx");
	}


}

void EngineTextureInit()
{
	{
		// ������ ���
		UEngineDirectory Dir;
		Dir.MoveToSearchChild("EngineResources");
		std::vector<UEngineFile> Files = Dir.GetAllFile({ ".png" }, true);
		for (UEngineFile& File : Files)
		{
			UEngineTexture::Load(File.GetFullPath());
		}
	}

}

// �������� �ڽ��ϳ��� ���������ָ� 
void UEngineGraphicDevice::EngineResourcesInit()
{
	MeshInit();
	ShaderInit();
	SettingInit();
	MaterialInit();
	EngineTextureInit();

	UEngineRenderTarget::RenderTargetInit();
}