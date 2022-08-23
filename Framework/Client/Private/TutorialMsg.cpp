#include "stdafx.h"
#include "TutorialMsg.h"

//전투
TutorialMsg::TutorialMsg(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

TutorialMsg::TutorialMsg(const TutorialMsg& rhs)
	:CUI(rhs)
{
}

HRESULT TutorialMsg::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT TutorialMsg::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}	

	
	m_iSprite = ((TMSGDESC*)pArg)->kind;
	m_iShaderPass = 3;

	if (FAILED(SetUp_Texture()))
	{
		return E_FAIL;
	}



	return S_OK;
}

void TutorialMsg::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	CGameInstance* pGameInstnace = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstnace);

	//일단은 시간에 맞춰 켰다 껐다를 하겠다.
	if (m_iSprite >= 3)
		m_iSprite = 0;

	time += (_float)TimeDelta;
	if(changeSprite)
	{
		m_iSprite += 1;
		changeSprite = false;
	}

	if (time > 3)
	{
		m_isRender = true;
		
		if (GetKeyState('E') & 0x8000)
		{
		
			time = 0;
			changeSprite = true;
			/*m_iSprite += 1;*/
		}
	}
	else
		m_isRender = false;


	Safe_Release(pGameInstnace);
}

void TutorialMsg::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT TutorialMsg::Render()
{

	if (m_isRender) {
		if (FAILED(__super::Render()))
		{
			return E_FAIL;
		}
	}
	return S_OK;
}

HRESULT TutorialMsg::SetUp_Texture()
{
	if (FAILED(__super::SetUp_Components(L"Com_Texture", LEVEL_STATIC, L"Prototype_Component_Texture_TutorialMsg", (CComponent**)&m_pTextureCom)))
	{
		MSG_BOX(L"Failed To TutorialMsg : SetUp_Texture : SetUp_Component(Com_Texture)");
		return E_FAIL;
	}
	return S_OK;
}

TutorialMsg * TutorialMsg::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	TutorialMsg* pInstance = new TutorialMsg(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(L"Failed To TutorialMsg : Create : pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * TutorialMsg::Clone(void * pArg)
{
	TutorialMsg* pInstance = new TutorialMsg(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To TutorialMsg : Clone : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void TutorialMsg::Free()
{
}
