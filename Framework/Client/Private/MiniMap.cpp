#include "stdafx.h"
#include "MiniMap.h"

//ÀüÅõ
MiniMap::MiniMap(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

MiniMap::MiniMap(const MiniMap& rhs)
	:CUI(rhs)
{
}

HRESULT MiniMap::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT MiniMap::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}	

	
	m_iSprite = ((MINIMAPDESC*)pArg)->kind;

	m_iSprite = 0;
	m_iShaderPass =3;

	if (FAILED(SetUp_Texture()))
	{
		return E_FAIL;
	}



	return S_OK;
}

void MiniMap::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	CGameInstance* pGameInstnace = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstnace);

	Safe_Release(pGameInstnace);
}

void MiniMap::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT MiniMap::Render()
{

	 
		if (FAILED(__super::Render()))
		{
			return E_FAIL;
		}
	
	return S_OK;
}

HRESULT MiniMap::SetUp_Texture()
{
	if (FAILED(__super::SetUp_Components(L"Com_Texture", LEVEL_STATIC, L"Prototype_Component_Texture_MiniMap", (CComponent**)&m_pTextureCom)))
	{
		MSG_BOX(L"Failed To MiniMap : SetUp_Texture : SetUp_Component(Com_Texture)");
		return E_FAIL;
	}
	return S_OK;
}

MiniMap * MiniMap::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	MiniMap* pInstance = new MiniMap(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(L"Failed To MiniMap : Create : pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * MiniMap::Clone(void * pArg)
{
	MiniMap* pInstance = new MiniMap(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To MiniMap : Clone : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void MiniMap::Free()
{
}
