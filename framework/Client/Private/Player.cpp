#include "stdafx.h"
#include "..\Public\Player.h"
#include "GameInstance.h"

CPlayer::CPlayer(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{

}

CPlayer::CPlayer(const CPlayer & rhs)
	: CGameObject(rhs)
{
}

HRESULT CPlayer::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::NativeConstruct(void * pArg, CTransform::TRANSFORMDESC* pTransformDesc)
{
	if (FAILED(__super::NativeConstruct(pArg, pTransformDesc)))
	{
		MSG_BOX(L"Failed To CPlayer : NativeConstruct : NativeConstruct");
		return E_FAIL;
	}

	return S_OK;
}

void CPlayer::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	if (m_iCurrentAnimationIndex == m_iNextAnimationIndex)
	{
		m_pModelCom->Update(TimeDelta*m_fAttackSpeed, m_bLoop);
		//m_pModelFaceCom->Update(TimeDelta*m_fAttackSpeed, m_bLoop);
		//m_pModelHairCom->Update(TimeDelta*m_fAttackSpeed, m_bLoop);
		_matrix WorldMatrix = m_pTransformCom->Get_WorldMatrix();
		_matrix LocalMatrix = m_pModelCom->Get_CurAnimation()->Get_MoveMatrix() * m_pModelCom->Get_PivotMatrix();
		_vector vWorldMove = XMVector3TransformCoord(LocalMatrix.r[3], WorldMatrix);
		WorldMatrix.r[3] = vWorldMove;
		m_pTransformCom->Set_WorldMatrix(WorldMatrix);
	}

	else
	{
		m_pModelCom->Update_NextAnim(TimeDelta*m_fAttackSpeed, m_iLinearTime, m_iNextAnimationIndex, &m_iCurrentAnimationIndex);
		//m_pModelFaceCom->Update_NextAnim(TimeDelta*m_fAttackSpeed, m_iLinearTime, m_iNextAnimationIndex, &m_iCurrentAnimationIndex);
		//m_pModelHairCom->Update_NextAnim(TimeDelta*m_fAttackSpeed, m_iLinearTime, m_iNextAnimationIndex, &m_iCurrentAnimationIndex);
	}

	if (GetKeyState(VK_UP) & 0x8000)
	{
		m_pTransformCom->Go_Straight(TimeDelta);
	}

	if (GetKeyState(VK_DOWN) & 0x8000)
	{
		m_pTransformCom->Go_BackWard(TimeDelta);
	}

	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), TimeDelta * -1.f);
	}

	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), TimeDelta);
	}

}

void CPlayer::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);	

	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
	}

}

HRESULT CPlayer::Render()
{
	return S_OK;
}


//HRESULT CPlayer::SetUp_ConstantTable()
//{
//	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
//
//	if (FAILED(m_pTransformCom->Bind_WorldMatrixOnShader(m_pShaderCom, "g_WorldMatrix")))
//	{
//		MSG_BOX(L"Failed To CPlayer : SetUp_ConstantTable : m_pTransform->Bind_WorldMatrixOnShader");
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

void CPlayer::Free()
{
	__super::Free();	
	
	Safe_Release(m_pSphereCom);
	Safe_Release(m_pAABBCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);

}
