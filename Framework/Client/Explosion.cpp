#include "stdafx.h"
#include "..\Public\Explosion.h"
#include "GameInstance.h"
#include "Animation.h"

CExplosion::CExplosion(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CBullet(pDeviceOut, pDeviceContextOut)
{

}

CExplosion::CExplosion(const CExplosion & rhs)
	: CBullet(rhs)
{
}

HRESULT CExplosion::NativeConstruct_Prototype()
{

	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CExplosion::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);


	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (pArg != nullptr)
	{
		memcpy(&m_BulletDesc, pArg, sizeof(BULLETDESC));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,XMVectorSetY(m_BulletDesc.vBulletPos,XMVectorGetY(m_BulletDesc.vBulletPos)+0.15f));
		m_pTransformCom->TurnAxis(-m_BulletDesc.vLook);
	}


	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	m_pTransformCom->Scaled(_float3(1.f, 1.f, 1.f));
	m_pAttackSphereCom->Set_On(true);
	RELEASE_INSTANCE(CGameInstance);


	m_DeadTime = 2.0;
	m_LiveTime = 0.0;
	return S_OK;
}

void CExplosion::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	
	_float vPosY = XMVectorGetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	if (vPosY > 0)
	{
		m_pTransformCom->Go_Straight(TimeDelta*1.f);
	}

	else {
		vPosY = 0;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION),0.f));
		m_pAttackSphereCom->Set_On(true);
		m_bRender = true;
	}



	



	



	RELEASE_INSTANCE(CGameInstance);

	m_pAttackSphereCom->Update(m_pTransformCom->Get_WorldMatrix());
}

void CExplosion::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);



#ifdef DEBUG

	m_pRendererCom->Add_DebugComponent(m_pAttackSphereCom);
#endif // DEBUG

}

HRESULT CExplosion::Render()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	//if (FAILED(SetUp_ConstantTable()))
	//	return E_FAIL;

	////16
	//for (_uint i = 0; i < iNumMeshContainers; ++i)
	//{
	//	if (FAILED(m_pModelCom->Bind_Material_OnShader(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", i)))
	//		return E_FAIL;


	//	if (FAILED(m_pModelCom->Render(m_pShaderCom, "g_BoneMatrices", i, 3)))
	//		return E_FAIL;
	//}


#ifdef _DEBUG

	//m_pOBBCom->Render();
	
	m_pAttackSphereCom->Render();
#endif // _DEBUG
	return S_OK;
}

HRESULT CExplosion::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNorTex"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	///* For.Com_Model */
	//if (FAILED(__super::SetUp_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_Spear"), (CComponent**)&m_pModelCom)))
	//	return E_FAIL;
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));


	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	ColliderDesc.vSize = _float3(0.15f, 0.3f, 1.2f);
	ColliderDesc.fRadius = 0.1f;
	if (FAILED(__super::SetUp_Components(TEXT("Com_AttackSPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pAttackSphereCom, &ColliderDesc)))
		return E_FAIL;

	//m_pAttackSphereCom->Set_HitInfo(1, false);

	return S_OK;
}

HRESULT CExplosion::SetUp_ConstantTable()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransformCom->Bind_WorldMatrixOnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;



	_float4x4	SocketMatrix;
	XMStoreFloat4x4(&SocketMatrix, XMMatrixIdentity());
	m_pShaderCom->Set_RawValue("g_SocketMatrix", &SocketMatrix, sizeof(_float4x4));





	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CExplosion * CExplosion::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CExplosion*	pInstance = new CExplosion(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CExplosion"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CExplosion::Clone(void * pArg)
{
	CExplosion*	pInstance = new CExplosion(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CExplosion"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CExplosion::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);

	Safe_Release(m_pRendererCom);

}
