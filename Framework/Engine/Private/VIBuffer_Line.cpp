#include "..\Public\VIBuffer_line.h"

CVIBuffer_Line::CVIBuffer_Line(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CVIBuffer(pDevice, pDeviceContext)
{
}

CVIBuffer_Line::CVIBuffer_Line(const CVIBuffer_Line & rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Line::NativeConstruct_Prototype(const _float3* pPoints, _uint iNumPoints, const _float4* pColor)
{
#pragma region VERTEX_BUFFER
	m_iStride = sizeof(VTXCOL);
	m_iNumVertices = iNumPoints;
	m_iNumVBuffers = 1;

	ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));
	m_VBDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_VBDesc.StructureByteStride = m_iStride;
	m_VBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBDesc.CPUAccessFlags = 0;
	m_VBDesc.MiscFlags = 0;

	VTXCOL*		pVertices = new VTXCOL[m_iNumVertices];

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		pVertices[i].vPosition = pPoints[i];
		pVertices[i].vColor = *pColor;
	}	
	
	ZeroMemory(&m_VBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_VBSubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);
#pragma endregion

#pragma region INDEX_BUFFER	

	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;

#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_Line::NativeConstruct(void * pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_Line::Render()
{
	if (nullptr == m_pDeviceContext)
		return E_FAIL;

	ID3D11Buffer*	pVertexBuffers[] = {
		m_pVB
	};

	_uint			iStrides[] = {
		m_iStride
	};

	_uint			iOffset[] = {
		0
	};

	m_pDeviceContext->IASetVertexBuffers(0, m_iNumVBuffers, pVertexBuffers, iStrides, iOffset);	
	m_pDeviceContext->IASetPrimitiveTopology(m_eTopology);
	m_pDeviceContext->Draw(m_iNumVertices, 0);	

	return S_OK;
}

CVIBuffer_Line * CVIBuffer_Line::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const _float3* pPoints, _uint iNumPoints, const _float4* pColor)
{
	CVIBuffer_Line*	pInstance = new CVIBuffer_Line(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(pPoints, iNumPoints, pColor)))
	{
		MSG_BOX(TEXT("Failed to Created CVIBuffer_Line"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_Line::Clone(void * pArg)
{
	CVIBuffer_Line*	pInstance = new CVIBuffer_Line(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CVIBuffer_Line"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Line::Free()
{
	__super::Free();

}
