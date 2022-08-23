#include "stdafx.h"
#include "..\Public\Button_Super.h"

CButton_Super::CButton_Super(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CUI(pDevice, pDeviceContext)
{
}

CButton_Super::CButton_Super(const CButton_Super& rhs)
	:CUI(rhs)
{
}

HRESULT CButton_Super::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		MSG_BOX(L"Failed To CButton_Super : NativeConstruct_Prototype : NativeConstruct_Prototype");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CButton_Super::NativeConstruct(void* pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		MSG_BOX(L"Failed To CButton_Super : NativeConstruct : NativeConstruct");
		return E_FAIL;
	}
	return S_OK;
}

void CButton_Super::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	Compute_Rect();
}

void CButton_Super::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT CButton_Super::Render()
{
	if (FAILED(__super::Render()))
	{
		MSG_BOX(L"Failed To CButton_Super : Render : Render");
		return E_FAIL;
	}
	return S_OK;
}

void CButton_Super::Compute_Rect()
{
	m_tRect.left = _long(m_tUIInfo.fX- m_tUIInfo.fCX * 0.5f);
	m_tRect.top = _long(m_tUIInfo.fY - m_tUIInfo.fCY * 0.5f);
	m_tRect.right = _long(m_tUIInfo.fX + m_tUIInfo.fCX * 0.5f);
	m_tRect.bottom = _long(m_tUIInfo.fY + m_tUIInfo.fCY * 0.5f);
}

void CButton_Super::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}