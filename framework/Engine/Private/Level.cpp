#include "..\Public\Level.h"

CLevel::CLevel(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: m_pDevice(pDevice)
	, m_pDeviceContext(pDeviceContext)
{
	Safe_AddRef(m_pDeviceContext);
	Safe_AddRef(m_pDevice);
}

HRESULT CLevel::NativeConstruct()
{
	return S_OK;
}

void CLevel::Tick(_double TimeDelta)
{
}

HRESULT CLevel::Render()
{
	return S_OK;
}

void CLevel::Free()
{
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
}
