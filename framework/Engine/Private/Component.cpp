#include "..\Public\Component.h"


CComponent::CComponent(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: m_pDevice(pDevice)
	, m_pDeviceContext(pDeviceContext)
	, m_isCloned(false)
{
	Safe_AddRef(m_pDeviceContext);
	Safe_AddRef(m_pDevice);
}

CComponent::CComponent(const CComponent & rhs)
	: m_pDevice(rhs.m_pDevice), m_pDeviceContext(rhs.m_pDeviceContext)
	, m_isCloned(true)
{
	Safe_AddRef(m_pDeviceContext);
	Safe_AddRef(m_pDevice);
}

HRESULT CComponent::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CComponent::NativeConstruct(void * pArg)
{
	return S_OK;
}

void CComponent::Free()
{
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
}
