#include "..\Public\VIBuffer_Rect_Instance.h"

CVIBuffer_Rect_Instance::CVIBuffer_Rect_Instance(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CVIBuffer(pDevice, pDeviceContext)
{
}

CVIBuffer_Rect_Instance::CVIBuffer_Rect_Instance(const CVIBuffer_Rect_Instance & rhs)
	: CVIBuffer(rhs)
	, m_pVBInstance(rhs.m_pVBInstance)
	, m_VBInstDesc(rhs.m_VBInstDesc)
	, m_iInstanceStride(rhs.m_iInstanceStride)
	, m_iNumInstance(rhs.m_iNumInstance)

{
	Safe_AddRef(m_pVBInstance);
}

HRESULT CVIBuffer_Rect_Instance::NativeConstruct_Prototype(_uint iNumInstance)
{
	//#pragma region VERTEX_BUFFER
	//	m_iStride = sizeof(VTXTEX);
	//	m_iNumVertices = 4;
	//	m_iNumVBuffers = 2;	
	//
	//	ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));
	//	m_VBDesc.ByteWidth = m_iStride * m_iNumVertices;
	//	m_VBDesc.StructureByteStride = m_iStride;
	//	m_VBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	//	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//	m_VBDesc.CPUAccessFlags = 0;
	//	m_VBDesc.MiscFlags = 0;
	//	VTXTEX*		pVertices = new VTXTEX[m_iNumVertices];
	//
	//		pVertices[0].vPosition = _float3(0.f, 0.5f, -0.5f);
	//		pVertices[0].vTexUV = _float2(0.0f, 0.0f);
	//
	//		pVertices[1].vPosition = _float3(0.f, 0.5f, 0.5f);
	//		pVertices[1].vTexUV = _float2(1.0f, 0.0f);
	//
	//		pVertices[2].vPosition = _float3(0.f, -0.5f, 0.5f);
	//		pVertices[2].vTexUV = _float2(1.0f, 1.0f);
	//
	//		pVertices[3].vPosition = _float3(0.f, -0.5f, -0.5f);
	//		pVertices[3].vTexUV = _float2(0.0f, 1.0f);
	//	
	//
	//	ZeroMemory(&m_VBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	//	m_VBSubResourceData.pSysMem = pVertices;
	//
	//	if (FAILED(__super::Create_VertexBuffer()))
	//		return E_FAIL;
	//	
	//	Safe_Delete_Array(pVertices);
	//#pragma endregion
	//
	//
	//#pragma region INSTANCE_BUFFER
	//
	//	m_iInstanceStride = sizeof(VTXMATRIX);
	m_iNumInstance = iNumInstance;
	//
	//	ZeroMemory(&m_VBInstDesc, sizeof(D3D11_BUFFER_DESC));
	//	m_VBInstDesc.ByteWidth = m_iInstanceStride * m_iNumInstance;
	//	m_VBInstDesc.StructureByteStride = m_iInstanceStride;
	//	m_VBInstDesc.Usage = D3D11_USAGE_DYNAMIC;
	//	m_VBInstDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//	m_VBInstDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//	m_VBInstDesc.MiscFlags = 0;
	//
	//	VTXMATRIX*		pInstanceVertices = new VTXMATRIX[m_iNumInstance];
	//
	//	for (_uint i = 0; i < m_iNumInstance; ++i)
	//	{
	//		pInstanceVertices[i].vRight = _float4(1.f, 0.f, 0.f, 0.f);
	//		pInstanceVertices[i].vUp = _float4(0.f, 1.f, 0.f, 0.f);
	//		pInstanceVertices[i].vLook = _float4(0.f, 0.f, 1.f, 0.f);
	//		pInstanceVertices[i].vTranslation = _float4(rand() % 3, 10.0f, rand() % 3, 1.f);		
	//	}
	//
	//	ZeroMemory(&m_VBInstSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	//	m_VBInstSubResourceData.pSysMem = pInstanceVertices;
	//
	//	if (FAILED(m_pDevice->CreateBuffer(&m_VBInstDesc, &m_VBInstSubResourceData, &m_pVBInstance)))
	//		return E_FAIL;
	//
	//	Safe_Delete_Array(pInstanceVertices);
	//
	//	
	//#pragma endregion
	//
	//#pragma region INDEX_BUFFER
	//
	//	m_iPrimitiveIndicesSize = sizeof(FACELISTINDICES16);
	//	m_iNumPrimitive = 2 * m_iNumInstance;
	//	m_iNumIndicesPerPrimitive = 3;
	//	m_eFormat = DXGI_FORMAT_R16_UINT;
	//	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	//
	//
	//	ZeroMemory(&m_IBDesc, sizeof(D3D11_BUFFER_DESC));
	//	m_IBDesc.ByteWidth = m_iPrimitiveIndicesSize * m_iNumPrimitive;
	//	m_IBDesc.StructureByteStride = 0;
	//	m_IBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	//	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//	m_IBDesc.CPUAccessFlags = 0;
	//	m_IBDesc.MiscFlags = 0;
	//	
	//	FACELISTINDICES16*		pIndices = new FACELISTINDICES16[m_iNumPrimitive];
	//	ZeroMemory(pIndices, sizeof(FACELISTINDICES16) * m_iNumPrimitive);
	//
	//	_uint		iNumFaces = 0;
	//
	//	for (_uint i = 0; i < m_iNumInstance; ++i)
	//	{
	//		pIndices[iNumFaces]._0 = 0;
	//		pIndices[iNumFaces]._1 = 1;
	//		pIndices[iNumFaces]._2 = 2;
	//		++iNumFaces;
	//
	//		pIndices[iNumFaces]._0 = 0;
	//		pIndices[iNumFaces]._1 = 2;
	//		pIndices[iNumFaces]._2 = 3;
	//		++iNumFaces;
	//	}
	//
	//	ZeroMemory(&m_IBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	//	m_IBSubResourceData.pSysMem = pIndices;
	//
	//	if (FAILED(Create_IndexBuffer()))
	//		return E_FAIL;
	//#pragma endregion
	//	
	//

	//Safe_Delete_Array(pIndices);



	return S_OK;
}

HRESULT CVIBuffer_Rect_Instance::NativeConstruct(void * pArg)
{
	if (pArg != nullptr)
	{
		m_EffectDesc = *(EFFECTDESC*)pArg;
	}

	m_iNumInstance = m_EffectDesc.iNum;
#pragma region VERTEX_BUFFER
	m_iStride = sizeof(VTXTEX);
	m_iNumVertices = 4;
	m_iNumVBuffers = 2;

	ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));
	m_VBDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_VBDesc.StructureByteStride = m_iStride;
	m_VBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBDesc.CPUAccessFlags = 0;
	m_VBDesc.MiscFlags = 0;
	VTXTEX*		pVertices = new VTXTEX[m_iNumVertices];

	pVertices[0].vPosition = _float3(0.f, 0.5f, -0.5f);
	pVertices[0].vTexUV = _float2(0.0f, 0.0f);

	pVertices[1].vPosition = _float3(0.f, 0.5f, 0.5f);
	pVertices[1].vTexUV = _float2(1.0f, 0.0f);

	pVertices[2].vPosition = _float3(0.f, -0.5f, 0.5f);
	pVertices[2].vTexUV = _float2(1.0f, 1.0f);

	pVertices[3].vPosition = _float3(0.f, -0.5f, -0.5f);
	pVertices[3].vTexUV = _float2(0.0f, 1.0f);


	ZeroMemory(&m_VBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_VBSubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);
#pragma endregion


#pragma region INSTANCE_BUFFER

	m_iInstanceStride = sizeof(VTXMATRIX);


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
		pInstanceVertices[i].vTranslation = _float4(rand() % 3, 10.0f, rand() % 3, 1.f);
	}

	ZeroMemory(&m_VBInstSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_VBInstSubResourceData.pSysMem = pInstanceVertices;

	if (FAILED(m_pDevice->CreateBuffer(&m_VBInstDesc, &m_VBInstSubResourceData, &m_pVBInstance)))
		return E_FAIL;

	Safe_Delete_Array(pInstanceVertices);


#pragma endregion

#pragma region INDEX_BUFFER

	m_iPrimitiveIndicesSize = sizeof(FACELISTINDICES16);
	m_iNumPrimitive = 2 * m_iNumInstance;
	m_iNumIndicesPerPrimitive = 3;
	m_eFormat = DXGI_FORMAT_R16_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;


	ZeroMemory(&m_IBDesc, sizeof(D3D11_BUFFER_DESC));
	m_IBDesc.ByteWidth = m_iPrimitiveIndicesSize * m_iNumPrimitive;
	m_IBDesc.StructureByteStride = 0;
	m_IBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_IBDesc.CPUAccessFlags = 0;
	m_IBDesc.MiscFlags = 0;

	FACELISTINDICES16*		pIndices = new FACELISTINDICES16[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(FACELISTINDICES16) * m_iNumPrimitive);

	_uint		iNumFaces = 0;

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		pIndices[iNumFaces]._0 = 0;
		pIndices[iNumFaces]._1 = 1;
		pIndices[iNumFaces]._2 = 2;
		++iNumFaces;

		pIndices[iNumFaces]._0 = 0;
		pIndices[iNumFaces]._1 = 2;
		pIndices[iNumFaces]._2 = 3;
		++iNumFaces;
	}

	ZeroMemory(&m_IBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_IBSubResourceData.pSysMem = pIndices;

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;
#pragma endregion



	Safe_Delete_Array(pIndices);






	m_InstanceDesc = new INSTANCEDESC[m_iNumInstance];
	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		m_InstanceDesc[i].fSpeed = 1.f;
		m_InstanceDesc[i].vMovement = _float4(0.f, 0.f, 1.f, 0.f);
	}

	Reset();

	return S_OK;
}

_bool CVIBuffer_Rect_Instance::Update(_double TimeDelta)
{
	if (nullptr == m_pDeviceContext)
		return E_FAIL;

	m_fTime += TimeDelta;
	if (m_EffectDesc.fLoop == 0.f)
	{
		_vector vLook;


		if (m_fMaxTime < m_fTime)
		{
			return true;
		}
		else if (m_EffectDesc.KeyFram_1_TimeEnd > m_fTime)
		{
			m_KeyFramDesc.KeyFram_TimeEnd = m_EffectDesc.KeyFram_1_TimeEnd;
			m_KeyFramDesc.KeyFram_RelaxSpeed = m_EffectDesc.KeyFram_1_RelaxSpeed;
			m_KeyFramDesc.KeyFram_Speed = m_EffectDesc.KeyFram_1_Speed;
			m_KeyFramDesc.KeyFram_fRotation = m_EffectDesc.KeyFram_1_fRotation;
			m_KeyFramDesc.KeyFram_fRelaxRotation = m_EffectDesc.KeyFram_1_fRelaxRotation;
			m_KeyFramDesc.KeyFram_Movement = m_EffectDesc.KeyFram_1_Movement;
		}
		else if (m_EffectDesc.KeyFram_2_TimeEnd > m_fTime)
		{
			m_KeyFramDesc.KeyFram_TimeEnd = m_EffectDesc.KeyFram_2_TimeEnd;
			m_KeyFramDesc.KeyFram_RelaxSpeed = m_EffectDesc.KeyFram_2_RelaxSpeed;
			m_KeyFramDesc.KeyFram_Speed = m_EffectDesc.KeyFram_2_Speed;
			m_KeyFramDesc.KeyFram_fRotation = m_EffectDesc.KeyFram_2_fRotation;
			m_KeyFramDesc.KeyFram_fRelaxRotation = m_EffectDesc.KeyFram_2_fRelaxRotation;
			m_KeyFramDesc.KeyFram_Movement = m_EffectDesc.KeyFram_2_Movement;
		}
		else if (m_EffectDesc.KeyFram_3_TimeEnd > m_fTime)
		{
			m_KeyFramDesc.KeyFram_TimeEnd = m_EffectDesc.KeyFram_3_TimeEnd;
			m_KeyFramDesc.KeyFram_RelaxSpeed = m_EffectDesc.KeyFram_3_RelaxSpeed;
			m_KeyFramDesc.KeyFram_Speed = m_EffectDesc.KeyFram_3_Speed;
			m_KeyFramDesc.KeyFram_fRotation = m_EffectDesc.KeyFram_3_fRotation;
			m_KeyFramDesc.KeyFram_fRelaxRotation = m_EffectDesc.KeyFram_3_fRelaxRotation;
			m_KeyFramDesc.KeyFram_Movement = m_EffectDesc.KeyFram_3_Movement;
		}
		else if (m_EffectDesc.KeyFram_4_TimeEnd > m_fTime)
		{
			m_KeyFramDesc.KeyFram_TimeEnd = m_EffectDesc.KeyFram_4_TimeEnd;
			m_KeyFramDesc.KeyFram_RelaxSpeed = m_EffectDesc.KeyFram_4_RelaxSpeed;
			m_KeyFramDesc.KeyFram_Speed = m_EffectDesc.KeyFram_4_Speed;
			m_KeyFramDesc.KeyFram_fRotation = m_EffectDesc.KeyFram_4_fRotation;
			m_KeyFramDesc.KeyFram_fRelaxRotation = m_EffectDesc.KeyFram_4_fRelaxRotation;
			m_KeyFramDesc.KeyFram_Movement = m_EffectDesc.KeyFram_4_Movement;
		}


		D3D11_MAPPED_SUBRESOURCE			SubResource;
		m_pDeviceContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);
		_float Size = m_EffectDesc.fScale;
		_float Size_Speed = m_EffectDesc.fScale_Speed;
		for (_uint i = 0; i < m_iNumInstance; ++i)
		{
			m_InstanceDesc[i].fSpeed = m_InstanceDesc[i].fSpeed * m_KeyFramDesc.KeyFram_RelaxSpeed;
			m_InstanceDesc[i].fSpeed += m_KeyFramDesc.KeyFram_Speed  * TimeDelta;

			vLook = XMLoadFloat4(&m_InstanceDesc[i].vMovement);
			XMStoreFloat4(&m_InstanceDesc[i].vMovement, XMVector3Normalize(vLook + XMLoadFloat4(&m_KeyFramDesc.KeyFram_Movement)));
			XMStoreFloat4(&((VTXMATRIX*)SubResource.pData)[i].vTranslation, XMLoadFloat4(&((VTXMATRIX*)SubResource.pData)[i].vTranslation) + (vLook * (m_InstanceDesc[i].fSpeed) * TimeDelta));
			vLook = (vLook * Size) + (vLook * (m_InstanceDesc[i].fSpeed * Size * Size_Speed));
			XMStoreFloat4(&((VTXMATRIX*)SubResource.pData)[i].vLook, vLook);

		}



		m_pDeviceContext->Unmap(m_pVBInstance, 0);
	}
	else
	{

		if (m_fMaxTime + m_EffectDesc.fLoop < m_fTime)
		{
			return true;
		}


		m_fTime += TimeDelta;
		_float Time = m_fMaxTime - m_fTime;
		_float Loop = m_EffectDesc.fLoop;



		D3D11_MAPPED_SUBRESOURCE			SubResource;
		m_pDeviceContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);
		_float Size = m_EffectDesc.fScale;
		_float Size_Speed = m_EffectDesc.fScale_Speed;
		_vector		vLook, vRight, vUp;
		for (_uint i = 0; i < m_iNumInstance; ++i)
		{
			m_InstanceDesc[i].fTime -= TimeDelta;

			if (0 > m_InstanceDesc[i].fTime)
			{
				if (m_EffectDesc.fLoop < Time - 1.f)
				{
					m_InstanceDesc[i].fSpeed = m_EffectDesc.fSpeed + (rand() % 100 * m_EffectDesc.fSpeedRand / 100);
					m_InstanceDesc[i].vMovement = _float4(m_EffectDesc.vMovement.x + (rand() % 100 * m_EffectDesc.vMovementRand.x / 100), m_EffectDesc.vMovement.y + (rand() % 100 * m_EffectDesc.vMovementRand.y / 100), m_EffectDesc.vMovement.z + (rand() % 100 * m_EffectDesc.vMovementRand.z / 100), 0.f);
					m_InstanceDesc[i].fTime = m_EffectDesc.fLoop;
					_vector		vLook = XMVector3Normalize(XMLoadFloat4(&m_InstanceDesc[i].vMovement));
					_vector		vAxis = XMVectorSet(0.f, 1.f, 0.f, 0.f);
					_vector		vRight = XMVector3Cross(vAxis, vLook);
					_vector		vUp = XMVector3Cross(vLook, vRight);
					_vector		vPos = XMVectorSet(m_EffectDesc.vPostion.x + (rand() % 100 * m_EffectDesc.vPostionRand.x / 100), m_EffectDesc.vPostion.y + (rand() % 100 * m_EffectDesc.vPostionRand.y / 100), m_EffectDesc.vPostion.z + (rand() % 100 * m_EffectDesc.vPostionRand.z / 100), 1.f);

					XMStoreFloat4(&m_InstanceDesc[i].vMovement, vLook);
					XMStoreFloat4(&((VTXMATRIX*)SubResource.pData)[i].vLook, vLook *(Size));
					XMStoreFloat4(&((VTXMATRIX*)SubResource.pData)[i].vRight, XMVector3Normalize(vRight) *Size);
					XMStoreFloat4(&((VTXMATRIX*)SubResource.pData)[i].vUp, XMVector3Normalize(vUp) *Size);
					XMStoreFloat4(&((VTXMATRIX*)SubResource.pData)[i].vTranslation, vPos + vLook* m_EffectDesc.fLastTime);

					m_KeyFramDesc.KeyFram_TimeEnd = m_EffectDesc.KeyFram_1_TimeEnd;
					m_KeyFramDesc.KeyFram_RelaxSpeed = m_EffectDesc.KeyFram_1_RelaxSpeed;
					m_KeyFramDesc.KeyFram_Speed = m_EffectDesc.KeyFram_1_Speed;
					m_KeyFramDesc.KeyFram_fRotation = m_EffectDesc.KeyFram_1_fRotation;
					m_KeyFramDesc.KeyFram_fRelaxRotation = m_EffectDesc.KeyFram_1_fRelaxRotation;
					m_KeyFramDesc.KeyFram_Movement = m_EffectDesc.KeyFram_1_Movement;


				}
				else
				{
					m_InstanceDesc[i].fTime = 0.f;

				}

			}
			else if (m_EffectDesc.KeyFram_3_TimeEnd < m_InstanceDesc[i].fTime)
			{
				m_KeyFramDesc.KeyFram_TimeEnd = m_EffectDesc.KeyFram_4_TimeEnd;
				m_KeyFramDesc.KeyFram_RelaxSpeed = m_EffectDesc.KeyFram_4_RelaxSpeed;
				m_KeyFramDesc.KeyFram_Speed = m_EffectDesc.KeyFram_4_Speed;
				m_KeyFramDesc.KeyFram_fRotation = m_EffectDesc.KeyFram_4_fRotation;
				m_KeyFramDesc.KeyFram_fRelaxRotation = m_EffectDesc.KeyFram_4_fRelaxRotation;
				m_KeyFramDesc.KeyFram_Movement = m_EffectDesc.KeyFram_4_Movement;
			}
			else if (m_EffectDesc.KeyFram_2_TimeEnd < m_InstanceDesc[i].fTime)
			{
				m_KeyFramDesc.KeyFram_TimeEnd = m_EffectDesc.KeyFram_3_TimeEnd;
				m_KeyFramDesc.KeyFram_RelaxSpeed = m_EffectDesc.KeyFram_3_RelaxSpeed;
				m_KeyFramDesc.KeyFram_Speed = m_EffectDesc.KeyFram_3_Speed;
				m_KeyFramDesc.KeyFram_fRotation = m_EffectDesc.KeyFram_3_fRotation;
				m_KeyFramDesc.KeyFram_fRelaxRotation = m_EffectDesc.KeyFram_3_fRelaxRotation;
				m_KeyFramDesc.KeyFram_Movement = m_EffectDesc.KeyFram_3_Movement;
			}
			else if (m_EffectDesc.KeyFram_1_TimeEnd < m_InstanceDesc[i].fTime)
			{
				m_KeyFramDesc.KeyFram_TimeEnd = m_EffectDesc.KeyFram_2_TimeEnd;
				m_KeyFramDesc.KeyFram_RelaxSpeed = m_EffectDesc.KeyFram_2_RelaxSpeed;
				m_KeyFramDesc.KeyFram_Speed = m_EffectDesc.KeyFram_2_Speed;
				m_KeyFramDesc.KeyFram_fRotation = m_EffectDesc.KeyFram_2_fRotation;
				m_KeyFramDesc.KeyFram_fRelaxRotation = m_EffectDesc.KeyFram_2_fRelaxRotation;
				m_KeyFramDesc.KeyFram_Movement = m_EffectDesc.KeyFram_2_Movement;
			}
			else if (0.f < m_InstanceDesc[i].fTime)
			{
				m_KeyFramDesc.KeyFram_TimeEnd = m_EffectDesc.KeyFram_1_TimeEnd;
				m_KeyFramDesc.KeyFram_RelaxSpeed = m_EffectDesc.KeyFram_1_RelaxSpeed;
				m_KeyFramDesc.KeyFram_Speed = m_EffectDesc.KeyFram_1_Speed;
				m_KeyFramDesc.KeyFram_fRotation = m_EffectDesc.KeyFram_1_fRotation;
				m_KeyFramDesc.KeyFram_fRelaxRotation = m_EffectDesc.KeyFram_1_fRelaxRotation;
				m_KeyFramDesc.KeyFram_Movement = m_EffectDesc.KeyFram_1_Movement;
			}





			m_InstanceDesc[i].fSpeed = m_InstanceDesc[i].fSpeed * m_KeyFramDesc.KeyFram_RelaxSpeed;
			m_InstanceDesc[i].fSpeed += m_KeyFramDesc.KeyFram_Speed * TimeDelta;

			vLook = XMLoadFloat4(&m_InstanceDesc[i].vMovement);

			XMStoreFloat4(&m_InstanceDesc[i].vMovement, XMVector3Normalize(vLook + XMLoadFloat4(&m_KeyFramDesc.KeyFram_Movement)));
			XMStoreFloat4(&((VTXMATRIX*)SubResource.pData)[i].vTranslation, XMLoadFloat4(&((VTXMATRIX*)SubResource.pData)[i].vTranslation) + (vLook * (m_InstanceDesc[i].fSpeed) * TimeDelta));
			vLook = (vLook * Size) + (vLook * (m_InstanceDesc[i].fSpeed * Size * Size_Speed));
			XMStoreFloat4(&((VTXMATRIX*)SubResource.pData)[i].vLook, vLook* m_InstanceDesc[i].fTime / Loop);
			XMStoreFloat4(&((VTXMATRIX*)SubResource.pData)[i].vRight, XMVector3Normalize(XMLoadFloat4(&((VTXMATRIX*)SubResource.pData)[i].vRight)) *Size * m_InstanceDesc[i].fTime / Loop);
			XMStoreFloat4(&((VTXMATRIX*)SubResource.pData)[i].vUp, XMVector3Normalize(XMLoadFloat4(&((VTXMATRIX*)SubResource.pData)[i].vUp)) *Size * m_InstanceDesc[i].fTime / Loop);

		}

		m_pDeviceContext->Unmap(m_pVBInstance, 0);
	}


	return false;
}

_bool CVIBuffer_Rect_Instance::Update(_double TimeDelta, _matrix CamMatrix)
{
	if (nullptr == m_pDeviceContext)
		return E_FAIL;

	m_fTime += TimeDelta;
	if (m_EffectDesc.fLoop == 0.f)
	{


		_vector		vLook;
		if (m_fMaxTime < m_fTime)
		{
			return true;
		}
		else if (m_EffectDesc.KeyFram_1_TimeEnd > m_fTime)
		{
			m_KeyFramDesc.KeyFram_TimeEnd = m_EffectDesc.KeyFram_1_TimeEnd;
			m_KeyFramDesc.KeyFram_RelaxSpeed = m_EffectDesc.KeyFram_1_RelaxSpeed;
			m_KeyFramDesc.KeyFram_Speed = m_EffectDesc.KeyFram_1_Speed;
			m_KeyFramDesc.KeyFram_fRotation = m_EffectDesc.KeyFram_1_fRotation;
			m_KeyFramDesc.KeyFram_fRelaxRotation = m_EffectDesc.KeyFram_1_fRelaxRotation;
			m_KeyFramDesc.KeyFram_Movement = m_EffectDesc.KeyFram_1_Movement;
		}
		else if (m_EffectDesc.KeyFram_2_TimeEnd > m_fTime)
		{
			m_KeyFramDesc.KeyFram_TimeEnd = m_EffectDesc.KeyFram_2_TimeEnd;
			m_KeyFramDesc.KeyFram_RelaxSpeed = m_EffectDesc.KeyFram_2_RelaxSpeed;
			m_KeyFramDesc.KeyFram_Speed = m_EffectDesc.KeyFram_2_Speed;
			m_KeyFramDesc.KeyFram_fRotation = m_EffectDesc.KeyFram_2_fRotation;
			m_KeyFramDesc.KeyFram_fRelaxRotation = m_EffectDesc.KeyFram_2_fRelaxRotation;
			m_KeyFramDesc.KeyFram_Movement = m_EffectDesc.KeyFram_2_Movement;
		}
		else if (m_EffectDesc.KeyFram_3_TimeEnd > m_fTime)
		{
			m_KeyFramDesc.KeyFram_TimeEnd = m_EffectDesc.KeyFram_3_TimeEnd;
			m_KeyFramDesc.KeyFram_RelaxSpeed = m_EffectDesc.KeyFram_3_RelaxSpeed;
			m_KeyFramDesc.KeyFram_Speed = m_EffectDesc.KeyFram_3_Speed;
			m_KeyFramDesc.KeyFram_fRotation = m_EffectDesc.KeyFram_3_fRotation;
			m_KeyFramDesc.KeyFram_fRelaxRotation = m_EffectDesc.KeyFram_3_fRelaxRotation;
			m_KeyFramDesc.KeyFram_Movement = m_EffectDesc.KeyFram_3_Movement;
		}
		else if (m_EffectDesc.KeyFram_4_TimeEnd > m_fTime)
		{
			m_KeyFramDesc.KeyFram_TimeEnd = m_EffectDesc.KeyFram_4_TimeEnd;
			m_KeyFramDesc.KeyFram_RelaxSpeed = m_EffectDesc.KeyFram_4_RelaxSpeed;
			m_KeyFramDesc.KeyFram_Speed = m_EffectDesc.KeyFram_4_Speed;
			m_KeyFramDesc.KeyFram_fRotation = m_EffectDesc.KeyFram_4_fRotation;
			m_KeyFramDesc.KeyFram_fRelaxRotation = m_EffectDesc.KeyFram_4_fRelaxRotation;
			m_KeyFramDesc.KeyFram_Movement = m_EffectDesc.KeyFram_4_Movement;
		}

		D3D11_MAPPED_SUBRESOURCE			SubResource;
		m_pDeviceContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);
		_float Size = m_EffectDesc.fScale;
		_float Size_Speed = m_EffectDesc.fScale_Speed;
		for (_uint i = 0; i < m_iNumInstance; ++i)
		{
			m_InstanceDesc[i].fSpeed = m_InstanceDesc[i].fSpeed * m_KeyFramDesc.KeyFram_RelaxSpeed;
			m_InstanceDesc[i].fSpeed += m_KeyFramDesc.KeyFram_Speed  * TimeDelta;

			vLook = XMLoadFloat4(&m_InstanceDesc[i].vMovement);
			XMStoreFloat4(&m_InstanceDesc[i].vMovement, XMVector3Normalize(vLook + XMLoadFloat4(&m_KeyFramDesc.KeyFram_Movement)));

			XMStoreFloat4(&((VTXMATRIX*)SubResource.pData)[i].vRight, CamMatrix.r[0] * Size);
			XMStoreFloat4(&((VTXMATRIX*)SubResource.pData)[i].vUp, CamMatrix.r[1] * Size);
			XMStoreFloat4(&((VTXMATRIX*)SubResource.pData)[i].vLook, CamMatrix.r[2] * Size);

			XMStoreFloat4(&((VTXMATRIX*)SubResource.pData)[i].vTranslation, XMLoadFloat4(&((VTXMATRIX*)SubResource.pData)[i].vTranslation) + (vLook * (m_InstanceDesc[i].fSpeed) * TimeDelta));

		}

		m_pDeviceContext->Unmap(m_pVBInstance, 0);
	}
	else
	{
		if (m_fMaxTime + m_EffectDesc.fLoop < m_fTime)
		{
			return true;
		}

		m_fTime += TimeDelta;
		_float Time = m_fMaxTime - m_fTime;
		_float Loop = m_EffectDesc.fLoop;



		D3D11_MAPPED_SUBRESOURCE			SubResource;
		m_pDeviceContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);
		_float Size = m_EffectDesc.fScale;
		_float Size_Speed = m_EffectDesc.fScale_Speed;
		_vector		vLook;
		for (_uint i = 0; i < m_iNumInstance; ++i)
		{
			m_InstanceDesc[i].fTime -= TimeDelta;

			if (0 > m_InstanceDesc[i].fTime)
			{
				if (m_EffectDesc.fLoop < Time - 1.f)
				{
					m_InstanceDesc[i].fSpeed = m_EffectDesc.fSpeed + (rand() % 100 * m_EffectDesc.fSpeedRand / 100);
					m_InstanceDesc[i].vMovement = _float4(m_EffectDesc.vMovement.x + (rand() % 100 * m_EffectDesc.vMovementRand.x / 100), m_EffectDesc.vMovement.y + (rand() % 100 * m_EffectDesc.vMovementRand.y / 100), m_EffectDesc.vMovement.z + (rand() % 100 * m_EffectDesc.vMovementRand.z / 100), 0.f);
					m_InstanceDesc[i].fTime = m_EffectDesc.fLoop;
					_vector		vLook = XMVector3Normalize(XMLoadFloat4(&m_InstanceDesc[i].vMovement));
					_vector		vAxis = XMVectorSet(0.f, 1.f, 0.f, 0.f);
					_vector		vRight = XMVector3Cross(vAxis, vLook);
					_vector		vUp = XMVector3Cross(vLook, vRight);
					_vector		vPos = XMVectorSet(m_EffectDesc.vPostion.x + (rand() % 100 * m_EffectDesc.vPostionRand.x / 100), m_EffectDesc.vPostion.y + (rand() % 100 * m_EffectDesc.vPostionRand.y / 100), m_EffectDesc.vPostion.z + (rand() % 100 * m_EffectDesc.vPostionRand.z / 100), 1.f);

					XMStoreFloat4(&m_InstanceDesc[i].vMovement, vLook);
					XMStoreFloat4(&((VTXMATRIX*)SubResource.pData)[i].vLook, vLook *(Size));
					XMStoreFloat4(&((VTXMATRIX*)SubResource.pData)[i].vRight, XMVector3Normalize(vRight) *Size);
					XMStoreFloat4(&((VTXMATRIX*)SubResource.pData)[i].vUp, XMVector3Normalize(vUp) *Size);
					XMStoreFloat4(&((VTXMATRIX*)SubResource.pData)[i].vTranslation, vPos + vLook* m_EffectDesc.fLastTime);

					m_KeyFramDesc.KeyFram_TimeEnd = m_EffectDesc.KeyFram_4_TimeEnd;
					m_KeyFramDesc.KeyFram_RelaxSpeed = m_EffectDesc.KeyFram_4_RelaxSpeed;
					m_KeyFramDesc.KeyFram_Speed = m_EffectDesc.KeyFram_4_Speed;
					m_KeyFramDesc.KeyFram_fRotation = m_EffectDesc.KeyFram_4_fRotation;
					m_KeyFramDesc.KeyFram_fRelaxRotation = m_EffectDesc.KeyFram_4_fRelaxRotation;
					m_KeyFramDesc.KeyFram_Movement = m_EffectDesc.KeyFram_4_Movement;


				}
				else
				{
					m_InstanceDesc[i].fTime = 0.f;

				}

			}
			else if (m_EffectDesc.KeyFram_3_TimeEnd < m_InstanceDesc[i].fTime)
			{
				m_KeyFramDesc.KeyFram_TimeEnd = m_EffectDesc.KeyFram_4_TimeEnd;
				m_KeyFramDesc.KeyFram_RelaxSpeed = m_EffectDesc.KeyFram_4_RelaxSpeed;
				m_KeyFramDesc.KeyFram_Speed = m_EffectDesc.KeyFram_4_Speed;
				m_KeyFramDesc.KeyFram_fRotation = m_EffectDesc.KeyFram_4_fRotation;
				m_KeyFramDesc.KeyFram_fRelaxRotation = m_EffectDesc.KeyFram_4_fRelaxRotation;
				m_KeyFramDesc.KeyFram_Movement = m_EffectDesc.KeyFram_4_Movement;
			}
			else if (m_EffectDesc.KeyFram_2_TimeEnd < m_InstanceDesc[i].fTime)
			{
				m_KeyFramDesc.KeyFram_TimeEnd = m_EffectDesc.KeyFram_3_TimeEnd;
				m_KeyFramDesc.KeyFram_RelaxSpeed = m_EffectDesc.KeyFram_3_RelaxSpeed;
				m_KeyFramDesc.KeyFram_Speed = m_EffectDesc.KeyFram_3_Speed;
				m_KeyFramDesc.KeyFram_fRotation = m_EffectDesc.KeyFram_3_fRotation;
				m_KeyFramDesc.KeyFram_fRelaxRotation = m_EffectDesc.KeyFram_3_fRelaxRotation;
				m_KeyFramDesc.KeyFram_Movement = m_EffectDesc.KeyFram_3_Movement;
			}
			else if (m_EffectDesc.KeyFram_1_TimeEnd < m_InstanceDesc[i].fTime)
			{
				m_KeyFramDesc.KeyFram_TimeEnd = m_EffectDesc.KeyFram_2_TimeEnd;
				m_KeyFramDesc.KeyFram_RelaxSpeed = m_EffectDesc.KeyFram_2_RelaxSpeed;
				m_KeyFramDesc.KeyFram_Speed = m_EffectDesc.KeyFram_2_Speed;
				m_KeyFramDesc.KeyFram_fRotation = m_EffectDesc.KeyFram_2_fRotation;
				m_KeyFramDesc.KeyFram_fRelaxRotation = m_EffectDesc.KeyFram_2_fRelaxRotation;
				m_KeyFramDesc.KeyFram_Movement = m_EffectDesc.KeyFram_2_Movement;
			}
			else if (0.f < m_InstanceDesc[i].fTime)
			{
				m_KeyFramDesc.KeyFram_TimeEnd = m_EffectDesc.KeyFram_1_TimeEnd;
				m_KeyFramDesc.KeyFram_RelaxSpeed = m_EffectDesc.KeyFram_1_RelaxSpeed;
				m_KeyFramDesc.KeyFram_Speed = m_EffectDesc.KeyFram_1_Speed;
				m_KeyFramDesc.KeyFram_fRotation = m_EffectDesc.KeyFram_1_fRotation;
				m_KeyFramDesc.KeyFram_fRelaxRotation = m_EffectDesc.KeyFram_1_fRelaxRotation;
				m_KeyFramDesc.KeyFram_Movement = m_EffectDesc.KeyFram_1_Movement;
			}

			m_InstanceDesc[i].fSpeed = m_InstanceDesc[i].fSpeed * m_KeyFramDesc.KeyFram_RelaxSpeed;
			m_InstanceDesc[i].fSpeed += m_KeyFramDesc.KeyFram_Speed * TimeDelta;

			vLook = XMLoadFloat4(&m_InstanceDesc[i].vMovement);
			XMStoreFloat4(&m_InstanceDesc[i].vMovement, XMVector3Normalize(vLook + XMLoadFloat4(&m_KeyFramDesc.KeyFram_Movement)));
			XMStoreFloat4(&((VTXMATRIX*)SubResource.pData)[i].vTranslation, XMLoadFloat4(&((VTXMATRIX*)SubResource.pData)[i].vTranslation) + (vLook * (m_InstanceDesc[i].fSpeed) * TimeDelta));

			XMStoreFloat4(&((VTXMATRIX*)SubResource.pData)[i].vRight, CamMatrix.r[0] * Size * m_InstanceDesc[i].fTime / Loop);
			XMStoreFloat4(&((VTXMATRIX*)SubResource.pData)[i].vUp, CamMatrix.r[1] * Size * m_InstanceDesc[i].fTime / Loop);
			XMStoreFloat4(&((VTXMATRIX*)SubResource.pData)[i].vLook, CamMatrix.r[2] * Size * m_InstanceDesc[i].fTime / Loop);

		}
		m_pDeviceContext->Unmap(m_pVBInstance, 0);
	}


	return false;
}

HRESULT CVIBuffer_Rect_Instance::Reset()
{
	if (nullptr == m_pDeviceContext)
		return E_FAIL;


	m_iNumInstance = m_EffectDesc.iNum;
	Safe_Release(m_pVBInstance);
	Safe_Release(m_pIB);

	if (m_EffectDesc.bBillboard == m_bBillboard)
	{

	}
	else if (m_EffectDesc.bBillboard)
	{
		m_bBillboard = m_EffectDesc.bBillboard;
		Safe_Release(m_pVB);

#pragma region VERTEX_BUFFER
		m_iStride = sizeof(VTXTEX);
		m_iNumVertices = 4;
		m_iNumVBuffers = 2;

		ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));
		m_VBDesc.ByteWidth = m_iStride * m_iNumVertices;
		m_VBDesc.StructureByteStride = m_iStride;
		m_VBDesc.Usage = D3D11_USAGE_IMMUTABLE;
		m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		m_VBDesc.CPUAccessFlags = 0;
		m_VBDesc.MiscFlags = 0;
		VTXTEX*		pVertices = new VTXTEX[m_iNumVertices];

		pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.f);
		pVertices[0].vTexUV = _float2(0.0f, 0.0f);

		pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.f);
		pVertices[1].vTexUV = _float2(1.0f, 0.0f);

		pVertices[2].vPosition = _float3(0.5f, -0.5f, 0.f);
		pVertices[2].vTexUV = _float2(1.0f, 1.0f);

		pVertices[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
		pVertices[3].vTexUV = _float2(0.0f, 1.0f);


		ZeroMemory(&m_VBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
		m_VBSubResourceData.pSysMem = pVertices;

		if (FAILED(__super::Create_VertexBuffer()))
			return E_FAIL;

		Safe_Delete_Array(pVertices);
#pragma endregion
	}
	else
	{
		m_bBillboard = m_EffectDesc.bBillboard;
		Safe_Release(m_pVB);

#pragma region VERTEX_BUFFER
		m_iStride = sizeof(VTXTEX);
		m_iNumVertices = 4;
		m_iNumVBuffers = 2;

		ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));
		m_VBDesc.ByteWidth = m_iStride * m_iNumVertices;
		m_VBDesc.StructureByteStride = m_iStride;
		m_VBDesc.Usage = D3D11_USAGE_IMMUTABLE;
		m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		m_VBDesc.CPUAccessFlags = 0;
		m_VBDesc.MiscFlags = 0;
		VTXTEX*		pVertices = new VTXTEX[m_iNumVertices];

		pVertices[0].vPosition = _float3(0.f, 0.5f, -0.5f);
		pVertices[0].vTexUV = _float2(0.0f, 0.0f);

		pVertices[1].vPosition = _float3(0.f, 0.5f, 0.5f);
		pVertices[1].vTexUV = _float2(1.0f, 0.0f);

		pVertices[2].vPosition = _float3(0.f, -0.5f, 0.5f);
		pVertices[2].vTexUV = _float2(1.0f, 1.0f);

		pVertices[3].vPosition = _float3(0.f, -0.5f, -0.5f);
		pVertices[3].vTexUV = _float2(0.0f, 1.0f);


		ZeroMemory(&m_VBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
		m_VBSubResourceData.pSysMem = pVertices;

		if (FAILED(__super::Create_VertexBuffer()))
			return E_FAIL;

		Safe_Delete_Array(pVertices);
#pragma endregion



	}





#pragma region INSTANCE_BUFFER

	m_iInstanceStride = sizeof(VTXMATRIX);


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
		pInstanceVertices[i].vTranslation = _float4(rand() % 3, 10.0f, rand() % 3, 1.f);
	}

	ZeroMemory(&m_VBInstSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_VBInstSubResourceData.pSysMem = pInstanceVertices;

	if (FAILED(m_pDevice->CreateBuffer(&m_VBInstDesc, &m_VBInstSubResourceData, &m_pVBInstance)))
		return E_FAIL;

	Safe_Delete_Array(pInstanceVertices);


#pragma endregion

#pragma region INDEX_BUFFER

	m_iPrimitiveIndicesSize = sizeof(FACELISTINDICES16);
	m_iNumPrimitive = 2 * m_iNumInstance;
	m_iNumIndicesPerPrimitive = 3;
	m_eFormat = DXGI_FORMAT_R16_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;


	ZeroMemory(&m_IBDesc, sizeof(D3D11_BUFFER_DESC));
	m_IBDesc.ByteWidth = m_iPrimitiveIndicesSize * m_iNumPrimitive;
	m_IBDesc.StructureByteStride = 0;
	m_IBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_IBDesc.CPUAccessFlags = 0;
	m_IBDesc.MiscFlags = 0;

	FACELISTINDICES16*		pIndices = new FACELISTINDICES16[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(FACELISTINDICES16) * m_iNumPrimitive);

	_uint		iNumFaces = 0;

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		pIndices[iNumFaces]._0 = 0;
		pIndices[iNumFaces]._1 = 1;
		pIndices[iNumFaces]._2 = 2;
		++iNumFaces;

		pIndices[iNumFaces]._0 = 0;
		pIndices[iNumFaces]._1 = 2;
		pIndices[iNumFaces]._2 = 3;
		++iNumFaces;
	}

	ZeroMemory(&m_IBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_IBSubResourceData.pSysMem = pIndices;

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;
#pragma endregion

	Safe_Delete_Array(pIndices);

	m_InstanceDesc = new INSTANCEDESC[m_iNumInstance];
	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		m_InstanceDesc[i].fSpeed = 1.f;
		m_InstanceDesc[i].vMovement = _float4(0.f, 0.f, 1.f, 0.f);
	}




	Safe_Delete_Array(pIndices);
	D3D11_MAPPED_SUBRESOURCE			SubResource;

	m_fTime = 0.f;

	m_pDeviceContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	_float Size = m_EffectDesc.fScale;
	if (m_bBillboard)
	{
		_matrix Mx = XMMatrixIdentity();

		for (_uint i = 0; i < m_iNumInstance; ++i)
		{
			m_InstanceDesc[i].fSpeed = m_EffectDesc.fSpeed + (rand() % 100 * m_EffectDesc.fSpeedRand / 100);
			m_InstanceDesc[i].vMovement = _float4(m_EffectDesc.vMovement.x + (rand() % 100 * m_EffectDesc.vMovementRand.x / 100), m_EffectDesc.vMovement.y + (rand() % 100 * m_EffectDesc.vMovementRand.y / 100), m_EffectDesc.vMovement.z + (rand() % 100 * m_EffectDesc.vMovementRand.z / 100), 0.f);
			m_InstanceDesc[i].fTime = rand() % 100 * (m_EffectDesc.fLoop / 100);

			_vector		vLook = XMVector3Normalize(XMLoadFloat4(&m_InstanceDesc[i].vMovement));
			_vector		vPos = XMVectorSet(m_EffectDesc.vPostion.x + (rand() % 100 * m_EffectDesc.vPostionRand.x / 100), m_EffectDesc.vPostion.y + (rand() % 100 * m_EffectDesc.vPostionRand.y / 100), m_EffectDesc.vPostion.z + (rand() % 100 * m_EffectDesc.vPostionRand.z / 100), 1.f);

			XMStoreFloat4(&m_InstanceDesc[i].vMovement, vLook);
			XMStoreFloat4(&((VTXMATRIX*)SubResource.pData)[i].vLook, Mx.r[2] * (Size));
			XMStoreFloat4(&((VTXMATRIX*)SubResource.pData)[i].vRight, Mx.r[0] * Size);
			XMStoreFloat4(&((VTXMATRIX*)SubResource.pData)[i].vUp, Mx.r[1] * Size);
			XMStoreFloat4(&((VTXMATRIX*)SubResource.pData)[i].vTranslation, vPos + vLook* m_EffectDesc.fLastTime);

		}
	}
	else
	{
		for (_uint i = 0; i < m_iNumInstance; ++i)
		{
			m_InstanceDesc[i].fSpeed = m_EffectDesc.fSpeed + (rand() % 100 * m_EffectDesc.fSpeedRand / 100);
			m_InstanceDesc[i].vMovement = _float4(m_EffectDesc.vMovement.x + (rand() % 100 * m_EffectDesc.vMovementRand.x / 100), m_EffectDesc.vMovement.y + (rand() % 100 * m_EffectDesc.vMovementRand.y / 100), m_EffectDesc.vMovement.z + (rand() % 100 * m_EffectDesc.vMovementRand.z / 100), 0.f);
			m_InstanceDesc[i].fTime = rand() % 100 * (m_EffectDesc.fLoop / 100);
			_vector		vLook = XMVector3Normalize(XMLoadFloat4(&m_InstanceDesc[i].vMovement));
			_vector		vAxis = XMVectorSet(0.f, 1.f, 0.f, 0.f);
			_vector		vRight = XMVector3Cross(vAxis, vLook);
			_vector		vUp = XMVector3Cross(vLook, vRight);
			_vector		vPos = XMVectorSet(m_EffectDesc.vPostion.x + (rand() % 100 * m_EffectDesc.vPostionRand.x / 100), m_EffectDesc.vPostion.y + (rand() % 100 * m_EffectDesc.vPostionRand.y / 100), m_EffectDesc.vPostion.z + (rand() % 100 * m_EffectDesc.vPostionRand.z / 100), 1.f);

			XMStoreFloat4(&m_InstanceDesc[i].vMovement, vLook);
			XMStoreFloat4(&((VTXMATRIX*)SubResource.pData)[i].vLook, vLook *(Size));
			XMStoreFloat4(&((VTXMATRIX*)SubResource.pData)[i].vRight, XMVector3Normalize(vRight) *Size);
			XMStoreFloat4(&((VTXMATRIX*)SubResource.pData)[i].vUp, XMVector3Normalize(vUp) *Size);
			XMStoreFloat4(&((VTXMATRIX*)SubResource.pData)[i].vTranslation, vPos + vLook* m_EffectDesc.fLastTime);

		}
	}


	m_pDeviceContext->Unmap(m_pVBInstance, 0);

	if (m_EffectDesc.fLoop > 0)
	{
		m_fMaxTime = m_EffectDesc.fMaxTime;
		m_EffectDesc.KeyFram_1_TimeEnd = (m_EffectDesc.fLoop * 1) / 4;
		m_EffectDesc.KeyFram_2_TimeEnd = (m_EffectDesc.fLoop * 2) / 4;
		m_EffectDesc.KeyFram_3_TimeEnd = (m_EffectDesc.fLoop * 3) / 4;
		m_EffectDesc.KeyFram_4_TimeEnd = (m_EffectDesc.fLoop * 4) / 4;

	}
	else
	{
		m_fMaxTime = m_EffectDesc.fMaxTime;
		m_EffectDesc.KeyFram_1_TimeEnd = (m_EffectDesc.fMaxTime * 1) / 4;
		m_EffectDesc.KeyFram_2_TimeEnd = (m_EffectDesc.fMaxTime * 2) / 4;
		m_EffectDesc.KeyFram_3_TimeEnd = (m_EffectDesc.fMaxTime * 3) / 4;
		m_EffectDesc.KeyFram_4_TimeEnd = (m_EffectDesc.fMaxTime * 4) / 4;
	}

	return S_OK;
}

HRESULT CVIBuffer_Rect_Instance::KeyFramReset()
{
	if (m_EffectDesc.fLoop > 0)
	{
		m_fMaxTime = m_EffectDesc.fMaxTime;

		m_EffectDesc.KeyFram_1_TimeEnd = (m_EffectDesc.fLoop * 1) / 4;
		m_EffectDesc.KeyFram_1_RelaxSpeed = 1.f;
		m_EffectDesc.KeyFram_1_Speed = 0.f;
		m_EffectDesc.KeyFram_1_fRotation = 1.f;
		m_EffectDesc.KeyFram_1_fRelaxRotation = 0.f;

		m_EffectDesc.KeyFram_2_TimeEnd = (m_EffectDesc.fLoop * 2) / 4;
		m_EffectDesc.KeyFram_2_RelaxSpeed = 1.f;
		m_EffectDesc.KeyFram_2_Speed = 0.f;
		m_EffectDesc.KeyFram_2_fRotation = 1.f;
		m_EffectDesc.KeyFram_2_fRelaxRotation = 0.f;

		m_EffectDesc.KeyFram_3_TimeEnd = (m_EffectDesc.fLoop * 3) / 4;
		m_EffectDesc.KeyFram_3_RelaxSpeed = 1.f;
		m_EffectDesc.KeyFram_3_Speed = 0.f;
		m_EffectDesc.KeyFram_3_fRotation = 1.f;
		m_EffectDesc.KeyFram_3_fRelaxRotation = 0.f;

		m_EffectDesc.KeyFram_4_TimeEnd = (m_EffectDesc.fLoop * 4) / 4;
		m_EffectDesc.KeyFram_4_RelaxSpeed = 1.f;
		m_EffectDesc.KeyFram_4_Speed = 0.f;
		m_EffectDesc.KeyFram_4_fRotation = 1.f;
		m_EffectDesc.KeyFram_4_fRelaxRotation = 0.f;

		m_EffectDesc.KeyFram_1_Movement = _float4(0.f, 0.f, 0.f, 0.f);
		m_EffectDesc.KeyFram_2_Movement = _float4(0.f, 0.f, 0.f, 0.f);
		m_EffectDesc.KeyFram_3_Movement = _float4(0.f, 0.f, 0.f, 0.f);
		m_EffectDesc.KeyFram_4_Movement = _float4(0.f, 0.f, 0.f, 0.f);
	}
	else
	{
		m_fMaxTime = m_EffectDesc.fMaxTime;

		m_EffectDesc.KeyFram_1_TimeEnd = (m_EffectDesc.fMaxTime * 1) / 4;
		m_EffectDesc.KeyFram_1_RelaxSpeed = 1.f;
		m_EffectDesc.KeyFram_1_Speed = 0.f;
		m_EffectDesc.KeyFram_1_fRotation = 1.f;
		m_EffectDesc.KeyFram_1_fRelaxRotation = 0.f;

		m_EffectDesc.KeyFram_2_TimeEnd = (m_EffectDesc.fMaxTime * 2) / 4;
		m_EffectDesc.KeyFram_2_RelaxSpeed = 1.f;
		m_EffectDesc.KeyFram_2_Speed = 0.f;
		m_EffectDesc.KeyFram_2_fRotation = 1.f;
		m_EffectDesc.KeyFram_2_fRelaxRotation = 0.f;

		m_EffectDesc.KeyFram_3_TimeEnd = (m_EffectDesc.fMaxTime * 3) / 4;
		m_EffectDesc.KeyFram_3_RelaxSpeed = 1.f;
		m_EffectDesc.KeyFram_3_Speed = 0.f;
		m_EffectDesc.KeyFram_3_fRotation = 1.f;
		m_EffectDesc.KeyFram_3_fRelaxRotation = 0.f;

		m_EffectDesc.KeyFram_4_TimeEnd = (m_EffectDesc.fMaxTime * 4) / 4;
		m_EffectDesc.KeyFram_4_RelaxSpeed = 1.f;
		m_EffectDesc.KeyFram_4_Speed = 0.f;
		m_EffectDesc.KeyFram_4_fRotation = 1.f;
		m_EffectDesc.KeyFram_4_fRelaxRotation = 0.f;

		m_EffectDesc.KeyFram_1_Movement = _float4(0.f, 0.f, 0.f, 0.f);
		m_EffectDesc.KeyFram_2_Movement = _float4(0.f, 0.f, 0.f, 0.f);
		m_EffectDesc.KeyFram_3_Movement = _float4(0.f, 0.f, 0.f, 0.f);
		m_EffectDesc.KeyFram_4_Movement = _float4(0.f, 0.f, 0.f, 0.f);
	}

	return S_OK;
}

HRESULT CVIBuffer_Rect_Instance::Restart()
{
	if (nullptr == m_pDeviceContext)
		return E_FAIL;

	D3D11_MAPPED_SUBRESOURCE			SubResource;

	_vector		vPos;
	m_fTime = 0.f;



	m_pDeviceContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	_float Size = m_EffectDesc.fScale;

	if (m_bBillboard)
	{
		_matrix Mx = XMMatrixIdentity();

		for (_uint i = 0; i < m_iNumInstance; ++i)
		{
			m_InstanceDesc[i].fSpeed = m_EffectDesc.fSpeed + (rand() % 100 * m_EffectDesc.fSpeedRand / 100);
			m_InstanceDesc[i].vMovement = _float4(m_EffectDesc.vMovement.x + (rand() % 100 * m_EffectDesc.vMovementRand.x / 100), m_EffectDesc.vMovement.y + (rand() % 100 * m_EffectDesc.vMovementRand.y / 100), m_EffectDesc.vMovement.z + (rand() % 100 * m_EffectDesc.vMovementRand.z / 100), 0.f);
			m_InstanceDesc[i].fTime = rand() % 100 * (m_EffectDesc.fLoop / 100);

			_vector		vLook = XMVector3Normalize(XMLoadFloat4(&m_InstanceDesc[i].vMovement));
			_vector		vPos = XMVectorSet(m_EffectDesc.vPostion.x + (rand() % 100 * m_EffectDesc.vPostionRand.x / 100), m_EffectDesc.vPostion.y + (rand() % 100 * m_EffectDesc.vPostionRand.y / 100), m_EffectDesc.vPostion.z + (rand() % 100 * m_EffectDesc.vPostionRand.z / 100), 1.f);

			XMStoreFloat4(&m_InstanceDesc[i].vMovement, vLook);
			XMStoreFloat4(&((VTXMATRIX*)SubResource.pData)[i].vLook, Mx.r[2] * (Size));
			XMStoreFloat4(&((VTXMATRIX*)SubResource.pData)[i].vRight, Mx.r[0] * Size);
			XMStoreFloat4(&((VTXMATRIX*)SubResource.pData)[i].vUp, Mx.r[1] * Size);
			XMStoreFloat4(&((VTXMATRIX*)SubResource.pData)[i].vTranslation, vPos + vLook* m_EffectDesc.fLastTime);

		}
	}
	else
	{
		for (_uint i = 0; i < m_iNumInstance; ++i)
		{
			m_InstanceDesc[i].fSpeed = m_EffectDesc.fSpeed + (rand() % 100 * m_EffectDesc.fSpeedRand / 100);
			m_InstanceDesc[i].vMovement = _float4(m_EffectDesc.vMovement.x + (rand() % 100 * m_EffectDesc.vMovementRand.x / 100), m_EffectDesc.vMovement.y + (rand() % 100 * m_EffectDesc.vMovementRand.y / 100), m_EffectDesc.vMovement.z + (rand() % 100 * m_EffectDesc.vMovementRand.z / 100), 0.f);
			m_InstanceDesc[i].fTime = rand() % 100 * (m_EffectDesc.fLoop / 100);
			_vector		vLook = XMVector3Normalize(XMLoadFloat4(&m_InstanceDesc[i].vMovement));
			_vector		vAxis = XMVectorSet(0.f, 1.f, 0.f, 0.f);
			_vector		vRight = XMVector3Cross(vAxis, vLook);
			_vector		vUp = XMVector3Cross(vLook, vRight);
			_vector		vPos = XMVectorSet(m_EffectDesc.vPostion.x + (rand() % 100 * m_EffectDesc.vPostionRand.x / 100), m_EffectDesc.vPostion.y + (rand() % 100 * m_EffectDesc.vPostionRand.y / 100), m_EffectDesc.vPostion.z + (rand() % 100 * m_EffectDesc.vPostionRand.z / 100), 1.f);

			XMStoreFloat4(&m_InstanceDesc[i].vMovement, vLook);
			XMStoreFloat4(&((VTXMATRIX*)SubResource.pData)[i].vLook, vLook *(Size));
			XMStoreFloat4(&((VTXMATRIX*)SubResource.pData)[i].vRight, XMVector3Normalize(vRight) *Size);
			XMStoreFloat4(&((VTXMATRIX*)SubResource.pData)[i].vUp, XMVector3Normalize(vUp) *Size);
			XMStoreFloat4(&((VTXMATRIX*)SubResource.pData)[i].vTranslation, vPos + vLook* m_EffectDesc.fLastTime);

		}
	}

	m_pDeviceContext->Unmap(m_pVBInstance, 0);



	if (m_EffectDesc.fLoop > 0)
	{
		m_fMaxTime = m_EffectDesc.fMaxTime;
		m_EffectDesc.KeyFram_1_TimeEnd = (m_EffectDesc.fLoop * 1) / 4;
		m_EffectDesc.KeyFram_2_TimeEnd = (m_EffectDesc.fLoop * 2) / 4;
		m_EffectDesc.KeyFram_3_TimeEnd = (m_EffectDesc.fLoop * 3) / 4;
		m_EffectDesc.KeyFram_4_TimeEnd = (m_EffectDesc.fLoop * 4) / 4;

	}
	else
	{
		m_fMaxTime = m_EffectDesc.fMaxTime;
		m_EffectDesc.KeyFram_1_TimeEnd = (m_EffectDesc.fMaxTime * 1) / 4;
		m_EffectDesc.KeyFram_2_TimeEnd = (m_EffectDesc.fMaxTime * 2) / 4;
		m_EffectDesc.KeyFram_3_TimeEnd = (m_EffectDesc.fMaxTime * 3) / 4;
		m_EffectDesc.KeyFram_4_TimeEnd = (m_EffectDesc.fMaxTime * 4) / 4;
	}
}

HRESULT CVIBuffer_Rect_Instance::Render()
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

	m_pDeviceContext->DrawIndexedInstanced(6, m_iNumInstance, 0, 0, 0);

	return S_OK;
}

CVIBuffer_Rect_Instance * CVIBuffer_Rect_Instance::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _uint iNumInstance)
{
	CVIBuffer_Rect_Instance*	pInstance = new CVIBuffer_Rect_Instance(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(iNumInstance)))
	{
		MSG_BOX(TEXT("Failed to Created CVIBuffer_Rect_Instance"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_Rect_Instance::Clone(void * pArg)
{
	CVIBuffer_Rect_Instance*	pInstance = new CVIBuffer_Rect_Instance(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CVIBuffer_Rect_Instance"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Rect_Instance::Free()
{
	__super::Free();

	if (false == m_isCloned)
		Safe_Delete_Array(m_InstanceDesc);



	Safe_Release(m_pVBInstance);

}
