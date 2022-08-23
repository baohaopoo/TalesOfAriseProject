#include "..\Public\VIBuffer_Terrain.h"
#include "Frustum.h"
#include "QuadTree.h"
#include "Transform.h"

CVIBuffer_Terrain::CVIBuffer_Terrain(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CVIBuffer(pDevice, pDeviceContext)
{

}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain & rhs)
	: CVIBuffer(rhs)
	, m_pIndices(rhs.m_pIndices)
	, m_iNumVerticesX(rhs.m_iNumVerticesX)
	, m_iNumVerticesZ(rhs.m_iNumVerticesZ)
	, m_pQuadTree(rhs.m_pQuadTree)
{
	Safe_AddRef(m_pQuadTree);

}

HRESULT CVIBuffer_Terrain::NativeConstruct_Prototype(const _tchar* pHeightMapPath)
{
	_ulong		dwByte = 0;
	HANDLE		hFile = CreateFile(pHeightMapPath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	BITMAPFILEHEADER		fh;
	BITMAPINFOHEADER		ih;

	ReadFile(hFile, &fh, sizeof(BITMAPFILEHEADER), &dwByte, nullptr);
	ReadFile(hFile, &ih, sizeof(BITMAPINFOHEADER), &dwByte, nullptr);

	_ulong*		pPixel = new _ulong[ih.biWidth * ih.biHeight];

	ReadFile(hFile, pPixel, sizeof(_ulong) * ih.biWidth * ih.biHeight, &dwByte, nullptr);

	CloseHandle(hFile);

#pragma region VERTEX_BUFFER
	m_iStride = sizeof(VTXNORTEX);
	m_iNumVerticesX = ih.biWidth;
	m_iNumVerticesZ = ih.biHeight;
	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;
	m_iNumVBuffers = 1;

	ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));
	m_VBDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_VBDesc.StructureByteStride = m_iStride;
	m_VBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBDesc.CPUAccessFlags = 0;
	m_VBDesc.MiscFlags = 0;

	VTXNORTEX*		pVertices = new VTXNORTEX[m_iNumVertices];
	m_pVerticesPos = new _float3[m_iNumVertices];

	for (_uint i = 0; i < m_iNumVerticesZ; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX; ++j)
		{
			_uint iIndex = i * m_iNumVerticesX + j;

			m_pVerticesPos[iIndex] = pVertices[iIndex].vPosition = _float3((_float)j, 0.f, (_float)i);
			pVertices[iIndex].vNormal = _float3(0.f, 0.f, 0.f);
			pVertices[iIndex].vTexUV = _float2(j / _float(m_iNumVerticesX - 1), i / _float(m_iNumVerticesZ - 1));
		}
	}
	
	ZeroMemory(&m_VBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_VBSubResourceData.pSysMem = pVertices;



	Safe_Delete_Array(pPixel);
#pragma endregion

#pragma region INDEX_BUFFER

	m_iPrimitiveIndicesSize = sizeof(FACELISTINDICES32);
	m_iNumPrimitive = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2;
	m_iNumIndicesPerPrimitive = 3;
	m_eFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;


	ZeroMemory(&m_IBDesc, sizeof(D3D11_BUFFER_DESC));
	m_IBDesc.ByteWidth = m_iPrimitiveIndicesSize * m_iNumPrimitive;
	m_IBDesc.StructureByteStride = 0;
	m_IBDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_IBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_IBDesc.MiscFlags = 0;
	
	FACELISTINDICES32*		pIndices = new FACELISTINDICES32[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(FACELISTINDICES32) * m_iNumPrimitive);

	m_pIndices = new FACELISTINDICES32[m_iNumPrimitive];
	ZeroMemory(m_pIndices, sizeof(FACELISTINDICES32) * m_iNumPrimitive);

	_ulong		dwNumFace = 0;

	for (_uint i = 0; i < m_iNumVerticesZ - 1; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX - 1; ++j)
		{
			_uint iIndex = i * m_iNumVerticesX + j;

			_uint iIndices[] = {
				iIndex + m_iNumVerticesX, 
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			pIndices[dwNumFace]._0 = iIndices[0];
			pIndices[dwNumFace]._1 = iIndices[1];
			pIndices[dwNumFace]._2 = iIndices[2];

			_vector		vSour, vDest, vNormal;
			vSour = XMLoadFloat3(&pVertices[pIndices[dwNumFace]._1].vPosition) - XMLoadFloat3(&pVertices[pIndices[dwNumFace]._0].vPosition);
			vDest = XMLoadFloat3(&pVertices[pIndices[dwNumFace]._2].vPosition) - XMLoadFloat3(&pVertices[pIndices[dwNumFace]._1].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSour, vDest));

			/* 면을 구성하는 첫번째 정점의 노말에 면의 노말을 더한다. */
			XMStoreFloat3(&pVertices[pIndices[dwNumFace]._0].vNormal, 
				XMLoadFloat3(&pVertices[pIndices[dwNumFace]._0].vNormal) + vNormal);		

			/* 면을 구성하는 두번째 정점의 노말에 면의 노말을 더한다. */
			XMStoreFloat3(&pVertices[pIndices[dwNumFace]._1].vNormal,
				XMLoadFloat3(&pVertices[pIndices[dwNumFace]._1].vNormal) + vNormal);			

			/* 면을 구성하는 세번째 정점의 노말에 면의 노말을 더한다. */
			XMStoreFloat3(&pVertices[pIndices[dwNumFace]._2].vNormal,
				XMLoadFloat3(&pVertices[pIndices[dwNumFace]._2].vNormal) + vNormal);			

			++dwNumFace;

			pIndices[dwNumFace]._0 = iIndices[0];
			pIndices[dwNumFace]._1 = iIndices[2];
			pIndices[dwNumFace]._2 = iIndices[3];

			vSour = XMLoadFloat3(&pVertices[pIndices[dwNumFace]._1].vPosition) - XMLoadFloat3(&pVertices[pIndices[dwNumFace]._0].vPosition);
			vDest = XMLoadFloat3(&pVertices[pIndices[dwNumFace]._2].vPosition) - XMLoadFloat3(&pVertices[pIndices[dwNumFace]._1].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSour, vDest));

			/* 면을 구성하는 첫번째 정점의 노말에 면의 노말을 더한다. */
			XMStoreFloat3(&pVertices[pIndices[dwNumFace]._0].vNormal,
				XMLoadFloat3(&pVertices[pIndices[dwNumFace]._0].vNormal) + vNormal);			

			/* 면을 구성하는 두번째 정점의 노말에 면의 노말을 더한다. */
			XMStoreFloat3(&pVertices[pIndices[dwNumFace]._1].vNormal,
				XMLoadFloat3(&pVertices[pIndices[dwNumFace]._1].vNormal) + vNormal);			

			/* 면을 구성하는 세번째 정점의 노말에 면의 노말을 더한다. */
			XMStoreFloat3(&pVertices[pIndices[dwNumFace]._2].vNormal,
				XMLoadFloat3(&pVertices[pIndices[dwNumFace]._2].vNormal) + vNormal);

			++dwNumFace;
		}
	}	

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3Normalize(XMLoadFloat3(&pVertices[i].vNormal)));
	}

	ZeroMemory(&m_IBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_IBSubResourceData.pSysMem = pIndices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;
#pragma endregion

	Safe_Delete_Array(pIndices);
	Safe_Delete_Array(pVertices);



	m_pQuadTree = CQuadTree::Create(m_iNumVerticesX * m_iNumVerticesZ - m_iNumVerticesX, 
		m_iNumVerticesX * m_iNumVerticesZ - 1, 
		m_iNumVerticesX - 1, 
		0);


	return S_OK;
}


HRESULT CVIBuffer_Terrain::NativeConstruct(void * pArg)
{
	return S_OK;
}

void CVIBuffer_Terrain::Culling(CTransform* pTransform)
{
	CFrustum*			pFrustum = GET_INSTANCE(CFrustum);

	pFrustum->Transform_ToLocalSpace(pTransform->Get_WorldMatrix());

	_uint				iNumFaces = 0;

	m_pQuadTree->Culling(pFrustum, m_pVerticesPos, m_pIndices, &iNumFaces);

	//for (_uint i = 0; i < m_iNumVerticesZ - 1; ++i)
	//{
	//	for (_uint j = 0; j < m_iNumVerticesX - 1; ++j)
	//	{
	//		_uint		iIndex = i * m_iNumVerticesX + j;

	//		_uint iIndices[] = {
	//			iIndex + m_iNumVerticesX,
	//			iIndex + m_iNumVerticesX + 1,
	//			iIndex + 1,
	//			iIndex
	//		};

	//		_bool		isInFrustum[4] = {
	//			pFrustum->isIn_LocalSpace(XMLoadFloat3(&m_pVerticesPos[iIndices[0]]), 0.f), 
	//			pFrustum->isIn_LocalSpace(XMLoadFloat3(&m_pVerticesPos[iIndices[1]]), 0.f),
	//			pFrustum->isIn_LocalSpace(XMLoadFloat3(&m_pVerticesPos[iIndices[2]]), 0.f),
	//			pFrustum->isIn_LocalSpace(XMLoadFloat3(&m_pVerticesPos[iIndices[3]]), 0.f)
	//		};

	//		if (true == isInFrustum[0] &&
	//			true == isInFrustum[1] &&
	//			true == isInFrustum[2])
	//		{
	//			m_pIndices[iNumFaces]._0 = iIndices[0];
	//			m_pIndices[iNumFaces]._1 = iIndices[1];
	//			m_pIndices[iNumFaces]._2 = iIndices[2];
	//			++iNumFaces;
	//		}

	//		if (true == isInFrustum[0] &&
	//			true == isInFrustum[2] &&
	//			true == isInFrustum[3])
	//		{
	//			m_pIndices[iNumFaces]._0 = iIndices[0];
	//			m_pIndices[iNumFaces]._1 = iIndices[2];
	//			m_pIndices[iNumFaces]._2 = iIndices[3];
	//			++iNumFaces;
	//		}
	//	}	
	//}

	


	D3D11_MAPPED_SUBRESOURCE		MappedSubResource;
	ZeroMemory(&MappedSubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	m_pDeviceContext->Map(m_pIB, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedSubResource);
	
	memcpy(MappedSubResource.pData, m_pIndices, sizeof(FACELISTINDICES32) * iNumFaces);

	m_pDeviceContext->Unmap(m_pIB, 0);	

	m_iNumPrimitive = iNumFaces;

	RELEASE_INSTANCE(CFrustum);
}

_float CVIBuffer_Terrain::Compute_Height(CTransform * pTransform, _fvector vPosition)
{
	_vector		vLocalPos = XMVector3TransformCoord(vPosition, pTransform->Get_WorldMatrixInverse());

	_uint		iIndex = (_uint)XMVectorGetZ(vLocalPos) * m_iNumVerticesX + (_uint)XMVectorGetX(vLocalPos);

	_uint		iIndices[] = {
		iIndex + m_iNumVerticesX, 
		iIndex + m_iNumVerticesX + 1,
		iIndex + 1,
		iIndex
	};

	_float		fWidth = XMVectorGetX(vLocalPos) - m_pVerticesPos[iIndex + m_iNumVerticesX].x;
	_float		fHeight = m_pVerticesPos[iIndex + m_iNumVerticesX].z - XMVectorGetZ(vLocalPos);

	_vector		vPlane;

	if (fWidth > fHeight)
	{
		vPlane = XMPlaneFromPoints(XMLoadFloat3(&m_pVerticesPos[iIndices[0]]),
			XMLoadFloat3(&m_pVerticesPos[iIndices[1]]),
			XMLoadFloat3(&m_pVerticesPos[iIndices[2]]));
	}
	else
	{
		vPlane = XMPlaneFromPoints(XMLoadFloat3(&m_pVerticesPos[iIndices[0]]),
			XMLoadFloat3(&m_pVerticesPos[iIndices[2]]),
			XMLoadFloat3(&m_pVerticesPos[iIndices[3]]));
	}

	return (-XMVectorGetX(vPlane) * XMVectorGetX(vLocalPos) - XMVectorGetZ(vPlane) * XMVectorGetZ(vLocalPos) - XMVectorGetW(vPlane)) / XMVectorGetY(vPlane);
}

CVIBuffer_Terrain * CVIBuffer_Terrain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const _tchar* pHeightMapPath)
{
	CVIBuffer_Terrain*	pInstance = new CVIBuffer_Terrain(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(pHeightMapPath)))
	{
		MSG_BOX(TEXT("Failed to Created CVIBuffer_Terrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_Terrain::Clone(void * pArg)
{
	CVIBuffer_Terrain*	pInstance = new CVIBuffer_Terrain(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CVIBuffer_Terrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Terrain::Free()
{
	__super::Free();

	Safe_Release(m_pQuadTree);

	if (false == m_isCloned)
		Safe_Delete_Array(m_pIndices);

}
