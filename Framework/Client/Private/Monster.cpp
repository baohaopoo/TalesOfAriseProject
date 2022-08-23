#include "stdafx.h"
#include "..\Public\Monster.h"
#include "GameInstance.h"

CMonster::CMonster(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{

}

CMonster::CMonster(const CMonster & rhs)
	: CGameObject(rhs)
{
}

HRESULT CMonster::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster::NativeConstruct(void * pArg, CTransform::TRANSFORMDESC* pTransformDesc)
{

	if (FAILED(__super::NativeConstruct(pArg, pTransformDesc)))
	{
		MSG_BOX(L"Failed To CMonster : NativeConstruct : NativeConstruct");
		return E_FAIL;
	}

	return S_OK;
}

void CMonster::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);


}

void CMonster::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	RELEASE_INSTANCE(CGameInstance);

	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
	}

}

HRESULT CMonster::Render()
{
	return S_OK;
}


//HRESULT CMonster::SetUp_ConstantTable()
//{
//	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
//
//	if (FAILED(m_pTransformCom->Bind_WorldMatrixOnShader(m_pShaderCom, "g_WorldMatrix")))
//	{
//		MSG_BOX(L"Failed To CMonster : SetUp_ConstantTable : m_pTransform->Bind_WorldMatrixOnShader");
//		return E_FAIL;
//	}
//	
//	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
//	{
//		MSG_BOX(L"Failed To CModel_Object : SetUp_ConstantTable : m_pShader->Set_RawValue(g_ViewMatrix)");
//		return E_FAIL;
//	}
//
//	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
//	{
//		MSG_BOX(L"Failed To CModel_Object : SetUp_ConstantTable : m_pShader->Set_RawValue(g_ProjMatrix)");
//		return E_FAIL;
//	}
//	
//	RELEASE_INSTANCE(CGameInstance);
//
//	return S_OK;
//}

void CMonster::Free()
{
	__super::Free();

	Safe_Release(m_pSphereCom);
	Safe_Release(m_pAABBCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);

}
