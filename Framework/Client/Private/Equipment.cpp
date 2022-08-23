#include "stdafx.h"
#include "..\Public\Equipment.h"
#include "GameInstance.h"

CEquipment::CEquipment(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{
}

CEquipment::CEquipment(const CEquipment & rhs)
	: CGameObject(rhs)
{
}

HRESULT CEquipment::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEquipment::NativeConstruct(void * pArg, CTransform::TRANSFORMDESC* pTransformDesc)
{
	if (FAILED(__super::NativeConstruct(pArg, pTransformDesc)))
	{
		MSG_BOX(L"Failed To Equpment : NativeConstruct : NativeConstruct");
		return E_FAIL;
	}

	return S_OK;
}

void CEquipment::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
}

void CEquipment::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);

	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
	}
}

HRESULT CEquipment::Render()
{
	return S_OK;
}

void CEquipment::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
}