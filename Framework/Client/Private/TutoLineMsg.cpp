#include "stdafx.h"
#include "TutoLineMsg.h"

//전투
TutoLineMsg::TutoLineMsg(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

TutoLineMsg::TutoLineMsg(const TutoLineMsg& rhs)
	:CUI(rhs)
{
}

HRESULT TutoLineMsg::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT TutoLineMsg::NativeConstruct(void * pArg)
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
	m_iShaderPass = 3;// 14;//3;
	m_fDistanceToCamera = 103;
	
	return S_OK;
}

void TutoLineMsg::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	CGameInstance* pGameInstnace = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstnace);
	
	


	time += (_float)TimeDelta;
	if (time > 4)
	{
		Set_Dead(true);
	}


	Safe_Release(pGameInstnace);

}

void TutoLineMsg::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT TutoLineMsg::Render()
{


	//잠만 보류
	//if (FAILED(m_pShaderCom->Set_RawValue("tutolinemsgrgb", &rgb, sizeof(float))))
	//	return E_FAIL;




	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT TutoLineMsg::SetUp_Texture()
{
	if (FAILED(__super::SetUp_Components(L"Com_Texture", LEVEL_STATIC, L"Prototype_Component_Texture_TutoLineMsg", (CComponent**)&m_pTextureCom)))
	{
		MSG_BOX(L"Failed To TutoLineMsg : SetUp_Texture : SetUp_Component(Com_Texture)");
		return E_FAIL;
	}
	return S_OK;
}

TutoLineMsg * TutoLineMsg::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	TutoLineMsg* pInstance = new TutoLineMsg(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(L"Failed To TutoLineMsg : Create : pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * TutoLineMsg::Clone(void * pArg)
{
	TutoLineMsg* pInstance = new TutoLineMsg(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To TutoLineMsg : Clone : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void TutoLineMsg::Free()
{
	__super::Free();

}
