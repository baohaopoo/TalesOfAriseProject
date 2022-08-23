#include "stdafx.h"
#include "..\Public\Enemy.h"
#include "GameInstance.h"
#include "Player.h"
#include "Navigation.h"

CEnemy::CEnemy(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
	, m_pBattle_Manager(CBattle_Manager::GetInstance())
	, m_pPlayer_Manager(CPlayer_Manager::GetInstance())
{

}

CEnemy::CEnemy(const CEnemy & rhs)
	: CGameObject(rhs)
	, m_pBattle_Manager(CBattle_Manager::GetInstance())
	, m_pPlayer_Manager(CPlayer_Manager::GetInstance())
	, m_eMapType(rhs.m_eMapType)
{
}

HRESULT CEnemy::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEnemy::NativeConstruct(void * pArg, CTransform::TRANSFORMDESC* pTransformDesc)
{

	if (FAILED(__super::NativeConstruct(pArg, pTransformDesc)))
	{
		MSG_BOX(L"Failed To CEnemy : NativeConstruct : NativeConstruct");
		return E_FAIL;
	}

	if (nullptr != pArg)
	{
		EnemyDesc Desc = *(EnemyDesc*)pArg;

		m_eMapType = Desc.eMapType;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, Desc.vPos);
	}

	return S_OK;
}

void CEnemy::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	if (!m_bBattle)
	{
		if (m_pSphereCom->Collsion_Sphere((CCollider*)m_pPlayer_Manager->Get_FieldPlayer()->Get_Component(TEXT("Com_SPHERE"))))
		{
			m_pBattle_Manager->Battle_Enter(this);
			m_bDead = true;
		}
	}

	else
	{
		Compute_Gravity(TimeDelta);
	}

}

void CEnemy::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);

#ifdef _DEBUG
	m_pRendererCom->Add_DebugComponent(m_pOBBCom);
	m_pRendererCom->Add_DebugComponent(m_pSphereCom);
#endif // _DEBUG

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	RELEASE_INSTANCE(CGameInstance);

	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
	}

}

void CEnemy::Compute_Collider()
{
	m_pAttackSphereCom->Set_On(m_bOnAttackCollider);
}

HRESULT CEnemy::Render()
{

#ifdef _DEBUG
	m_pOBBCom->Render();
	m_pSphereCom->Render();
#endif // _DEBUG

	return S_OK;
}

void CEnemy::Compute_Gravity(_double TimeDelta)
{

	if (m_bBattle)
	{

		//m_fPowerUp = m_HitDamageInfo.fPowerUp*TimeDelta;

		_float Gravity = 0.052f;


		m_HitDamageInfo.fPowerUp -= (_float)TimeDelta*Gravity;



		//m_fPowerUp -= TimeDelta;

		_vector  vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float vPosY = XMVectorGetY(vPos);




		vPosY += m_HitDamageInfo.fPowerUp;
		XMVectorSet(0.f, 1.f, 0.f, 1.f)*m_HitDamageInfo.fPowerUp;
		if (vPosY <= 0)
		{

			vPosY = 0;

		}
		vPos = XMVectorSetY(vPos, vPosY);

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vPos, vPosY));











		if (m_HitDamageInfo.fPowerBack > 0.f)
		{
			m_HitDamageInfo.fPowerBack -= TimeDelta*Gravity;
			Set_EnemyPos(Get_EnemyPos() + XMLoadFloat3(&m_vKnockBackDir)*m_HitDamageInfo.fPowerBack);



		}

	}




	// 네비 메쉬를 타고 있을 경우, y값만 조정
	if (nullptr != m_pNaviCom) {
		// y값 조정
		m_pNaviCom->Find_My_Cell(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
		m_pTransformCom->Set_Height(m_pTransformCom->Get_Height(m_pNaviCom));
	}


}

CNavigation * CEnemy::SetUp_Navigation(const TCHAR * pTagNaviCompoent)
{
	// 기존에 있는 Navi컴포넌트를 삭제한다.
	if (nullptr != m_pNaviCom)
		Safe_Release(m_pNaviCom);

	Delete_Component(TEXT("Com_Navigation"));

	// 인자로 받은 Navi Component를 복제하여 해당 정보를
	CNavigation::NAVIDESC NaviDesc;
	NaviDesc.iCurrentIndex = 0;
	NaviDesc.pParent = nullptr;

	if (!_tcscmp(pTagNaviCompoent, TEXT("Prototype_Component_Navigation_Map_GrandYork_Battle_02"))) {

		_float4x4 matWorld4x4;
		XMStoreFloat4x4(&matWorld4x4, XMMatrixIdentity());

		_float3 fPos = _float3(1000.f, 0.f, 0.f);
		memcpy(&matWorld4x4.m[CTransform::STATE_POSITION], &fPos, sizeof(_float3));

		NaviDesc.NaviWorldMatrix = XMLoadFloat4x4(&matWorld4x4);
	}
	else if (!_tcscmp(pTagNaviCompoent, TEXT("Prototype_Component_Navigation_Map_GrandYork_Battle_04"))) {

		_float4x4 matWorld4x4;
		XMStoreFloat4x4(&matWorld4x4, XMMatrixIdentity());

		_float3 fPos = _float3(2000.f, 0.f, 0.f);
		memcpy(&matWorld4x4.m[CTransform::STATE_POSITION], &fPos, sizeof(_float3));

		NaviDesc.NaviWorldMatrix = XMLoadFloat4x4(&matWorld4x4);
	}
	else {
		NaviDesc.NaviWorldMatrix = XMMatrixIdentity();
	}

	if (FAILED(__super::SetUp_Components(TEXT("Com_Navigation"), LEVEL_STATIC, pTagNaviCompoent, (CComponent**)&m_pNaviCom, &NaviDesc)))
		return nullptr;

	return m_pNaviCom;
}

_bool CEnemy::Delete_NaviCom(void)
{
	// 기존에 있는 Navi컴포넌트를 삭제한다.
	if (nullptr != m_pNaviCom)
		Safe_Release(m_pNaviCom);

	Delete_Component(TEXT("Com_Navigation"));

	return (nullptr == m_pNaviCom);
}

void CEnemy::Free()
{
	__super::Free();

	Safe_Release(m_pSphereCom);
	Safe_Release(m_pOBBCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pNaviCom);
}
