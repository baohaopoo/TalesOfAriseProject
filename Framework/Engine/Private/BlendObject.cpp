#include "..\Public\BlendObject.h"

#include "GameInstance.h"
//#include "Transform.h"



CBlendObject::CBlendObject(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{
	
}

CBlendObject::CBlendObject(const CBlendObject & rhs)
	: CGameObject(rhs.m_pDevice, rhs.m_pDeviceContext)
{

}

HRESULT CBlendObject::NativeConstruct_Prototype()
{

	return S_OK;
}

HRESULT CBlendObject::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	return S_OK;
}

void CBlendObject::Tick(_double TimeDelta)
{
	CGameObject::Tick(TimeDelta);
}

void CBlendObject::LateTick(_double TimeDelta)
{
	CGameObject::LateTick(TimeDelta);

}

HRESULT CBlendObject::Render()
{
	return S_OK;
}

void CBlendObject::Compute_CamDistance()
{
	CGameInstance * pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	m_fCamDistance = XMVectorGetX(XMVector3Length(m_pTransformCom->Get_State(CTransform::STATE_POSITION) - pGameInstance->Get_CamPosition()));

	Safe_Release(pGameInstance);
}

void CBlendObject::Free()
{
	__super::Free();


}
