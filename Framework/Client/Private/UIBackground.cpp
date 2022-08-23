#include "stdafx.h"
#include "..\Public\UIBackground.h"
#include "GameInstance.h"


//0. 로고
//1. 로비
//2. 로딩
//3. tutorial

CUIBackground::CUIBackground(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

CUIBackground::CUIBackground(const CUIBackground& rhs)
	: CUI(rhs)
{
}

HRESULT CUIBackground::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CUIBackground::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}

	m_iSprite = ((UIBACKDESC*)pArg)->kind;


	if (m_iSprite == 3) rgb = 1;
	if (FAILED(SetUp_Texture()))
	{
		return E_FAIL;
	}
	m_tUIInfo.fCX = g_iWinCX;
	m_tUIInfo.fCY = g_iWinCY;
	m_tUIInfo.fX = g_iWinCX / 2;
	m_tUIInfo.fY = g_iWinCY / 2;

	//m_fDistanceToCamera = 10;

	return S_OK;
}

void CUIBackground::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	CGameInstance* pGameInstnace = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstnace);

	if (m_iSprite == 3)
	{//인벤토리 이미지만 검정색으로 변하기
		m_iShaderPass = 2;
		rgb -= 0.008f;
	}
	else if (m_iSprite == 4) {
		//스킬셋 이미지 pass원래 디졸브로 하려다가 이렇게 됨.
		//다시 3내지 2로 바꿔도 됨.
		m_iShaderPass = 13;

	}
	else
	{
		m_iShaderPass = 2;
		if (!m_isFirst)
			rgb += 0.008f;

		if (rgb > 1) {
			rgb = 1;
			m_isFirst = true;

		}
	}


	//if (pGameInstnace->Key_Down(DIK_SPACE))
	//{
	//	m_isFirst = false;
	//}


	Safe_Release(pGameInstnace);
}

void CUIBackground::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);


}

HRESULT CUIBackground::Render()
{

	if (FAILED(m_pShaderCom->Set_RawValue("backuirgb", &rgb, sizeof(float))))
		return E_FAIL;


	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CUIBackground::SetUp_Texture()
{
	if (FAILED(__super::SetUp_Components(L"Com_Texture", LEVEL_STATIC, L"Prototype_Component_Texture_BackGround", (CComponent**)&m_pTextureCom)))
	{
		MSG_BOX(L"Failed To CUIBackground : SetUp_Texture : SetUp_Component(Com_Texture)");
		return E_FAIL;
	}

	return S_OK;
}

CUIBackground * CUIBackground::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CUIBackground* pInstance = new CUIBackground(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(L"Failed To CUIBackground : Create : pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUIBackground::Clone(void * pArg)
{
	CUIBackground* pInstance = new CUIBackground(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To CUIBackground : Clone : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUIBackground::Free()
{
	__super::Free();
}