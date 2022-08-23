#include "stdafx.h"
#include "..\Public\Gravitas_Field.h"
#include "GameInstance.h"
#include "Animation.h"

CGravitas_Field::CGravitas_Field(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CBullet(pDeviceOut, pDeviceContextOut)
{

}

CGravitas_Field::CGravitas_Field(const CGravitas_Field & rhs)
	: CBullet(rhs)
{
}

HRESULT CGravitas_Field::NativeConstruct_Prototype()
{

	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGravitas_Field::NativeConstruct(void * pArg)
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
		m_pSocketMatrix = m_BulletDesc.pModelCom->Get_CombinedTransformationMatrix(m_BulletDesc.pBoneName);

		m_PivotMatrix = m_BulletDesc.pModelCom->Get_PivotMatrix4x4();
	}

	if (nullptr == m_pSocketMatrix)
		return E_FAIL;
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	/* 뼈의 스케일상태는 무시. 회전값과 이동값만 무기의 상태로 사용한다. */
	_matrix		BoneMatrix = XMLoadFloat4x4(m_pSocketMatrix);

	BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
	BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
	BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);

	CTransform*		pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), CGameObject::m_pTransformTag, 1);
	if (nullptr == pPlayerTransform)
		return E_FAIL;

	_float4x4	SocketMatrix;
	XMStoreFloat4x4(&SocketMatrix, (BoneMatrix * XMLoadFloat4x4(&m_PivotMatrix)*XMMatrixRotationY(XMConvertToRadians(180.f)) * pPlayerTransform->Get_WorldMatrix()));



	m_pTransformCom->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix()*XMLoadFloat4x4(&SocketMatrix));


	//m_pTransformCom->TurnAxis(XMVectorSetY(m_BulletDesc.vTargetPos, 0.f) - XMVectorSetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION),0.f));


	m_pTransformCom->Scaled(_float3(1.f, 1.f, 1.f));
	m_pAttackSphereCom->Set_On(true);
	RELEASE_INSTANCE(CGameInstance);


	m_DeadTime = 2.0;
	m_LiveTime = 0.0;
	return S_OK;
}

void CGravitas_Field::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_BackTime += TimeDelta;

	m_pTransformCom->Go_Straight(TimeDelta*2.f);






	RELEASE_INSTANCE(CGameInstance);

	m_pAttackSphereCom->Update(m_pTransformCom->Get_WorldMatrix());
}

void CGravitas_Field::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);



#ifdef DEBUG

	m_pRendererCom->Add_DebugComponent(m_pAttackSphereCom);
#endif // DEBUG

}

HRESULT CGravitas_Field::Render()
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

HRESULT CGravitas_Field::SetUp_Components()
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


	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.5f);
	ColliderDesc.vSize = _float3(0.15f, 0.3f, 1.2f);
	ColliderDesc.fRadius = 0.1f;
	if (FAILED(__super::SetUp_Components(TEXT("Com_AttackSPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pAttackSphereCom, &ColliderDesc)))
		return E_FAIL;

	//m_pAttackSphereCom->Set_HitInfo(1, false);

	return S_OK;
}

HRESULT CGravitas_Field::SetUp_ConstantTable()
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

CGravitas_Field * CGravitas_Field::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CGravitas_Field*	pInstance = new CGravitas_Field(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CGravitas_Field"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CGravitas_Field::Clone(void * pArg)
{
	CGravitas_Field*	pInstance = new CGravitas_Field(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CGravitas_Field"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGravitas_Field::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);

	Safe_Release(m_pRendererCom);

}
