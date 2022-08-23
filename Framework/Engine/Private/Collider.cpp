#include "..\Public\Collider.h"
#include "PipeLine.h"

CCollider::CCollider(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
{

}

CCollider::CCollider(const CCollider & rhs)
	: CComponent(rhs)
	, m_eType(rhs.m_eType)
	, m_pBasicEffect(rhs.m_pBasicEffect)
	, m_pBatch(rhs.m_pBatch)
	, m_pInputLayout(rhs.m_pInputLayout)
{
	Safe_AddRef(m_pInputLayout);
}

HRESULT CCollider::NativeConstruct_Prototype(TYPE eType)
{
	m_eType = eType;

	m_pBasicEffect = new BasicEffect(m_pDevice);
	m_pBasicEffect->SetVertexColorEnabled(true);

	const void*		pShaderByteCode = nullptr;
	size_t		ShaderByteCodeLength = 0;

	m_pBasicEffect->GetVertexShaderBytecode(&pShaderByteCode, &ShaderByteCodeLength);

	m_pBatch = new PrimitiveBatch<VertexPositionColor>(m_pDeviceContext);

	if (FAILED(m_pDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, pShaderByteCode, ShaderByteCodeLength, &m_pInputLayout)))
		return E_FAIL;

	return S_OK;
}

HRESULT CCollider::NativeConstruct(void * pArg)
{
	COLLIDERDESC		ColliderDesc;

	if (nullptr != pArg)
		memcpy(&ColliderDesc, pArg, sizeof(COLLIDERDESC));

	else
	{
		ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
		ColliderDesc.vSize = _float3(1.f, 1.f, 1.f);
		ColliderDesc.fRadius = 1.f;
	}	
	
	switch (m_eType)
	{
	case CCollider::TYPE_AABB:
		m_pAABB = new BoundingBox(ColliderDesc.vPosition, _float3(ColliderDesc.vSize.x * 0.5f, ColliderDesc.vSize.y * 0.5f, ColliderDesc.vSize.z * 0.5f));
		m_pAABBInWorld = new BoundingBox();
		break;
	case CCollider::TYPE_OBB:
		m_pOBB = new BoundingOrientedBox(ColliderDesc.vPosition, _float3(ColliderDesc.vSize.x * 0.5f, ColliderDesc.vSize.y * 0.5f, ColliderDesc.vSize.z * 0.5f), _float4(0.f, 0.f, 0.f, 1.f));
		m_pOBBInWorld = new BoundingOrientedBox();
		break;
	case CCollider::TYPE_SPHERE:
		m_pSPHERE = new BoundingSphere(ColliderDesc.vPosition, ColliderDesc.fRadius);
		m_pSPHEREInWorld = new BoundingSphere();
		break;
	}

	return S_OK;
}

_bool CCollider::Collsion_Sphere(CCollider * pTargetCollider, _float* pCollisionDistance,_vector* vDir)
{
	if (TYPE_SPHERE == pTargetCollider->m_eType)
		m_isCollision = m_pSPHEREInWorld->Intersects(*pTargetCollider->m_pSPHEREInWorld);

	if (pTargetCollider->Get_SPHEREInWolrd() == nullptr)
	{
		return m_isCollision;
	}

	if (pCollisionDistance == nullptr)
	{
		return m_isCollision;
	}

	if (m_isCollision)
	{
		// 두 점의y좌표가같다면
		_vector vSourPos = XMLoadFloat3(&m_pSPHEREInWorld->Center);
		_float vSourRadius = m_pSPHEREInWorld->Radius;
		_vector vDestPos = XMLoadFloat3(&pTargetCollider->Get_SPHEREInWolrd()->Center);
		_float vDestRadius = pTargetCollider->Get_SPHEREInWolrd()->Radius;
		_vector vSourDir = XMVector3Normalize(vDestPos - vSourPos)*vDestRadius;
		_vector vDestDir = XMVector3Normalize(vSourPos - vDestPos)*vSourRadius;
		_float fDistance = XMVectorGetX(XMVector3Length(vDestPos - vSourPos));
		fDistance = vSourRadius + vDestRadius - fDistance;

		//_vector vTargetPos = vDestPos + vDestDir;
		//

		//XMStoreFloat3(vCollisionDir, vTargetPos);
		*pCollisionDistance = fDistance;
		
		*vDir = XMVector3Normalize(vDestPos - vSourPos);
	}

	return m_isCollision;
}


_bool CCollider::Collsion_AABB(CCollider * pTargetCollider)
{
	if (TYPE_AABB == pTargetCollider->m_eType)
	{
		_vector		vSourMin = Compute_Min();
		_vector		vSourMax = Compute_Max();

		_vector		vDestMin = pTargetCollider->Compute_Min();
		_vector		vDestMax = pTargetCollider->Compute_Max();

		m_isCollision = false;

		if (max(XMVectorGetX(vSourMin), XMVectorGetX(vDestMin))
			> min(XMVectorGetX(vSourMax), XMVectorGetX(vDestMax)))
			return false;

		if (max(XMVectorGetY(vSourMin), XMVectorGetY(vDestMin))
			> min(XMVectorGetY(vSourMax), XMVectorGetY(vDestMax)))
			return false;

		if (max(XMVectorGetZ(vSourMin), XMVectorGetZ(vDestMin))
			> min(XMVectorGetZ(vSourMax), XMVectorGetZ(vDestMax)))
			return false;

		m_isCollision = true;

		return true;
	}	

	if (TYPE_OBB == pTargetCollider->m_eType)
		m_isCollision = m_pAABBInWorld->Intersects(*pTargetCollider->m_pOBBInWorld);

	if (TYPE_SPHERE == pTargetCollider->m_eType)
		m_isCollision = m_pAABBInWorld->Intersects(*pTargetCollider->m_pSPHEREInWorld);	

	return m_isCollision;
}

_bool CCollider::Collsion_OBB(CCollider * pTargetCollider)
{
	OBBDESC		OBBDesc[2] = {
		Compute_OBB(), 
		pTargetCollider->Compute_OBB()
	};	

	m_isCollision = false;

	_float		fDistance[3];

	for (_uint i = 0; i < 2; ++i)
	{
		for (_uint j = 0; j < 3; ++j)
		{
			_vector		vCenterDir = XMLoadFloat3(&OBBDesc[1].vCenter) - XMLoadFloat3(&OBBDesc[0].vCenter);
			fDistance[0] = fabs(XMVectorGetX(XMVector3Dot(vCenterDir, XMLoadFloat3(&OBBDesc[i].vAlignAxis[j]))));

			fDistance[1] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[0].vCenterAxis[0]), XMLoadFloat3(&OBBDesc[i].vAlignAxis[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[0].vCenterAxis[1]), XMLoadFloat3(&OBBDesc[i].vAlignAxis[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[0].vCenterAxis[2]), XMLoadFloat3(&OBBDesc[i].vAlignAxis[j]))));

			fDistance[2] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenterAxis[0]), XMLoadFloat3(&OBBDesc[i].vAlignAxis[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenterAxis[1]), XMLoadFloat3(&OBBDesc[i].vAlignAxis[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenterAxis[2]), XMLoadFloat3(&OBBDesc[i].vAlignAxis[j]))));

			if (fDistance[0] > fDistance[1] + fDistance[2])
				return false;
		}
	}

	m_isCollision = true;

	return true;
}

_bool CCollider::Collision_Lay(_fvector vRayPos, _fvector vRayDir, _float & fDist)
{
	_bool bCollision = false;
	switch (m_eType)
	{
	case TYPE_AABB:
		bCollision = m_pAABBInWorld->Intersects(vRayPos, vRayDir, fDist);
		break;
	case TYPE_OBB:
		bCollision = m_pOBBInWorld->Intersects(vRayPos, vRayDir, fDist);
		break;
	case TYPE_SPHERE:
		bCollision = m_pSPHEREInWorld->Intersects(vRayPos, vRayDir, fDist);
		break;
	}

	return bCollision;
}

_bool CCollider::Collision(CCollider * pTargetCollider)
{
	_bool bCollision = false;
	switch (m_eType)
	{
	case TYPE_AABB:
		switch (pTargetCollider->m_eType)
		{
		case TYPE_AABB:
			bCollision = m_pAABBInWorld->Intersects(*(pTargetCollider->m_pAABBInWorld));
			break;
		case TYPE_OBB:
			bCollision = m_pAABBInWorld->Intersects(*(pTargetCollider->m_pOBBInWorld));
			break;
		case TYPE_SPHERE:
			bCollision = m_pAABBInWorld->Intersects(*(pTargetCollider->m_pSPHEREInWorld));
			break;
		}
		break;
	case TYPE_OBB:
		switch (pTargetCollider->m_eType)
		{
		case TYPE_AABB:
			bCollision = m_pOBBInWorld->Intersects(*(pTargetCollider->m_pAABBInWorld));
			break;
		case TYPE_OBB:
			bCollision = m_pOBBInWorld->Intersects(*(pTargetCollider->m_pOBBInWorld));
			break;
		case TYPE_SPHERE:
			bCollision = m_pOBBInWorld->Intersects(*(pTargetCollider->m_pSPHEREInWorld));
			break;
		}
		break;
	case TYPE_SPHERE:
		switch (pTargetCollider->m_eType)
		{
		case TYPE_AABB:
			bCollision = m_pSPHEREInWorld->Intersects(*(pTargetCollider->m_pAABBInWorld));
			break;
		case TYPE_OBB:
			bCollision = m_pSPHEREInWorld->Intersects(*(pTargetCollider->m_pOBBInWorld));
			break;
		case TYPE_SPHERE:
			bCollision = m_pSPHEREInWorld->Intersects(*(pTargetCollider->m_pSPHEREInWorld));
			break;
		}
		break;
	}
	return bCollision;
}

void CCollider::Update(_fmatrix WorldMatrix)
{
	if (TYPE_AABB == m_eType)
	{
		m_pAABB->Transform(*m_pAABBInWorld, Remove_Rotation(WorldMatrix));
	}

	if (TYPE_OBB == m_eType)
	{
		m_pOBB->Transform(*m_pOBBInWorld, WorldMatrix);
	}
	if (TYPE_SPHERE == m_eType)
	{
		m_pSPHERE->Transform(*m_pSPHEREInWorld, WorldMatrix);
	}


}

#ifdef _DEBUG
HRESULT CCollider::Render()
{
	_vector		vColor = m_isCollision == true ? XMVectorSet(1.f, 0.f, 0.f, 1.f) : XMVectorSet(0.f, 1.f, 0.f, 1.f);

	_matrix		ViewMatrix, ProjMatrix;

	CPipeLine*		pPipeLine = GET_INSTANCE(CPipeLine);

	ViewMatrix = pPipeLine->Get_TransformMatrix(CPipeLine::D3DTS_VIEW);
	ProjMatrix = pPipeLine->Get_TransformMatrix(CPipeLine::D3DTS_PROJ);

	m_pDeviceContext->GSSetShader(nullptr, nullptr, 0);

	m_pBasicEffect->SetWorld(XMMatrixIdentity());
	m_pBasicEffect->SetView(ViewMatrix);
	m_pBasicEffect->SetProjection(ProjMatrix);

	m_pDeviceContext->IASetInputLayout(m_pInputLayout);

	m_pBasicEffect->Apply(m_pDeviceContext);

	m_pBatch->Begin();

	switch (m_eType)
	{
	case CCollider::TYPE_AABB:
		DX::Draw(m_pBatch, *m_pAABBInWorld, vColor);
		break;
	case CCollider::TYPE_OBB:
		DX::Draw(m_pBatch, *m_pOBBInWorld, vColor);
		break;
	case CCollider::TYPE_SPHERE:
		DX::Draw(m_pBatch, *m_pSPHEREInWorld, vColor);
		break;
	}

	m_pBatch->End();	

	RELEASE_INSTANCE(CPipeLine);

	return S_OK;
}
#endif // _DEBUG

_matrix CCollider::Remove_Rotation(_fmatrix TransformMatrix)
{
	_matrix		ResultMatrix = TransformMatrix;
		
	_vector		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * XMVectorGetX(XMVector3Length(TransformMatrix.r[0]));
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * XMVectorGetX(XMVector3Length(TransformMatrix.r[1]));
	_vector		vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * XMVectorGetX(XMVector3Length(TransformMatrix.r[2]));

	ResultMatrix.r[0] = vRight;
	ResultMatrix.r[1] = vUp;
	ResultMatrix.r[2] = vLook;

	return ResultMatrix;
}

_vector CCollider::Compute_Min()
{
	if (nullptr == m_pAABBInWorld)
		return XMVectorZero();

	return XMVectorSet(m_pAABBInWorld->Center.x - m_pAABBInWorld->Extents.x,
		m_pAABBInWorld->Center.y - m_pAABBInWorld->Extents.y,
		m_pAABBInWorld->Center.z - m_pAABBInWorld->Extents.z,
		1.f);
}

_vector CCollider::Compute_Max()
{
	if (nullptr == m_pAABBInWorld)
		return XMVectorZero();

	return XMVectorSet(m_pAABBInWorld->Center.x + m_pAABBInWorld->Extents.x,
		m_pAABBInWorld->Center.y + m_pAABBInWorld->Extents.y,
		m_pAABBInWorld->Center.z + m_pAABBInWorld->Extents.z,
		1.f);
}

CCollider::OBBDESC CCollider::Compute_OBB()
{
	OBBDESC		OBBDesc;
	ZeroMemory(&OBBDesc, sizeof(OBBDESC));

	_float3		vPoints[8];

	m_pOBBInWorld->GetCorners(vPoints);

	XMStoreFloat3(&OBBDesc.vCenter, (XMLoadFloat3(&vPoints[2]) + XMLoadFloat3(&vPoints[4])) * 0.5f);

	XMStoreFloat3(&OBBDesc.vAlignAxis[0], XMVector3Normalize(XMLoadFloat3(&vPoints[2]) - XMLoadFloat3(&vPoints[3])));
	XMStoreFloat3(&OBBDesc.vAlignAxis[1], XMVector3Normalize(XMLoadFloat3(&vPoints[2]) - XMLoadFloat3(&vPoints[1])));
	XMStoreFloat3(&OBBDesc.vAlignAxis[2], XMVector3Normalize(XMLoadFloat3(&vPoints[2]) - XMLoadFloat3(&vPoints[6])));
	

	XMStoreFloat3(&OBBDesc.vCenterAxis[0], (XMLoadFloat3(&vPoints[2]) + XMLoadFloat3(&vPoints[5])) * 0.5f - XMLoadFloat3(&OBBDesc.vCenter));
	XMStoreFloat3(&OBBDesc.vCenterAxis[1], (XMLoadFloat3(&vPoints[2]) + XMLoadFloat3(&vPoints[7])) * 0.5f - XMLoadFloat3(&OBBDesc.vCenter));
	XMStoreFloat3(&OBBDesc.vCenterAxis[2], (XMLoadFloat3(&vPoints[2]) + XMLoadFloat3(&vPoints[0])) * 0.5f - XMLoadFloat3(&OBBDesc.vCenter));

	return OBBDesc;	
}

CCollider * CCollider::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, TYPE eType)
{
	CCollider *	pInstance = new CCollider(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(eType)))
	{
		MSG_BOX(TEXT("Failed to Created CCollider"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CCollider::Clone(void * pArg)
{
	CCollider *	pInstance = new CCollider(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CCollider"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollider::Free()
{
	__super::Free();

	Safe_Delete(m_pOBBInWorld);
	Safe_Delete(m_pOBB);

	Safe_Delete(m_pSPHEREInWorld);
	Safe_Delete(m_pSPHERE);

	Safe_Delete(m_pAABBInWorld);
	Safe_Delete(m_pAABB);



	Safe_Release(m_pInputLayout);

	if (false == m_isCloned)
	{		
		Safe_Delete(m_pBatch);
		Safe_Delete(m_pBasicEffect);
	}

	

}
