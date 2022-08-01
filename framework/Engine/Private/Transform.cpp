#include "..\Public\Transform.h"
#include "Shader.h"
#include "PipeLine.h"

CTransform::CTransform(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
{

}

_float3 CTransform::Get_Scale()
{
	_float3		vScale;

	vScale.x = XMVectorGetX(XMVector3Length(Get_State(STATE_RIGHT)));
	vScale.y = XMVectorGetX(XMVector3Length(Get_State(STATE_UP)));
	vScale.z = XMVectorGetX(XMVector3Length(Get_State(STATE_LOOK)));

	return vScale;
}

HRESULT CTransform::Set_WorldMatrix(_fmatrix WorldMatrix)
{
	XMStoreFloat4x4(&m_WorldMatrix, WorldMatrix);

	return S_OK;
}

_matrix CTransform::Get_WorldMatrix()
{
	return XMLoadFloat4x4(&m_WorldMatrix);
}

_matrix CTransform::Get_WorldMatrixInverse()
{
	return XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix));
}

_float4x4 CTransform::Get_WorldFloat4x4()
{
	return m_WorldMatrix;
}

_float4x4 CTransform::Get_WorldFloat4x4_TP()
{
	_float4x4	WorldMatrix;
	XMStoreFloat4x4(&WorldMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_WorldMatrix)));

	return WorldMatrix;
}

HRESULT CTransform::NativeConstruct_Prototype(void* pArg)
{
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

	if (nullptr != pArg)
		memcpy(&m_TransformDesc, pArg, sizeof(TRANSFORMDESC));

	return S_OK;

}

HRESULT CTransform::Bind_WorldMatrixOnShader(CShader * pShader, const char * pConstantName)
{
	if (nullptr == pShader)
		return E_FAIL;

	_float4x4		WorldMatrixTP;

	XMStoreFloat4x4(&WorldMatrixTP, XMMatrixTranspose(XMLoadFloat4x4(&m_WorldMatrix)));

	pShader->Set_RawValue(pConstantName, &WorldMatrixTP, sizeof(_float4x4));

	return S_OK;
}

HRESULT CTransform::Go_Straight(_double TimeDelta)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);

	vPosition += XMVector3Normalize(vLook) * (_float)m_TransformDesc.SpeedPerSec * (_float)TimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);

	return S_OK;
}

HRESULT CTransform::Go_Left(_double TimeDelta)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);

	vPosition -= XMVector3Normalize(vRight) * (_float)m_TransformDesc.SpeedPerSec * (_float)TimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);

	return S_OK;
}

HRESULT CTransform::Go_Right(_double TimeDelta)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);

	vPosition += XMVector3Normalize(vRight) * (_float)m_TransformDesc.SpeedPerSec * (_float)TimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);

	return S_OK;
}

HRESULT CTransform::Go_CircleLeft(_double TimeDelta, _float radius)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);
	_float		fPosy = XMVectorGetY(Get_State(CTransform::STATE_POSITION));

	_float x = radius * cosf(XMConvertToRadians((-1) * TimeDelta));
	_float z = radius * sinf(XMConvertToRadians((-1) * TimeDelta));

	vPosition = XMVectorSet(x, fPosy, z, 1.f);

	Set_State(CTransform::STATE_POSITION, vPosition);

	return S_OK;
}

HRESULT CTransform::Go_TurnYAxis(_double TimeDelta)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vAxis = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_fmatrix	fRotateMatrix = XMMatrixRotationAxis(vAxis, TimeDelta);

	vPosition = XMVector3TransformNormal(vPosition, fRotateMatrix);
	vPosition = XMVectorSetW(vPosition, 1.f);


	Set_State(CTransform::STATE_POSITION, vPosition);

	return S_OK;
}


HRESULT CTransform::Go_CircleNtropyLeft(_double TimeDelta, _float radius)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);
	_float		fPosy = XMVectorGetY(Get_State(CTransform::STATE_POSITION));

	_float x = radius * cosf(XMConvertToRadians((-2) * TimeDelta));
	_float z = radius * sinf(XMConvertToRadians((-2) * TimeDelta));

	vPosition = XMVectorSet(x, fPosy, z, 1.f);

	Set_State(CTransform::STATE_POSITION, vPosition);

	return S_OK;
}


_vector CTransform::CirclePos(_double TimeDelta, _float radius)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);
	_float		fPosy = XMVectorGetY(Get_State(CTransform::STATE_POSITION));

	_float x = radius * cosf(XMConvertToRadians((-1) * TimeDelta));
	_float z = radius * sinf(XMConvertToRadians((-1) * TimeDelta));

	vPosition = XMVectorSet(x, fPosy, z, 1.f);

	return vPosition;
}

HRESULT CTransform::Go_CircleRight(_double TimeDelta, _float radius)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);
	_float		fPosy = XMVectorGetY(Get_State(CTransform::STATE_POSITION));
	_float x = radius * cosf(XMConvertToRadians(TimeDelta));
	_float z = radius * sinf(XMConvertToRadians(TimeDelta));

	//vPosition += XMVector3Normalize(vRight) * (_float)m_TransformDesc.SpeedPerSec * (_float)TimeDelta;
	vPosition = XMVectorSet(x, fPosy, z, 1.f);
	Set_State(CTransform::STATE_POSITION, vPosition);

	return S_OK;
}

HRESULT CTransform::Go_CircleCameraLeft(_double TimeDelta, _float radius)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);

	_float x = radius * cosf(XMConvertToRadians((-1) * TimeDelta));
	_float z = radius * sinf(XMConvertToRadians((-1) * TimeDelta));

	_float y = XMVectorGetY(vPosition);
	vPosition = XMVectorSet(x, y, z, 1.f);
	Set_State(CTransform::STATE_POSITION, vPosition);

	return S_OK;
}

HRESULT CTransform::Go_BackWard(_double TimeDelta)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);

	vPosition -= XMVector3Normalize(vLook) * (_float)m_TransformDesc.SpeedPerSec * (_float)TimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);

	return S_OK;
}

_bool CTransform::Distance(_fvector vTargetPos, _float fLimitDistance)
{
	_vector		vPosition = Get_State(STATE_POSITION);

	_vector		vLook = vTargetPos - vPosition;

	_vector		fDistance = XMVector3Length(vLook);

	if (XMVectorGetX(fDistance) < fLimitDistance)
		return true;

	else
		return false;
}

_bool CTransform::FollowingDistance(_fvector vTargetPos, _float fLimitDistance)
{
	_vector		vPosition = Get_State(STATE_POSITION);

	_vector		vLook = vTargetPos - vPosition;

	_vector		fDistance = XMVector3Length(vLook);

	if (XMVectorGetX(fDistance) >= fLimitDistance)
		return true;

	else
		return false;
}

HRESULT CTransform::TurnBack(_fvector vAxis)
{
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);
	_vector		vUp = Get_State(CTransform::STATE_UP);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);

	_matrix		RotationMatrix;
	RotationMatrix = XMMatrixRotationAxis(vAxis, XMConvertToRadians(180.f));

	vRight = XMVector3TransformNormal(vRight, RotationMatrix);
	vUp = XMVector3TransformNormal(vUp, RotationMatrix);
	vLook = XMVector3TransformNormal(vLook, RotationMatrix);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);

	return S_OK;
}

HRESULT CTransform::LookAt(_fvector vTargetPos)
{
	_vector      vPosition = Get_State(STATE_POSITION);
	_float3      vScale = Get_Scale();

	_vector      vLook = vTargetPos - vPosition;
	_vector      vAxisY = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_vector      vRight = XMVector3Cross(vAxisY, vLook);
	_vector      vUp = XMVector3Cross(vLook, vRight);


	vRight = XMVector3Normalize(vRight)*vScale.x;
	vUp = XMVector3Normalize(vUp)*vScale.y;
	vLook = XMVector3Normalize(vLook)*vScale.z;


	Set_State(STATE_RIGHT, vRight);
	Set_State(STATE_UP, vUp);
	Set_State(STATE_LOOK, vLook);

	return S_OK;
}

HRESULT CTransform::LookAtReverse(_fvector vTargetPos)		// LaserBox 설치할때 윗줄라인은 이 코드를 사용해야 한다
{
	_vector      vPosition = Get_State(STATE_POSITION);
	_float3      vScale = Get_Scale();

	_vector      vLook = vTargetPos - vPosition;
	_vector      vAxisY = XMVectorSet(0.f, -1.f, 0.f, 0.f);
	_vector      vRight = XMVector3Cross(vAxisY, vLook);
	_vector      vUp = XMVector3Cross(vLook, vRight);


	vRight = XMVector3Normalize(vRight)*vScale.x;
	vUp = XMVector3Normalize(vUp)*vScale.y;
	vLook = XMVector3Normalize(vLook)*vScale.z;


	Set_State(STATE_RIGHT, vRight);
	Set_State(STATE_UP, vUp);
	Set_State(STATE_LOOK, vLook);

	return S_OK;
}



HRESULT CTransform::Scaled(const _float3 & vScale)
{
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);
	_vector		vUp = Get_State(CTransform::STATE_UP);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);

	vRight = XMVector3Normalize(vRight) * vScale.x;
	vUp = XMVector3Normalize(vUp) * vScale.y;
	vLook = XMVector3Normalize(vLook) * vScale.z;

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);

	return S_OK;
}

HRESULT CTransform::Scaling(_float fTimeDelta)
{
	_vector		fScale = XMVectorSet(0.f, 0.f, 0.f, 0.f);

	_vector		vRight = Get_State(CTransform::STATE_RIGHT);
	fScale = XMVector3Normalize(vRight) + m_TransformDesc.fScalePerSec * fTimeDelta;

	Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight) * fScale);

	_vector		vUp = Get_State(CTransform::STATE_UP);
	fScale = XMVector3Normalize(vUp) + m_TransformDesc.fScalePerSec * fTimeDelta;

	Set_State(CTransform::STATE_UP, XMVector3Normalize(vUp) * fScale);

	_vector		vLook = Get_State(CTransform::STATE_LOOK);
	fScale = XMVector3Normalize(vLook) + m_TransformDesc.fScalePerSec * fTimeDelta;

	Set_State(CTransform::STATE_LOOK, XMVector3Normalize(vLook) * fScale);

	return S_OK;
}

HRESULT CTransform::Turn(_fvector vAxis, _double TimeDelta)
{
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);
	_vector		vUp = Get_State(CTransform::STATE_UP);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);

	_matrix		RotationMatrix;
	RotationMatrix = XMMatrixRotationAxis(vAxis, _float(m_TransformDesc.RotationPerSec * TimeDelta));

	vRight = XMVector3TransformNormal(vRight, RotationMatrix);
	vUp = XMVector3TransformNormal(vUp, RotationMatrix);
	vLook = XMVector3TransformNormal(vLook, RotationMatrix);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);

	return S_OK;
}


HRESULT CTransform::Rotation(_fvector vAxis, _double Radian)
{
	_float3		vScale = Get_Scale();

	_vector		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * vScale.x;
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * vScale.y;
	_vector		vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * vScale.z;

	_matrix		RotationMatrix;
	RotationMatrix = XMMatrixRotationAxis(vAxis, _float(Radian));

	vRight = XMVector3TransformNormal(vRight, RotationMatrix);
	vUp = XMVector3TransformNormal(vUp, RotationMatrix);
	vLook = XMVector3TransformNormal(vLook, RotationMatrix);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);

	return S_OK;
}

HRESULT CTransform::TurnAxis(_fvector vAxis)
{
	_float3      vScale = Get_Scale();
	_vector      vLook = vAxis;
	_vector      vAxisY = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_vector      vRight = XMVector3Cross(vAxisY, vLook);
	_vector      vUp = XMVector3Cross(vLook, vRight);


	vRight = XMVector3Normalize(vRight)*vScale.x;
	vUp = XMVector3Normalize(vUp)*vScale.y;
	vLook = XMVector3Normalize(vLook)*vScale.z;


	Set_State(STATE_RIGHT, vRight);
	Set_State(STATE_UP, vUp);
	Set_State(STATE_LOOK, vLook);

	return S_OK;
}

HRESULT CTransform::TurnReverseAxis(_fvector vAxis)
{
	_float3      vScale = Get_Scale();
	_vector      vLook = vAxis;
	_vector      vAxisY = XMVectorSet(0.f, -1.f, 0.f, 0.f);
	_vector      vRight = XMVector3Cross(vAxisY, vLook);
	_vector      vUp = XMVector3Cross(vLook, vRight);


	vRight = XMVector3Normalize(vRight)*vScale.x;
	vUp = XMVector3Normalize(vUp)*vScale.y;
	vLook = XMVector3Normalize(vLook)*vScale.z;


	Set_State(STATE_RIGHT, vRight);
	Set_State(STATE_UP, vUp);
	Set_State(STATE_LOOK, vLook);

	return S_OK;
}

HRESULT CTransform::Chase(_fvector vTargetPos, _double TimeDelta)
{
	_vector		vPosition = Get_State(STATE_POSITION);

	_vector		vLook = vTargetPos - vPosition;

	_vector		fDistance = XMVector3Length(vLook);

	vPosition += XMVector3Normalize(vLook) * (_float)m_TransformDesc.SpeedPerSec * (_float)TimeDelta;

	Set_State(STATE_POSITION, vPosition);

	return S_OK;
}

CTransform * CTransform::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void* pArg)
{
	CTransform*	pInstance = new CTransform(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CTransform"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CTransform::Clone(void * pArg)
{
	return nullptr;
}

void CTransform::Free()
{
	__super::Free();

}