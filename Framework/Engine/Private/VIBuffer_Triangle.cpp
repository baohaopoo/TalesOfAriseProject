#include "..\Public\VIBuffer_Triangle.h"
#include "Shader.h"

CVIBuffer_Triangle::CVIBuffer_Triangle(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CVIBuffer(pDevice, pDeviceContext)
{
}

CVIBuffer_Triangle::CVIBuffer_Triangle(const CVIBuffer_Triangle& rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Triangle::NativeConstruct_Prototype()
{
#pragma region VERTEX_BUFFER
	ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));

	m_iNumIndicesPerPrimitive = 2;
	m_iNumVertices = 3;
	m_iNumVBuffers = 1;

	m_VBDesc.ByteWidth = sizeof(VTXCOL) * m_iNumVertices;
	m_VBDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBDesc.StructureByteStride = sizeof(VTXCOL);
	m_VBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	VTXCOL* pVertices = new VTXCOL[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXCOL) * m_iNumVertices);

	pVertices[0].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[0].vColor = _float4(1.0f, 1.f, 1.f, 1.f);

	pVertices[1].vPosition = _float3(0.0f, 0.5f, 0.f);
	pVertices[1].vColor = _float4(1.0f, 1.f, 1.f, 1.f);

	pVertices[2].vPosition = _float3(0.5f, -0.5f, 0.f);
	pVertices[2].vColor = _float4(1.0f, 1.f, 1.f, 1.f);

	ZeroMemory(&m_VBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_VBSubResourceData.pSysMem = pVertices;

	if (FAILED(Create_VertexBuffer()))
	{
		MSG_BOX(L"Failed To CVIBuffer_Triangle : NativeConstruct_Prototype : Create_VertexBuffer");
		return E_FAIL;
	}
	Safe_Delete_Array(pVertices);

#pragma endregion

#pragma region INDEX_BUFFER
	m_iNumPrimitive = 3;
	m_eFormat = DXGI_FORMAT_R16_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;

	ZeroMemory(&m_IBDesc, sizeof(D3D11_BUFFER_DESC));
	m_IBDesc.ByteWidth = sizeof(FACELISTINDICES16) * m_iNumPrimitive;
	m_IBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	_ushort* pIndices = new _ushort[4];
	ZeroMemory(pIndices, sizeof(_ushort) * 4);

	pIndices[0] = 0;
	pIndices[1] = 1;
	pIndices[2] = 2;
	pIndices[3] = 0;

	ZeroMemory(&m_IBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_IBSubResourceData.pSysMem = pIndices;

	if (FAILED(Create_IndexBuffer()))
	{
		MSG_BOX(L"Failed To CVIBuffer_Triangle : NativeConstruct_Prototype : Create_IndexBuffer");
		return E_FAIL;
	}
	Safe_Delete_Array(pIndices);

#pragma endregion
	return S_OK;
}

HRESULT CVIBuffer_Triangle::NativeConstruct(void* pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_Triangle::Render(CShader* pShader, _uint iPassIndex)
{
	if (nullptr == m_pDeviceContext)
	{
		MSG_BOX(L"Failed To CVIBuffer_Triangle : Render : nullptr == m_pDeviceContext");
		return E_FAIL;
	}
	ID3D11Buffer* pVertexBuffers[] = {
		m_pVB
	};
	_uint iStrides[] = {
		m_VBDesc.StructureByteStride
	};
	_uint iOffsets[] = {
		0,
	};

	m_pDeviceContext->IASetVertexBuffers(0, m_iNumVBuffers, pVertexBuffers, iStrides, iOffsets);
	m_pDeviceContext->IASetIndexBuffer(m_pIB, m_eFormat, 0);
	m_pDeviceContext->IASetPrimitiveTopology(m_eTopology);

	if (FAILED(pShader->Begin(iPassIndex)))
	{
		MSG_BOX(L"Failed To CVIBuffer_Triangle : Render : pShader->Begin");
		return E_FAIL;
	}
	m_pDeviceContext->DrawIndexed(4, 0, 0);

	return S_OK;
}

HRESULT CVIBuffer_Triangle::Update(const _float3 * pPoints)
{
	if (nullptr == m_pDeviceContext)
	{
		MSG_BOX(L"Failed To CVIBuffer_Triangle : Update : nullptr == m_pDeviceContext");
		return E_FAIL;
	}
	D3D11_MAPPED_SUBRESOURCE SubResource;
	ZeroMemory(&SubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	m_pDeviceContext->Map(m_pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResource);

	for (_uint i = 0; i < 3; ++i)
	{
		((VTXCOL*)SubResource.pData)[i].vPosition = pPoints[i];
		((VTXCOL*)SubResource.pData)[i].vColor = _float4(1.f, 1.f, 1.f, 1.f);
	}
	m_pDeviceContext->Unmap(m_pVB, 0);
	return S_OK;
}

CVIBuffer_Triangle* CVIBuffer_Triangle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CVIBuffer_Triangle*	pInstance = new CVIBuffer_Triangle(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(L"Failed To CVIBuffer_Triangle : Create : pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CVIBuffer_Triangle::Clone(void * pArg)
{
	CVIBuffer_Triangle*	pInstance = new CVIBuffer_Triangle(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To CVIBuffer_Triangle : Clone : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Triangle::Free()
{
	__super::Free();
}