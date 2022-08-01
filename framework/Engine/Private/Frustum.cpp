#include "..\Public\Frustum.h"
#include "PIpeLine.h"

IMPLEMENT_SINGLETON(CFrustum)

CFrustum::CFrustum()
{
}

HRESULT CFrustum::NativeConstruct()
{
	/* 투영스페이스 상의 절두체를 구성한다. */
	m_vPoints[0] = _float3(-1.f, 1.f, 0.f);
	m_vPoints[1] = _float3(1.f, 1.f, 0.f);
	m_vPoints[2] = _float3(1.f, -1.f, 0.f);
	m_vPoints[3] = _float3(-1.f, -1.f, 0.f);

	m_vPoints[4] = _float3(-1.f, 1.f, 1.f);
	m_vPoints[5] = _float3(1.f, 1.f, 1.f);
	m_vPoints[6] = _float3(1.f, -1.f, 1.f);
	m_vPoints[7] = _float3(-1.f, -1.f, 1.f);	
		
	return S_OK;
}

HRESULT CFrustum::Transform_ToLocalSpace(_fmatrix WorldMatrix)
{
	_matrix			WorldMatrixInverse = XMMatrixInverse(nullptr, WorldMatrix);

	_vector			vPointInLocal[8];

	for (_uint i = 0; i < 8; ++i)	
		vPointInLocal[i] = XMVector3TransformCoord(XMLoadFloat3(&m_vPointInWorld[i]), WorldMatrixInverse);				

	XMStoreFloat4(&m_vPlaneInLocal[0], XMPlaneFromPoints(vPointInLocal[1], vPointInLocal[5], vPointInLocal[6]));
	XMStoreFloat4(&m_vPlaneInLocal[1], XMPlaneFromPoints(vPointInLocal[4], vPointInLocal[0], vPointInLocal[3]));
	XMStoreFloat4(&m_vPlaneInLocal[2], XMPlaneFromPoints(vPointInLocal[4], vPointInLocal[5], vPointInLocal[1]));
	XMStoreFloat4(&m_vPlaneInLocal[3], XMPlaneFromPoints(vPointInLocal[3], vPointInLocal[2], vPointInLocal[6]));
	XMStoreFloat4(&m_vPlaneInLocal[4], XMPlaneFromPoints(vPointInLocal[5], vPointInLocal[4], vPointInLocal[7]));
	XMStoreFloat4(&m_vPlaneInLocal[5], XMPlaneFromPoints(vPointInLocal[0], vPointInLocal[1], vPointInLocal[2]));


	return S_OK;
}

HRESULT CFrustum::Transform_ToWorldSpace()
{
	CPipeLine*		pPipeLine = GET_INSTANCE(CPipeLine);

	_matrix		ProjMatrix = pPipeLine->Get_TransformMatrix(CPipeLine::D3DTS_PROJ);
	ProjMatrix = XMMatrixInverse(nullptr, ProjMatrix);

	_matrix		ViewMatrix = pPipeLine->Get_TransformMatrix(CPipeLine::D3DTS_VIEW);
	ViewMatrix = XMMatrixInverse(nullptr, ViewMatrix);

	_vector			vPointInWorld[8];

	for (_uint i = 0; i < 8; ++i)
	{
		vPointInWorld[i] = XMVector3TransformCoord(XMLoadFloat3(&m_vPoints[i]), ProjMatrix);
		vPointInWorld[i] = XMVector3TransformCoord(vPointInWorld[i], ViewMatrix);		

		XMStoreFloat3(&m_vPointInWorld[i], vPointInWorld[i]);
	}

	XMStoreFloat4(&m_vPlaneInWorld[0], XMPlaneFromPoints(vPointInWorld[1], vPointInWorld[5], vPointInWorld[6]));
	XMStoreFloat4(&m_vPlaneInWorld[1], XMPlaneFromPoints(vPointInWorld[4], vPointInWorld[0], vPointInWorld[3]));
	XMStoreFloat4(&m_vPlaneInWorld[2], XMPlaneFromPoints(vPointInWorld[4], vPointInWorld[5], vPointInWorld[1]));
	XMStoreFloat4(&m_vPlaneInWorld[3], XMPlaneFromPoints(vPointInWorld[3], vPointInWorld[2], vPointInWorld[6]));
	XMStoreFloat4(&m_vPlaneInWorld[4], XMPlaneFromPoints(vPointInWorld[5], vPointInWorld[4], vPointInWorld[7]));
	XMStoreFloat4(&m_vPlaneInWorld[5], XMPlaneFromPoints(vPointInWorld[0], vPointInWorld[1], vPointInWorld[2]));

	RELEASE_INSTANCE(CPipeLine);

	return S_OK;
}

_bool CFrustum::isIn_WorldSpace(_vector vWorldPos, _float fRange)
{
	for (_uint i = 0; i < 6; ++i)
	{
		if (fRange < XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_vPlaneInWorld[i]), vWorldPos)))
			return false;
	}

	return true;
}

_bool CFrustum::isIn_LocalSpace(_vector vLocalPos, _float fRange)
{
	for (_uint i = 0; i < 6; ++i)
	{
		if (fRange < XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_vPlaneInLocal[i]), vLocalPos)))
			return false;
	}

	return true;
}

void CFrustum::Free()
{
}
