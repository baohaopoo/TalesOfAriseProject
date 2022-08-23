#include"stdafx.h"
#include"UIStatus.h"
#include "UI_Manager.h"
#include"Battle_Manager.h"
CUIStatus::CUIStatus(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

CUIStatus::CUIStatus(const CUIStatus& rhs)
	: CUI(rhs)
{
}

HRESULT CUIStatus::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CUIStatus::NativeConstruct(void * pArg)
{


	if (FAILED(SetUp_Texture()))
	{
		return E_FAIL;
	}

	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}


	m_iSprite = ((STATUSDESC*)pArg)->kind;


	m_fDistanceToCamera = 89.5;

	m_iShaderPass = 19;

	return S_OK;
}

void CUIStatus::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	//m_fPercentage = cp / 700;

	//if (cp <= 10)
	//	cp = 10;

	//if (cp > 680)
	//	cp = 680;

	//if (cp < 300) {
	//	m_isRed = true;
	//}
	//else
	//	m_isRed = false;

	//if (GetKeyState('H') & 0x8000)
	//{

	//	cp -= 10;

	//}
	//if (GetKeyState('J') & 0x8000)
	//{

	//	cp += 10;
	//}


	CBattle_Manager* pBattleManager = CBattle_Manager::GetInstance();
	Safe_AddRef(pBattleManager);

	////10퍼보다 작으면 꽉채워라.
	if (pBattleManager->GetCP() <= 10)
		pBattleManager->SetCP(pBattleManager->GetMAXCP());

	maxcp = pBattleManager->GetMAXCP();
	////꽉차면 그냥 꽉차게 두고
	//if (pBattleManager->GetCP() >  pBattleManager->GetMAXCP())
	//	pBattleManager->SetCP( pBattleManager->GetMAXCP());
	cp = pBattleManager->GetCP();
	m_fPercentage = cp / pBattleManager->GetMAXCP();


	//40퍼정도 작으면 빨간색이 되어라.
	if (pBattleManager->GetCP()<70/*pBattleManager->GetMAXCP() - pBattleManager->GetMAXCP()*0.002*/) {
		m_isRed = true;
	}
	else
		m_isRed = false;

	Safe_Release(pBattleManager);
}

void CUIStatus::LateTick(_double TimeDelta)
{

	__super::LateTick(TimeDelta);

}

HRESULT CUIStatus::Render()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	m_pShaderCom->Set_RawValue("g_fPercentage", &m_fPercentage, sizeof(_float));
	m_pShaderCom->Set_RawValue("m_isRed", &m_isRed, sizeof(bool));
	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}

	wsprintf(HitEffect, TEXT(" / %d"), maxcp/*pBattleManager->GetMAXCP()*/);
	////뒤에 있는 숫자를 보자
	if (FAILED(pGameInstance->Render_Fonts(TEXT("Font_Javan40"), HitEffect, _float2(g_iWinCX / 2 + 790, 450/*g_iWinCX / 2 + 700, g_iWinCY / 2 - 200*/), XMVectorSet(0.9f, 1.f, 0.7f, 1.f))))
		return E_FAIL;

	wsprintf(HitCurrent, TEXT("%d"), cp/*pBattleManager->GetMAXCP()*/);
	////뒤에 있는 숫자를 보자
	if (FAILED(pGameInstance->Render_Fonts(TEXT("Font_Javan50"), HitCurrent, _float2(g_iWinCX / 2 + 700, 390/*g_iWinCX / 2 + 700, g_iWinCY / 2 - 200*/), XMVectorSet(0.9f, 1.f, 0.7f, 1.f))))
		return E_FAIL;


	return S_OK;
}

HRESULT CUIStatus::SetUp_Texture()
{
	//dd.png 썼음 조심. 뭐가문제라면 얘가 범인일지도
	if (FAILED(__super::SetUp_Components(L"Com_Texture", LEVEL_STATIC, L"Prototype_Component_Texture_GlowUI", (CComponent**)&m_pTextureCom)))
	{


		MSG_BOX(L"Failed To UIName : SetUp_Texture : SetUp_Component(Com_Texture)");
		return E_FAIL;
	}
	return S_OK;
}

CUIStatus * CUIStatus::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CUIStatus* pInstance = new CUIStatus(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(L"Failed To UINAme : Create : pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUIStatus::Clone(void * pArg)
{
	CUIStatus* pInstance = new CUIStatus(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To UIName : Clone : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUIStatus::Free()
{
	__super::Free();

}
