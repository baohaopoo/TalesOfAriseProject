#include "..\Public\VIBuffer_Point_Instance.h"

CVIBuffer_Point_Instance::CVIBuffer_Point_Instance(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CVIBuffer(pDevice, pDeviceContext)
{
}

CVIBuffer_Point_Instance::CVIBuffer_Point_Instance(const CVIBuffer_Point_Instance & rhs)
	: CVIBuffer(rhs)
	, m_pVBInstance(rhs.m_pVBInstance)
	, m_VBInstDesc(rhs.m_VBInstDesc)
	, m_iInstanceStride(rhs.m_iInstanceStride)
	, m_iNumInstance(rhs.m_iNumInstance)
	, m_pInstanceSpeed(rhs.m_pInstanceSpeed)
{
	Safe_AddRef(m_pVBInstance);
}

HRESULT CVIBuffer_Point_Instance::NativeConstruct_Prototype(_uint iNumInstance)
{
#pragma region VERTEX_BUFFER
	m_iStride = sizeof(VTXPOINT);
	m_iNumVertices = 1;
	m_iNumVBuffers = 2;	

	ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));
	m_VBDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_VBDesc.StructureByteStride = m_iStride;
	m_VBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBDesc.CPUAccessFlags = 0;
	m_VBDesc.MiscFlags = 0;

	VTXPOINT*		pVertices = new VTXPOINT[m_iNumVertices];

	pVertices->vPosition = _float3(0.f, 0.f, 0.f);
	pVertices->vPSize = _float2(0.25f, 0.25f);

	ZeroMemory(&m_VBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_VBSubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);
#pragma endregion


#pragma region INSTANCE_BUFFER

	m_iInstanceStride = sizeof(VTXMATRIX);
	m_iNumInstance = iNumInstance;

	ZeroMemory(&m_VBInstDesc, sizeof(D3D11_BUFFER_DESC));
	m_VBInstDesc.ByteWidth = m_iInstanceStride * m_iNumInstance;
	m_VBInstDesc.StructureByteStride = m_iInstanceStride;
	m_VBInstDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_VBInstDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBInstDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_VBInstDesc.MiscFlags = 0;

	VTXMATRIX*		pInstanceVertices = new VTXMATRIX[m_iNumInstance];

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		pInstanceVertices[i].vRight = _float4(1.f, 0.f, 0.f, 0.f);
		pInstanceVertices[i].vUp = _float4(0.f, 1.f, 0.f, 0.f);
		pInstanceVertices[i].vLook = _float4(0.f, 0.f, 1.f, 0.f);
		pInstanceVertices[i].vTranslation = _float4(rand() % 5, 10.0f, rand() % 5, 1.f);		
	}

	ZeroMemory(&m_VBInstSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_VBInstSubResourceData.pSysMem = pInstanceVertices;

	if (FAILED(m_pDevice->CreateBuffer(&m_VBInstDesc, &m_VBInstSubResourceData, &m_pVBInstance)))
		return E_FAIL;

	Safe_Delete_Array(pInstanceVertices);

	m_pInstanceSpeed = new _float[m_iNumInstance];

	for (_uint i = 0; i < m_iNumInstance; ++i)
		m_pInstanceSpeed[i] = rand() % 7 + 3;
#pragma endregion

#pragma region INDEX_BUFFER

	m_iPrimitiveIndicesSize = sizeof(_ushort);
	m_iNumPrimitive = m_iNumInstance;
	m_iNumIndicesPerPrimitive = 1;
	m_eFormat = DXGI_FORMAT_R16_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;


	ZeroMemory(&m_IBDesc, sizeof(D3D11_BUFFER_DESC));
	m_IBDesc.ByteWidth = m_iPrimitiveIndicesSize * m_iNumPrimitive;
	m_IBDesc.StructureByteStride = 0;
	m_IBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_IBDesc.CPUAccessFlags = 0;
	m_IBDesc.MiscFlags = 0;
	
	_ushort*		pIndices = new _ushort[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(_ushort) * m_iNumPrimitive);	

	ZeroMemory(&m_IBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_IBSubResourceData.pSysMem = pIndices;

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;
#pragma endregion

	Safe_Delete_Array(pIndices);



	return S_OK;
}

HRESULT CVIBuffer_Point_Instance::NativeConstruct(void * pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_Point_Instance::Update(_double TimeDelta)
{
	if (nullptr == m_pDeviceContext)
		return E_FAIL;

	D3D11_MAPPED_SUBRESOURCE			SubResource;

	m_pDeviceContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		_vector			vPosition = XMLoadFloat4(&((VTXMATRIX*)SubResource.pData)[i].vTranslation);

		vPosition += XMVectorSet(0.f, -1.f, 0.f, 0.f) * m_pInstanceSpeed[i] * TimeDelta;

		if (vPosition.m128_f32[1] < 0.0f)
			vPosition.m128_f32[1] = 10.f;

		XMStoreFloat4(&((VTXMATRIX*)SubResource.pData)[i].vTranslation, vPosition);



	}

	m_pDeviceContext->Unmap(m_pVBInstance, 0);

	return S_OK;
}

HRESULT CVIBuffer_Point_Instance::Render()
{
	if (nullptr == m_pDeviceContext)
		return E_FAIL;

	ID3D11Buffer*	pVertexBuffers[] = {
		m_pVB, 
		m_pVBInstance
	};

	_uint			iStrides[] = {
		m_iStride, 
		m_iInstanceStride

	};

	_uint			iOffset[] = {
		0, 0
	};

	m_pDeviceContext->IASetVertexBuffers(0, m_iNumVBuffers, pVertexBuffers, iStrides, iOffset);
	m_pDeviceContext->IASetIndexBuffer(m_pIB, m_eFormat, 0);
	m_pDeviceContext->IASetPrimitiveTopology(m_eTopology);

	m_pDeviceContext->DrawIndexedInstanced(1, m_iNumInstance, 0, 0, 0);

	return S_OK;
}

CVIBuffer_Point_Instance * CVIBuffer_Point_Instance::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _uint iNumInstance)
{
	CVIBuffer_Point_Instance*	pInstance = new CVIBuffer_Point_Instance(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(iNumInstance)))
	{
		MSG_BOX(TEXT("Failed to Created CVIBuffer_Point_Instance"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_Point_Instance::Clone(void * pArg)
{
	CVIBuffer_Point_Instance*	pInstance = new CVIBuffer_Point_Instance(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CVIBuffer_Point_Instance"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Point_Instance::Free()
{
	__super::Free();

	if (false == m_isCloned)
		Safe_Delete_Array(m_pInstanceSpeed);

	Safe_Release(m_pVBInstance);

}
