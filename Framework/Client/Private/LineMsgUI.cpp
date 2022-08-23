#include "stdafx.h"
#include "LineMsgUI.h"

//전투
LineMsgUI::LineMsgUI(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

LineMsgUI::LineMsgUI(const LineMsgUI& rhs)
	:CUI(rhs)
{
}

HRESULT LineMsgUI::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT LineMsgUI::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}

	m_iSprite = ((MSGLINEUIDESC*)pArg)->sprite;
	if (FAILED(SetUp_Texture()))
	{
		return E_FAIL;
	}


	m_iShaderPass = 3;
	return S_OK;
}

void LineMsgUI::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	CGameInstance* pGameInstnace = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstnace);
	time += (_float)TimeDelta;
	
	if (GetKeyState(VK_F8) & 0x8000)
	{
		Set_Dead(1);
	}


	if (time > 2)
	{
		//가중치 빛의 속도.
		acc += 2;
		m_tUIInfo.fX += acc;
		rgb -= 0.3f;

		//Setdead처리 하기.	

		if (acc > 14)
			Set_Dead(true);
	}

	

	Safe_Release(pGameInstnace);

}

void LineMsgUI::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT LineMsgUI::Render()
{


	//잠만 보류
	if (FAILED(m_pShaderCom->Set_RawValue("linemsgrgb", &rgb, sizeof(float))))
		return E_FAIL;


	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT LineMsgUI::SetUp_Texture()
{
	if (FAILED(__super::SetUp_Components(L"Com_Texture", LEVEL_STATIC, L"Prototype_Component_Texture_LineMsgUI", (CComponent**)&m_pTextureCom)))
	{
		MSG_BOX(L"Failed To LineMsgUI : SetUp_Texture : SetUp_Component(Com_Texture)");
		return E_FAIL;
	}
	return S_OK;
}

LineMsgUI * LineMsgUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	LineMsgUI* pInstance = new LineMsgUI(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(L"Failed To LineMsgUI : Create : pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * LineMsgUI::Clone(void * pArg)
{
	LineMsgUI* pInstance = new LineMsgUI(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To LineMsgUI : Clone : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void LineMsgUI::Free()
{
	__super::Free();

}
