#include "stdafx.h"
#include"ItemList.h"
//이게 이제 버튼 누르면 반짝거리는 이미지임.
ItemList::ItemList(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CButton_Super(pDevice, pDeviceContext)
{
}

ItemList::ItemList(const ItemList& rhs)
	: CButton_Super(rhs)
{
}

HRESULT ItemList::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT ItemList::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}



	if (FAILED(SetUp_Texture()))
	{
		return E_FAIL;
	}


	m_iShaderPass = 24;
	m_fDistanceToCamera = 99.9;

	return S_OK;
}

void ItemList::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	CGameInstance* pGameInstnace = CGameInstance::GetInstance();
	rgb= 0.8;
}
void ItemList::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT ItemList::Render()
{
	if (m_isRender) {
		if (FAILED(m_pShader->Set_RawValue("ItemListrgb", &rgb, sizeof(float))))
			return E_FAIL;


		if (FAILED(__super::Render()))
		{
			return E_FAIL;
		}
	}
	return S_OK;
}

HRESULT ItemList::SetUp_Texture()
{
	if (FAILED(__super::SetUp_Component(L"Com_Texture", LEVEL_STATIC, L"Prototype_Component_Texture_ItemList", (CComponent**)&m_pTexture)))
	{
		MSG_BOX(L"texture오류다임마.");
		return E_FAIL;
	}
}



ItemList * ItemList::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	ItemList* pInstance = new ItemList(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(L"Failed To ItemList : Create : pInstance->NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * ItemList::Clone(void * pArg)
{
	ItemList* pInstance = new ItemList(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To ItemList : Clone : pInstance->NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void ItemList::Free()
{
}
