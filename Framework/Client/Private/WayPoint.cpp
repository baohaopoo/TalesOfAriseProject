#include "stdafx.h"
#include "..\Public\WayPoint.h"
#include "Player.h"

CWayPoint::CWayPoint(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CWayPoint::CWayPoint(const CWayPoint & rhs)
	: CGameObject(rhs)
{
}

HRESULT CWayPoint::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSG_BOX(L"CWayPoint -> NativeConstruct_Prototype -> NativeConstruct_Prototype");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CWayPoint::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CWayPoint::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
}

void CWayPoint::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);

#ifdef _DEBUG
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (nullptr != m_pRendererCom &&
		true == pGameInstance->isInFrustum_WorldSpace(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 3.f))
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);

	RELEASE_INSTANCE(CGameInstance);
#endif // _DEBUG
}

HRESULT CWayPoint::Render()
{
#ifdef _DEBUG
	m_pSphereCom->Render();
#endif // _DEBUG

	return S_OK;
}

HRESULT CWayPoint::SetUp_Components(void * pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	WayPoint_DESC Desc = *((WayPoint_DESC*)pArg);

	// 방향 설정
	m_fCameraLook = Desc.fDir;

	// 번호 설정
	m_iWayPointNumber = Desc.iPointNumber;

	// 위치 설정
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(Desc.fPos.x, Desc.fPos.y, Desc.fPos.z, 1.f));



	// 원형 충돌체 생성
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	/* For.Com_SPHERE */
	ColliderDesc.vPosition = _float3(0.f, 0.5f, 0.f);		// 상대적 위치 및 반지름 생성
	ColliderDesc.fRadius = 1.f;
	if (FAILED(__super::SetUp_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
		return E_FAIL;

	


	return S_OK;
}

HRESULT CWayPoint::SetUp_ConstantTable()
{
	return S_OK;
}

HRESULT CWayPoint::Activate_WayPoint(CPlayer * pPlayer)
{
	if (nullptr == m_pDestWayPoint)
		return E_FAIL;

	// 플레이어의 위치 변경

	// 도착 지점의 위치를 가져온다.
	_vector vDestPos = dynamic_cast<CTransform*>(m_pDestWayPoint->Get_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);

	// 플레이어의 위치를 바꾼다.
	dynamic_cast<CTransform*>(pPlayer->Get_Component(TEXT("Com_Transform")))->Set_State(CTransform::STATE_POSITION, vDestPos);





	// 플레이어가 바라보는 방향 변경

	// 도착지점의 방향을 가져온다.
	_float3 fLook = m_pDestWayPoint->Get_CameraLook();

	// 플레이어가 바라보는 방향을 바꾼다.
	dynamic_cast<CTransform*>(pPlayer->Get_Component(TEXT("Com_Transform")))->Look(XMVectorSet(fLook.x, fLook.y, fLook.z, 1.f));



	// 카메라의 위치 변경

	// 여기서 카메라의 위치를 조정한다.
	return S_OK;
}

CWayPoint * CWayPoint::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CWayPoint* pInstance = new CWayPoint(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(L"CWayPoint -> Create -> pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CWayPoint::Clone(void * pArg)
{
	CWayPoint* pInstance = new CWayPoint(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(L"CWayPoint -> Clone -> pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CWayPoint::Free()
{
}