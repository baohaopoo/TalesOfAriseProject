#include "..\Public\LandObject.h"
#include "GameInstance.h"

CLandObject::CLandObject(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CLandObject::CLandObject(const CLandObject & rhs)
	: CGameObject(rhs)
{
}

HRESULT CLandObject::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLandObject::NativeConstruct(void * pArg, CTransform::TRANSFORMDESC * pTransformDesc)
{
	if (FAILED(__super::NativeConstruct(pArg, pTransformDesc)))
		return E_FAIL;

	return S_OK;
}

void CLandObject::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
}

void CLandObject::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT CLandObject::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLandObject::SetUp_OnTerrain(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pBufferTag, _uint iIndex)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CVIBuffer*	pTerrainBuffer = (CVIBuffer*)pGameInstance->Get_Component(iLevelIndex, pLayerTag, pBufferTag);
	if (nullptr == pTerrainBuffer)
		return E_FAIL;

	CTransform*	pTerrainTransform = (CTransform*)pGameInstance->Get_Component(iLevelIndex, pLayerTag, CGameObject::m_pTransformTag);
	if (nullptr == pTerrainTransform)
		return E_FAIL;

	_vector		vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_float		fHeight = pTerrainBuffer->Compute_Height(pTerrainTransform, vPosition);

	vPosition = XMVectorSetY(vPosition, fHeight);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CLandObject::Free()
{
	__super::Free();

}
