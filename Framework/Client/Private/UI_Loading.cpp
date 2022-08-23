#include"stdafx.h"
#include "../Public/UI_Loading.h"
#include "Level_Loading.h"

CUI_Loading::CUI_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

CUI_Loading::CUI_Loading(const CUI_Loading& rhs)
	:CUI(rhs)
{
}

HRESULT CUI_Loading::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CUI_Loading::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}
	if (FAILED(SetUp_Texture()))
	{
		return E_FAIL;
	}
	m_tUIInfo.fCX = 1280;
	m_tUIInfo.fCY = 720;
	m_tUIInfo.fX = 640;
	m_tUIInfo.fY = 360;

	m_iShaderPass = 10;
	return S_OK;
}

void CUI_Loading::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	CGameInstance* pGameInstnace = CGameInstance::GetInstance();

	if(!m_isFirst)
	rgb += 0.008f;
	
	if (rgb > 1) {
		rgb = 1;
		m_isFirst = true;
	}
	
	if(pGameInstnace->Key_Down(DIK_SPACE))
	{
		m_isFirst = false;
		
		
	}
	


}

void CUI_Loading::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT CUI_Loading::Render()
{

	if (FAILED(m_pShader->Set_RawValue("rgb", &rgb, sizeof(float))))
		return E_FAIL;



	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CUI_Loading::SetUp_Texture()
{
	if (FAILED(__super::SetUp_Component(L"Com_Texture", LEVEL_LOBY, L"Prototype_Component_Texture_LobbyUI", (CComponent**)&m_pTexture)))
	{
		MSG_BOX(L"Failed To CUI_Loading : SetUp_Texture : SetUp_Component(Com_Texture)");
		return E_FAIL;
	}
	return S_OK;
}

CUI_Loading * CUI_Loading::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CUI_Loading* pInstance = new CUI_Loading(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(L"Failed To CUI_Loading : Create : pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUI_Loading::Clone(void * pArg)
{
	CUI_Loading* pInstance = new CUI_Loading(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To CUI_Loading : Clone : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_Loading::Free()
{
}
