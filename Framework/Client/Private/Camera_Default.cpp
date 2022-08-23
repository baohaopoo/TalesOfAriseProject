#include "stdafx.h"
#include "..\Public\Camera_Default.h"
#include "GameInstance.h"
#include"Player.h"
#include "Input_Device.h"
CCamera_Default::CCamera_Default(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CCamera(pDeviceOut, pDeviceContextOut)
	, m_pPlayer_Manager(CPlayer_Manager::GetInstance())
	, m_pGameInstance(CGameInstance::GetInstance())
{
}

CCamera_Default::CCamera_Default(const CCamera_Default & rhs)
	: CCamera(rhs)
	, m_pPlayer_Manager(CPlayer_Manager::GetInstance())
	, m_pGameInstance(CGameInstance::GetInstance())
{
}

HRESULT CCamera_Default::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Default::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 10.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;

	//구면좌표계 관련
	m_fCameraLookatPlayerHeight = 0.4f;	//카메라가 플레이어 높이 어디 처다볼지
	m_fMouseSensitivity = 0.5f;			//마우스 민감도
	m_fRadius = 3.f;
	m_minRadius = 0.1f;
	m_maxRadius = 6.f;
	m_minAzimuth = 0.f;
	m_maxAzimuth = 360.f;
	m_minElevation = 0.f;
	m_maxElevation = 70.f;

	m_pPlayer_Manager->Set_Camera(this);

	return S_OK;
}

void CCamera_Default::Tick(_double TimeDelta)
{

	__super::Tick(TimeDelta);

	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	Safe_AddRef(pGameInstance);

	_vector vPlayerPos = ((CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"), m_pPlayer_Manager->Get_MainPlayerIndex()))->Get_State(CTransform::STATE_POSITION);

	if (!m_bSphericalStart)
	{

		_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		m_bSphericalStart = true;
		SphericalCoordinates(vPos, vPlayerPos);
		m_fAzimuth = XMConvertToRadians(-90.f);
		m_fElevation = XMConvertToRadians(38.5f);
		//m_fRadius = 6.f;
		m_fRadius = 3.f;
		_vector vCameraPos = toCartesian() + vPlayerPos;
		XMStoreFloat3(&m_vPrePlayerPos, vPlayerPos);
		XMStoreFloat3(&m_vCurPlayerPos, vPlayerPos);

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetW(vCameraPos, 1.f));
		XMStoreFloat3(&m_vCameraEye, vPlayerPos);
		m_pTransformCom->LookAt(XMVectorSetY(vPlayerPos, XMVectorGetY(vPlayerPos) + m_fCameraLookatPlayerHeight));
		XMStoreFloat3(&m_vCameraAt, XMVectorSetY(vPlayerPos, XMVectorGetY(vPlayerPos) + m_fCameraLookatPlayerHeight));
	}


	//F1키 로 구면좌표계<->선생님카메라
	if (pGameInstance->Key_Down(DIK_F1))
	{
		if (m_bSpherical)
			m_bSpherical = false;
		else
			m_bSpherical = true;
	}


	switch (m_eCameraState)
	{
	case Client::CCamera_Default::CAMERA_STATE_FIELD:
		Camera_Field(TimeDelta);
		break;
	case Client::CCamera_Default::CAMERA_STATE_BATTLE:
		Camera_Battle(TimeDelta);
		break;
	case Client::CCamera_Default::CAMERA_STATE_CHANGE:
		break;
	case Client::CCamera_Default::CAMERA_STATE_END:
		break;
	default:
		break;
	}







	//_float fSpeed = XMVectorGetX(XMVector3Length(vPlayerPos - m_pTransformCom->Get_State(CTransform::STATE_POSITION)));
	//m_fPlayerPosLerp += TimeDelta *pow(fSpeed, 1.5);
	//if (m_fPlayerPosLerp >= 1.f)
	//{
	//	m_fPlayerPosLerp = 0.f;
	//	m_vPrePlayerPos = m_vCurPlayerPos;   //전프레임의 알펜 저장 
	//	XMStoreFloat3(&m_vCurPlayerPos, vPlayerPos); //현재 프레임의 알펜 위치저장 
	//

	//}
	//_vector vPlayerLerpPos = XMVectorLerp(XMLoadFloat3(&m_vPrePlayerPos), XMLoadFloat3(&m_vCurPlayerPos), m_fPlayerPosLerp);

	//두 플레이어 포스와 나사이를 선형보간 .


	//if (m_pBattle_Manager->Get_Battle())
	//{
	// 

	// _float fSpeed = XMVectorGetX(XMVector3Length(m_pPlayer_Manager->Get_MainPlayer()->Get_TargetPos() - XMLoadFloat3(&m_vCameraAt)));
	// m_fTargetPosLerp += TimeDelta *pow(fSpeed, 1.5);
	// if (m_fPlayerPosLerp >= 1.f)
	// {
	//	 m_fPlayerPosLerp = 0.f;
	//	 m_vPreTargetPos = m_vCurTargetPos;   //전프레임의 알펜 저장 
	//	 XMStoreFloat3(&m_vCurPlayerPos, vPlayerPos); //현재 프레임의 알펜 위치저장 

	// }


	// m_pTransformCom->LookAt(XMVectorLerp(XMLoadFloat3(&m_vPrePlayerPos), XMLoadFloat3(&m_vCurPlayerPos), m_fPlayerPosLerp));

	// XMStoreFloat3(&m_vCurTargetPos, m_pPlayer_Manager->Get_MainPlayer()->Get_TargetPos());
	//}



	if (m_bChange)
	{


		m_pTransformCom->LookAt(m_pPlayer_Manager->Get_MainPlayer()->Get_TargetPos());
	}
	else {

		if (m_bSpherical) //구면좌표계	
		{

		}
		else
		{
			if (pGameInstance->Get_DIKeyState(DIK_W) & 0x8000)
			{
				m_pTransformCom->Go_Straight(TimeDelta);
			}
			if (GetKeyState('S') & 0x8000)
			{
				m_pTransformCom->Go_BackWard(TimeDelta);
			}
			if (GetKeyState('A') & 0x8000)
			{
				m_pTransformCom->Go_Left(TimeDelta);
			}
			if (GetKeyState('D') & 0x8000)
			{
				m_pTransformCom->Go_Right(TimeDelta);
			}



			_long		MouseMove;

			if (MouseMove = pGameInstance->Get_DIMMoveState(CInput_Device::DIMM_X))
			{
				m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), TimeDelta * MouseMove * 0.1f);
			}

			if (MouseMove = pGameInstance->Get_DIMMoveState(CInput_Device::DIMM_Y))
			{
				m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), TimeDelta * MouseMove* 0.1f);
			}

		}
	}




	//카메라흔들기

	if (m_bDownShake)
	{
		m_DownShakeTime += TimeDelta;
		if (m_DownShakeTime > 0.25)
		{
			m_DownShakeTime = 0.0;
			m_bDownShake = false;
			m_fShakeDown = 0.f;
		}

		if (m_DownShakeTime < 0.125)
		{
			m_fShakeDown += (rand() % 500) / 2500.f;
		}
		else {
			m_fShakeDown -= (rand() % 500) / 2500.f;
		}


		/*  아래쪽으로 점점 흔들리는 카메라. */


		_float x = (rand() % 1000 - 500) / 2500.f;
		_vector vCameraShake = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_UP))*-m_fShakeDown + XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_RIGHT))*x;
		_vector vCameraPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCameraPos + vCameraShake);


	}

	if (m_bCommonShake)
	{

		m_CommonShakeTime += TimeDelta;
		if (m_CommonShakeTime > 0.1)
		{
			m_CommonShakeTime = 0.0;
			m_bCommonShake = false;
		}


		_float x = (rand() % 1000 - 500) / 10000.f;
		_float y = (rand() % 1000 - 500) / 10000.f;
		_float z = (rand() % 1000 - 500) / 10000.f;
		_vector vCameraShake = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_UP))*y + XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_RIGHT))*x;
		_vector vCameraPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCameraPos + vCameraShake);
	}





	Safe_Release(pGameInstance);
}

void CCamera_Default::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT CCamera_Default::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CCamera_Default::SphericalCoordinates(_fvector vCamerapos, _fvector vTargetPos)
{
	m_RadMinAzimuth = XMConvertToRadians(m_minAzimuth);
	m_RadMaxAzimuth = XMConvertToRadians(m_maxAzimuth);

	m_RadmaxElevation = XMConvertToRadians(m_maxElevation);
	m_RadminElevation = XMConvertToRadians(m_minElevation);

	m_fAzimuth = atan2f(XMVectorGetZ(vCamerapos), XMVectorGetX(vCamerapos));
	m_fElevation = asinf(XMVectorGetY(vCamerapos) / m_fRadius);

	m_fT = m_fRadius*cosf(m_fElevation);

	_vector vCameraPos = XMVectorSet(m_fT*cosf(m_fAzimuth), m_fRadius*sinf(m_fElevation), m_fT*sinf(m_fAzimuth), 1.f);
	vCameraPos += vTargetPos;
	XMVectorSetW(vCameraPos, 1.f);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCameraPos);

}

void CCamera_Default::SphericalCoordinatesRotate(_float newAzimuth, _float newElevation)
{


	m_fAzimuth += newAzimuth;
	m_fElevation += newElevation;


	//repeat
	m_fAzimuth = m_fAzimuth - ((_uint)(m_fAzimuth / 360.f))*360.f;



	//clamp
	if (m_fElevation >= m_RadmaxElevation)
	{
		m_fElevation = m_RadmaxElevation;
	}
	else if (m_fElevation <= m_RadminElevation)
	{
		m_fElevation = m_RadminElevation;
	}
}

void CCamera_Default::SphericalCoordinatesTranslateRadius(_float newRadius)
{
	m_fRadius += newRadius;

	if (m_fRadius >= m_maxRadius)
	{
		m_fRadius = m_maxRadius;
	}
	else if (m_fRadius <= m_minRadius)
	{
		m_fRadius = m_minRadius;
	}
}

_vector CCamera_Default::toCartesian()
{
	m_fT = m_fRadius*cosf(m_fElevation);
	_vector vCameraPos = XMVectorSet(m_fT*cosf(m_fAzimuth), m_fRadius*sinf(m_fElevation), m_fT*sinf(m_fAzimuth), 1.f);
	vCameraPos = XMVectorSetW(vCameraPos, 1.f);

	return vCameraPos;

}

void CCamera_Default::CartesianToLocal(_fvector vTargetPos)
{

	_float t, e, a;



	_float posy = XMVectorGetY(vTargetPos);
	_float posx = XMVectorGetX(vTargetPos);

	e = asinf(posy / m_fRadius);
	t = m_fRadius*cosf(e);
	a = acosf(posx / t);

	m_fT = t;
	m_fAzimuth = a;
	m_fElevation = e;



}


void CCamera_Default::Camera_Field(_double TimeDelta)
{

	_long MouseMoveX = 0, MouseMoveY = 0, MouseMoveWheel = 0; //방위각,앙각 
	_vector vCameraPos;

	_vector vPlayerPos = m_pPlayer_Manager->Get_FieldPlayer()->Get_PlayerPos();


	if (MouseMoveX = -m_pGameInstance->Get_DIMMoveState(CInput_Device::DIMM_X))
	{


	}
	if (MouseMoveY = m_pGameInstance->Get_DIMMoveState(CInput_Device::DIMM_Y))
	{

	}
	if (MouseMoveWheel = -m_pGameInstance->Get_DIMMoveState(CInput_Device::DIMM_WHEEL))
	{

	}
	SphericalCoordinatesTranslateRadius((_float)MouseMoveWheel*1.2f*(_float)TimeDelta*0.2f);
	SphericalCoordinatesRotate((_float)MouseMoveX*(_float)TimeDelta*m_fMouseSensitivity, (_float)MouseMoveY*(_float)TimeDelta*m_fMouseSensitivity);
	vCameraPos = toCartesian() + vPlayerPos;
	XMStoreFloat3(&m_vCameraEye,vPlayerPos);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetW(vCameraPos, 1.f));
	m_pTransformCom->LookAt(XMVectorSetY(vPlayerPos, XMVectorGetY(vPlayerPos) + m_fCameraLookatPlayerHeight));
	XMStoreFloat3(&m_vCameraAt, XMVectorSetY(vPlayerPos, XMVectorGetY(vPlayerPos) + m_fCameraLookatPlayerHeight));

	//else {
	//	//_vector vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	//	vCameraPos = toCartesian() + vPlayerPos;
	//	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetW(vCameraPos, 1.f));
	//	m_pTransformCom->LookAt(XMVectorSetY(vPlayerPos, XMVectorGetY(vPlayerPos) + m_fCameraLookatPlayerHeight));
	//	XMStoreFloat3(&m_vCameraAt, XMVectorSetY(vPlayerPos, XMVectorGetY(vPlayerPos) + m_fCameraLookatPlayerHeight));

	//}

}

void CCamera_Default::Camera_Battle(_double TimeDelta)
{

	_long MouseMoveX = 0, MouseMoveY = 0, MouseMoveWheel = 0; //방위각,앙각 
	_vector vCameraPos;

	_vector vPlayerPos = m_pPlayer_Manager->Get_MainPlayer()->Get_PlayerPos();
	_vector vTargetPos = m_pPlayer_Manager->Get_MainPlayer()->Get_TargetPos(); 

	if (!m_bBattleStart)
	{
		m_vPreCameraAt = m_vCameraAt;
		m_vPreCameraEye = m_vCameraEye;
		m_vCurCameraAt = m_vCameraAt;
		m_vCurCameraEye = m_vCameraEye;

		////lerp가 100다될떄까지 
		//
		//_vector vPreAt = XMLoadFloat3(&m_vCameraAt);  //이전 카메라의 at;
		//_vector vPreEye = XMLoadFloat3(&m_vCameraEye);  //이전 카메라의 eye;
		//

		m_bBattleStart = true;
	}

	 _float fSpeed = XMVectorGetX(XMVector3Length(m_pPlayer_Manager->Get_MainPlayer()->Get_PlayerPos() - m_pTransformCom->Get_State(CTransform::STATE_POSITION)));
	 
	 
	 m_fEyeLerp += TimeDelta *pow(fSpeed, 1.5)*0.5f;
	if (m_fEyeLerp >= 1.f)
	 {
		m_fEyeLerp = 0.f;
		m_vPreCameraEye = m_vCurCameraEye;
		 XMStoreFloat3(&m_vCurCameraEye, vPlayerPos); //현재 프레임의 알펜 위치저장 

	 }



	fSpeed = XMVectorGetX(XMVector3Length(m_pPlayer_Manager->Get_MainPlayer()->Get_TargetPos() -  XMLoadFloat3(&m_vCameraAt)));

	m_fAtLerp += TimeDelta *pow(fSpeed, 1.5)*0.2f;
	if (m_fAtLerp >= 1.f)
	{
		m_fAtLerp = 0.f;
		m_vPreCameraAt = m_vCurCameraAt;
		XMStoreFloat3(&m_vCurCameraAt, vTargetPos); //현재 프레임의 알펜 위치저장 

	}

    



	

	 //_float fSpeed = XMVectorGetX(XMVector3Length(vPlayerPos - m_pTransformCom->Get_State(CTransform::STATE_POSITION)));
	 //m_fPlayerPosLerp += TimeDelta *pow(fSpeed, 1.5);
	 //if (m_fPlayerPosLerp >= 1.f)
	 //{
		// m_fPlayerPosLerp = 0.f;
		// m_vPrePlayerPos = m_vCurPlayerPos;   //전프레임의 알펜 저장 
		// XMStoreFloat3(&m_vCurPlayerPos, vPlayerPos); //현재 프레임의 알펜 위치저장 

	 //}

	 

	

	if (MouseMoveX = -m_pGameInstance->Get_DIMMoveState(CInput_Device::DIMM_X))
	{


	}
	if (MouseMoveY = m_pGameInstance->Get_DIMMoveState(CInput_Device::DIMM_Y))
	{

	}
	if (MouseMoveWheel = -m_pGameInstance->Get_DIMMoveState(CInput_Device::DIMM_WHEEL))
	{

	}
	SphericalCoordinatesTranslateRadius((_float)MouseMoveWheel*1.2f*(_float)TimeDelta*0.2f);
	SphericalCoordinatesRotate((_float)MouseMoveX*(_float)TimeDelta*m_fMouseSensitivity, (_float)MouseMoveY*(_float)TimeDelta*m_fMouseSensitivity);
	vCameraPos = toCartesian() + XMVectorLerp(XMLoadFloat3(&m_vPreCameraEye), XMLoadFloat3(&m_vCurCameraEye), m_fEyeLerp);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetW(vCameraPos, 1.f));
	//m_pTransformCom->LookAt(XMVectorLerp(XMLoadFloat3(&m_vPreCameraAt), XMLoadFloat3(&m_vCurCameraAt), m_fAtLerp));
	m_pTransformCom->LookAt(vPlayerPos);

	XMStoreFloat3(&m_vCameraAt, XMVectorLerp(XMLoadFloat3(&m_vPreCameraAt), XMLoadFloat3(&m_vCurCameraAt), m_fAtLerp));

}

void CCamera_Default::TagetChange()
{
}

CCamera_Default * CCamera_Default::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CCamera_Default*	pInstance = new CCamera_Default(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CCamera_Default"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Default::Clone(void * pArg)
{
	CCamera_Default*	pInstance = new CCamera_Default(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CCamera_Default"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Default::Free()
{
	__super::Free();

}
