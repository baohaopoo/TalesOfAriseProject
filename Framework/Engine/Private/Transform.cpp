#include "..\Public\Transform.h"
#include "Shader.h"
#include "PipeLine.h"
#include "Navigation.h"
#include "Cell.h"

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

HRESULT CTransform::Check_Right_Left(_fvector vTargetLook, _double TimeDelta)
{
	_vector vLook = Get_State(STATE_LOOK);
	_vector vPos = Get_State(STATE_POSITION);
	_float fDot = XMVectorGetX(XMVector3Dot(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMVector3Cross(vTargetLook, vLook)));

	_float fDot2 = XMVectorGetX(XMVector3Dot(vLook, vTargetLook));



	//원래 방향과 정반대 누르면 안감. 왜냐먼 좌우 각도만 계산하므로 정반대방향은 동일하므로...
	if (0.1f < fDot) // 0 < cosθ
		Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), -TimeDelta); //왼쪽 오른쪽 구별 
	else if (fDot < -0.1f) // cosθ < 0
		Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), TimeDelta); //왼쪽 오른쪽 구별 

	else { //정반대벡터 ,정방향벡터 두개가 존재하는데. 두 벡터의 내적이 cos
		
		_float absf = fabsf(fDot2);
		_float seta = XMConvertToRadians(120.f);
		_float delta = (fabsf(acosf(fDot2)));
			if (fabsf(acosf(fDot2))>XMConvertToRadians(120.f))
		{

			Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), TimeDelta*5.f); //왼쪽 오른쪽 구별 
		}


	}


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

HRESULT CTransform::Go_Straight(_double TimeDelta, CNavigation* pNavigation)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);

	vPosition += XMVector3Normalize(vLook) * (_float)m_TransformDesc.SpeedPerSec * (_float)TimeDelta;

	if (nullptr == pNavigation)
	{
		Set_State(CTransform::STATE_POSITION, vPosition);
		return S_OK;
	}

	if (true == pNavigation->Move_OnNavigation(vPosition)) {
		Set_State(CTransform::STATE_POSITION, vPosition);

		// 현재 셀 정보
		CCell* pCurCell = pNavigation->Get_CurCell();

		if (nullptr != pCurCell) {
			_float3* pPoints = pCurCell->Get_Points();

			_vector vPlane = XMPlaneFromPoints(XMLoadFloat3(&pPoints[0]), XMLoadFloat3(&pPoints[1]), XMLoadFloat3(&pPoints[2]));
			_float A = XMVectorGetX(vPlane);
			_float B = XMVectorGetY(vPlane);
			_float C = XMVectorGetZ(vPlane);
			_float D = XMVectorGetW(vPlane);


			// AX + BY + CZ + D = 0;
			// Y = -(AX + CZ + D) / B;
		}

	}

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

HRESULT CTransform::Go_Down(_double TimeDelta)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vUp = Get_State(CTransform::STATE_UP);

	vPosition -= XMVector3Normalize(vUp) * (_float)m_TransformDesc.SpeedPerSec * (_float)TimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);

	return S_OK;
}

HRESULT CTransform::Go_CircleLeft(_double TimeDelta, _float radius)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);
	_float		fPosy = XMVectorGetY(Get_State(CTransform::STATE_POSITION));

	_float x = radius * cosf(XMConvertToRadians((-1) * (_float)TimeDelta));
	_float z = radius * sinf(XMConvertToRadians((-1) * (_float)TimeDelta));

	vPosition = XMVectorSet(x, fPosy, z, 1.f);

	Set_State(CTransform::STATE_POSITION, vPosition);

	return S_OK;
}

HRESULT CTransform::Go_TurnYAxis(_double TimeDelta)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vAxis = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_fmatrix	fRotateMatrix = XMMatrixRotationAxis(vAxis, (_float)TimeDelta);

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

	_float x = radius * cosf(XMConvertToRadians((-2) * (_float)TimeDelta));
	_float z = radius * sinf(XMConvertToRadians((-2) * (_float)TimeDelta));

	vPosition = XMVectorSet(x, fPosy, z, 1.f);

	Set_State(CTransform::STATE_POSITION, vPosition);

	return S_OK;
}


_vector CTransform::CirclePos(_double TimeDelta, _float radius)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);
	_float		fPosy = XMVectorGetY(Get_State(CTransform::STATE_POSITION));

	_float x = radius * cosf(XMConvertToRadians((-1) * (_float)TimeDelta));
	_float z = radius * sinf(XMConvertToRadians((-1) * (_float)TimeDelta));

	vPosition = XMVectorSet(x, fPosy, z, 1.f);

	return vPosition;
}

HRESULT CTransform::Go_CircleRight(_double TimeDelta, _float radius)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);
	_float		fPosy = XMVectorGetY(Get_State(CTransform::STATE_POSITION));
	_float x = radius * cosf(XMConvertToRadians((_float)TimeDelta));
	_float z = radius * sinf(XMConvertToRadians((_float)TimeDelta));

	//vPosition += XMVector3Normalize(vRight) * (_float)m_TransformDesc.SpeedPerSec * (_float)TimeDelta;
	vPosition = XMVectorSet(x, fPosy, z, 1.f);
	Set_State(CTransform::STATE_POSITION, vPosition);

	return S_OK;
}

HRESULT CTransform::Go_CircleCameraLeft(_double TimeDelta, _float radius)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);

	_float x = radius * cosf(XMConvertToRadians((-1) * (_float)TimeDelta));
	_float z = radius * sinf(XMConvertToRadians((-1) * (_float)TimeDelta));

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

_bool CTransform::FrontPlayer(_fvector vTargetPos)
{
	_vector      vForward = Get_State(STATE_LOOK);
	_vector		 vPosition = Get_State(STATE_POSITION);
	_vector      vTarget = XMVector3Normalize(vTargetPos - vPosition);

	_float fDot = XMVectorGetX(XMVector3Dot(vForward, vTarget));

	if (fDot >= 0)			// 적이 앞에 있다				
		return true;

	else
		return false;
}

_bool CTransform::LeftPlayer(_fvector vTargetPos)
{
	_vector vLook = Get_State(STATE_LOOK);
	_vector vPos = Get_State(STATE_POSITION);
	_vector vTarget = XMVector3Normalize(vTargetPos - vPos);

	_float result = XMVectorGetX(XMVector3Dot(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMVector3Cross(vTarget, vLook)));

	if (result >= 0.f)			// 플레이어가 적 왼쪽에 있다
		return true;

	else                        // 플레이어가 적 오른쪽에 있다
		return false;
}

_float CTransform::Get_Height(CNavigation * pNavigation)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);

	if (true == pNavigation->Move_OnNavigation(vPosition)) {
		Set_State(CTransform::STATE_POSITION, vPosition);

		// 현재 셀 정보
		CCell* pCurCell = pNavigation->Get_CurCell();

		if (nullptr != pCurCell) {
			_float3* pPoints = pCurCell->Get_Points();

			_vector vPlane = XMPlaneFromPoints(XMLoadFloat3(&pPoints[0]), XMLoadFloat3(&pPoints[1]), XMLoadFloat3(&pPoints[2]));
			_float A = XMVectorGetX(vPlane);
			_float B = XMVectorGetY(vPlane);
			_float C = XMVectorGetZ(vPlane);
			_float D = XMVectorGetW(vPlane);


			// AX + BY + CZ + D = 0;
			// Y = -(AX + CZ + D) / B;

			_float3 fPos;
			XMStoreFloat3(&fPos, vPosition);

			fPos.y = -(A * fPos.x + C * fPos.z + D) / B;

			return fPos.y;
		}
	}

	return 0.f;
}

void CTransform::Set_Height(_float fHeight)
{
	m_WorldMatrix._42 = fHeight;
}

HRESULT CTransform::Move(_float3 fPos)
{
	Set_State(CTransform::STATE_POSITION, XMVectorSet(fPos.x, fPos.y, fPos.z, 1.f));

	return S_OK;
}

HRESULT CTransform::Move(_float fPosX, _float fPosY, _float fPosZ)
{
	return Move(_float3(fPosX, fPosY, fPosZ));
}

HRESULT CTransform::Go_Straight_PlaneXZ(_double TimeDelta)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);
	vLook = XMVectorSetY(vLook, 0.f);

	vPosition += XMVector3Normalize(vLook) * (_float)m_TransformDesc.SpeedPerSec * (_float)TimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);

	return S_OK;
}

HRESULT CTransform::Go_BackWard_PlaneXZ(_double TimeDelta)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);
	vLook = XMVectorSetY(vLook, 0.f);

	vPosition -= XMVector3Normalize(vLook) * (_float)m_TransformDesc.SpeedPerSec * (_float)TimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);

	return S_OK;
}

HRESULT CTransform::TurnByRadian(_fvector vAxis, _double Radian)
{
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);
	_vector		vUp = Get_State(CTransform::STATE_UP);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);

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

HRESULT CTransform::Look(_fvector vLookk)
{
	_vector      vPosition = Get_State(STATE_POSITION);
	_float3      vScale = Get_Scale();

	_vector      vLook = vLookk;
	_vector      vAxisY = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_vector      vRight = XMVector3Cross(vAxisY, vLook);
	_vector      vUp = XMVector3Cross(vLook, vRight);
	vLook = XMVectorSetY(vLook, 0.f);


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

HRESULT CTransform::RotationXYZ(_double RadianX, _double RadianY, _double RadianZ)
{
	_float3		vScale = Get_Scale();

	_vector		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * vScale.x;
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * vScale.y;
	_vector		vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * vScale.z;

	_matrix		RotationMatrix;
	RotationMatrix =
		XMMatrixRotationAxis(XMVectorSet(1.f, 0.f, 0.f, 1.f), _float(RadianX)) *
		XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 1.f), _float(RadianY)) *
		XMMatrixRotationAxis(XMVectorSet(0.f, 0.f, 1.f, 1.f), _float(RadianZ));

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