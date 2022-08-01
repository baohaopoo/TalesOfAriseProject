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

void CBlendObject::Compute_CamDistance(CTransform * pTransform)
{
	//_float4x4			ViewMatrix;

	//m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	//D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	//_float3		vDir = *(_float3*)&ViewMatrix.m[3][0] - pTransform->Get_State(CTransform::STATE_POSITION);

	//m_fCamDistance = D3DXVec3Length(&vDir);
}

void CBlendObject::Free()
{
	__super::Free();


}
