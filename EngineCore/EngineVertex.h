#pragma once
#include <EngineBase/EngineMath.h>
#include "DirectXHelper.h"

// 3D�� �׷����� ���� ��ġ�� �����ʿ䰡 �����ϴ�.
// ����
// ��ġ ����� �� ����

// �̰� ���� ������ ������ �ȴ�.
// �� 1���� �����ϴ� ��ҵ��� ���� ������� ������ �ִ�.
// �װ� directx�� �˾ƿ� �����?
// ���� �������
// �� ���� ��� �־�� �մϱ�?
// ������ġ�߿� ��� �ֳ�?
// �׷���ī�� ���� ���� �˴ϴ�.
// => �׷���ī��� �� �������鷡
//    ũ��

class UVertexLayOutInfo
{
public:
	std::vector<D3D11_INPUT_ELEMENT_DESC> Descs;
	UINT OffsetSize = 0;

	void AddInputLayOut(
		LPCSTR _SemanticName,
		DXGI_FORMAT _Format, // float4 DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT
		UINT _SemanticIndex = 0, // POSITION0 POSITION1
		UINT _InputSlot = 0,
		UINT _AlignedByteOffset = -1, // -1
		D3D11_INPUT_CLASSIFICATION _InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,
		UINT _InstanceDataStepRate = 0
	)
	{
		D3D11_INPUT_ELEMENT_DESC Desc;

		Desc.SemanticName = _SemanticName; // "POSITION"
		Desc.SemanticIndex = _SemanticIndex;
		Desc.Format = _Format;

		// �� ���̾ƿ��� ������ ���� �ִµ�.
		// �׶� ���� ��ġ�Դϴ�.
		// ���Ŀ� �ν��Ͻ��̶� ������ �ֽ��ϴ�.
		// �ϴ� 0 �־������ ��.
		Desc.InputSlot = _InputSlot;
		Desc.AlignedByteOffset = _InputSlot;

		// �ڵ�����ض�.
		if (-1 == _AlignedByteOffset)
		{
			Desc.AlignedByteOffset += OffsetSize;
		}

		OffsetSize += UDirectXHelper::DirectFormatSize(_Format);

		// D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
		// D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_INSTANCE_DATA
		Desc.InputSlotClass = _InputSlotClass;
		Desc.InstanceDataStepRate = _InstanceDataStepRate;

		Descs.push_back(Desc);
	}

};

#define VERTEXMEM(Type, Name) Type Name
// FEngineVertex::Info.AddInputLayOut("POSITION", DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT);

struct FEngineVertex
{
	// ������ �̸��� Info���� �մϴ�.
	static UVertexLayOutInfo Info;

	float4 POSITION;
	float4 TEXCOORD;
};