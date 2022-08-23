#include "stdafx.h"
#include"UINumber.h"
#include"UIGlow.h"
CUINumber::CUINumber(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

CUINumber::CUINumber(const CUINumber& rhs)
	:CUI(rhs)
{
}

HRESULT CUINumber::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CUINumber::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}

	numKind = ((UINUMDESC*)pArg)->kind;


	if (FAILED(SetUp_Texture()))
	{
		return E_FAIL;
	}

	m_iSprite = 0;
	m_iShaderPass = 3;
	m_fDistanceToCamera = 89.3f;
	return S_OK;
}

void CUINumber::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	CGameInstance* pGameInstnace = CGameInstance::GetInstance();

	Safe_AddRef(pGameInstnace);
		if (numKind == 0) {
			if (m_isGlowFirst) {
				UIGlow::UIGLOWDESC uidesc;

				//glow UI추가.
				uidesc.tUIInfo.fCX = 90;
				uidesc.tUIInfo.fCY = 90;
				uidesc.tUIInfo.fX = m_tUIInfo.fX;
				uidesc.tUIInfo.fY = m_tUIInfo.fY;
				uidesc.kind = 0;
				//if (nullptr == (pGameInstnace->Add_GameObjectToLayer(LEVEL_GAME1, L"Layer_UI_Glow", TEXT("Prototype_GameObject_GlowUI"), &uidesc)))
				//	return;
				m_isGlowFirst = false;
			}

			//이건 숫자 크기 생성될때마다 크게하려고
			if (m_iSprite > 8)
				m_iSprite = 0;

			if (m_tUIInfo.fCX > 70) {
				m_iSprite++;
				m_tUIInfo.fCX = 60;
				m_tUIInfo.fCY = 60;
			}
			m_tUIInfo.fCX += 1;
			m_tUIInfo.fCY += 1;

		}
		else if (numKind == 1)
		{
			if (m_isGlowFirst) {
				UIGlow::UIGLOWDESC uidesc;

				//glow UI추가.
				uidesc.tUIInfo.fCX = 30;
				uidesc.tUIInfo.fCY = 30;
				uidesc.tUIInfo.fX = m_tUIInfo.fX;
				uidesc.tUIInfo.fY = m_tUIInfo.fY;
				uidesc.kind = 1;
				//if (nullptr == (pGameInstnace->Add_GameObjectToLayer(LEVEL_GAME1, L"Layer_UI_Glow", TEXT("Prototype_GameObject_GlowUI"), &uidesc)))
				//	return;
				m_isGlowFirst = false;
			}

			//이건 숫자 크기 생성될때마다 크게하려고
			if (m_iSprite > 8)
				m_iSprite = 0;

			if (m_tUIInfo.fCX > 30) {
				m_iSprite++;
				m_tUIInfo.fCX = 20;
				m_tUIInfo.fCY = 20;
			}
			m_tUIInfo.fCX += 1;
			m_tUIInfo.fCY += 1;
		}
		else if (numKind == 3)
		{

		}
		else if (numKind == 4)
			m_iSprite = 10;

		Safe_Release(pGameInstnace);

}

void CUINumber::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT CUINumber::Render()
{

	if (!m_isBattleOn) {

		//if (FAILED(m_pShaderCom->Set_RawValue("alphargb", &rgb, sizeof(float))))
		//	return E_FAIL;




		if (FAILED(__super::Render()))
		{
			return E_FAIL;
		}
	}
	return S_OK;
}

HRESULT CUINumber::SetUp_Texture()
{
	if (FAILED(__super::SetUp_Components(L"Com_Texture", LEVEL_STATIC, L"Prototype_Component_Texture_Font", (CComponent**)&m_pTextureCom)))
	{
		MSG_BOX(L"Failed To UINumber : SetUp_Texture : SetUp_Component(Com_Texture)");
		return E_FAIL;
	}
	return S_OK;
}

CUINumber * CUINumber::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CUINumber* pInstance = new CUINumber(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(L"Failed To UINumber : Create : pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUINumber::Clone(void * pArg)
{
	CUINumber* pInstance = new CUINumber(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To UINumber : Clone : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUINumber::Free()
{
	__super::Free();

}
