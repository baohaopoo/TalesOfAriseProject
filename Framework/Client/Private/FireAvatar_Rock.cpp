#include "stdafx.h"
#include "..\Public\FireAvatar_Rock.h"
#include "GameInstance.h"
#include "Fire_Avatar.h"
CFireAvatar_Rock::CFireAvatar_Rock(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CEquipment(pDeviceOut, pDeviceContextOut)
{

}

CFireAvatar_Rock::CFireAvatar_Rock(const CFireAvatar_Rock & rhs)
	: CEquipment(rhs)
{
}

HRESULT CFireAvatar_Rock::NativeConstruct_Prototype()
{

	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CFireAvatar_Rock::NativeConstruct(void * pArg)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (nullptr != pArg)
		memcpy(&m_SocketDesc, pArg, sizeof(SOCKETDESC));

	CTransform*		pAvatarTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_TUTORIAL, TEXT("Layer_FireAvatar"), TEXT("Com_Transform"));
	if (nullptr == pAvatarTransform)
		return E_FAIL;

	_vector vRight = pAvatarTransform->Get_State(CTransform::STATE_RIGHT) * (-1.025f);
	//_vector vLook = pAvatarTransform->Get_State(CTransform::STATE_LOOK) * (-1.f);

	_float4x4 PivotRightMatrix, PivotLookMatrix;
	XMStoreFloat4x4(&PivotRightMatrix, XMMatrixIdentity());
	//XMStoreFloat4x4(&PivotLookMatrix, XMMatrixIdentity());


	PivotRightMatrix._41 = XMVectorGetX(vRight);
	PivotRightMatrix._42 = XMVectorGetY(vRight);
	PivotRightMatrix._43 = XMVectorGetZ(vRight);
	PivotRightMatrix._44 = 1.f;

	m_RockRightMatrix = PivotRightMatrix;

	//PivotLookMatrix._41 = XMVectorGetX(vLook);
	//PivotLookMatrix._42 = XMVectorGetY(vLook);
	//PivotLookMatrix._43 = XMVectorGetZ(vLook);
	//PivotLookMatrix._44 = 1.f;

	//m_RockLookMatrix = PivotLookMatrix;


	m_pSocketMatrix = m_SocketDesc.pModelCom->Get_CombinedTransformationMatrix(m_SocketDesc.pBoneName);

	m_PivotMatrix = m_SocketDesc.pModelCom->Get_PivotMatrix4x4();

	if (nullptr == m_pSocketMatrix)
		return E_FAIL;

	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CFireAvatar_Rock::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	class CModel* pModelCom = (CModel*)pGameInstance->Get_Component(LEVEL_TUTORIAL, TEXT("Layer_FireAvatar"), TEXT("Com_AvatarSKL"));
	CTransform*		pRockTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_TUTORIAL, TEXT("Layer_FireAvatar"), TEXT("Com_Transform"));
	if (nullptr == pRockTransform)
		return;


	CGameObject* pFire_Avatar = pGameInstance->Get_GameObject(LEVEL_TUTORIAL, TEXT("Layer_FireAvatar"), 0);
	m_bDead = dynamic_cast<CFire_Avatar*>(pFire_Avatar)->Get_CrashRock();

	if (m_bDead)
	{
		_matrix StoreMatrix = XMLoadFloat4x4(pModelCom->Get_CombinedTransformationMatrix("KK_R"))* pModelCom->Get_PivotMatrix() * pRockTransform->Get_WorldMatrix();
		_float4x4 mat;
		XMStoreFloat4x4(&mat, StoreMatrix);

		vPos = XMLoadFloat4((_float4*)&mat.m[3][0]);
	}


	RELEASE_INSTANCE(CGameInstance);
}

void CFireAvatar_Rock::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT CFireAvatar_Rock::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;


	_uint		iNumMeshContainers = m_pModelCom->Get_NumMeshContainer();

	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		if (FAILED(m_pModelCom->Bind_Material_OnShader(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", i)))
			return E_FAIL;
		if (FAILED(m_pModelCom->Bind_Material_OnShader(m_pShaderCom, aiTextureType_NORMALS, "g_NormalTexture", i)))
			return E_FAIL;
		if (FAILED(m_pModelCom->Render(m_pShaderCom, "g_BoneMatrices", i, 1)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CFireAvatar_Rock::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNonAnim"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Model"), LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_EFRRock"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CFireAvatar_Rock::SetUp_ConstantTable()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	/* 뼈 기준의 스워드 메시의 상태변환. */
	if (FAILED(m_pTransformCom->Bind_WorldMatrixOnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	/* 뼈의 스케일상태는 무시. 회전값과 이동값만 무기의 상태로 사용한다. */
	_matrix		BoneMatrix = XMLoadFloat4x4(m_pSocketMatrix);

	BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
	BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
	BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);

	CTransform*		pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_TUTORIAL, TEXT("Layer_FireAvatar"), CGameObject::m_pTransformTag, 0);
	if (nullptr == pPlayerTransform)
		return E_FAIL;

	_float4x4	SocketMatrix;
	XMStoreFloat4x4(&SocketMatrix, XMMatrixTranspose(BoneMatrix * XMLoadFloat4x4(&m_PivotMatrix) * XMLoadFloat4x4(&m_RockRightMatrix) /** XMLoadFloat4x4(&m_RockLookMatrix)*/ * pPlayerTransform->Get_WorldMatrix()));

	//XMStoreFloat4x4(&RealPosMatrix, BoneMatrix*XMLoadFloat4x4(&m_PivotMatrix)*XMLoadFloat4x4(&m_RockRightMatrix) * pPlayerTransform->Get_WorldMatrix());

	m_pShaderCom->Set_RawValue("g_SocketMatrix", &SocketMatrix, sizeof(_float4x4));

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CFireAvatar_Rock * CFireAvatar_Rock::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CFireAvatar_Rock*	pInstance = new CFireAvatar_Rock(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CFireAvatar_Rock"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CFireAvatar_Rock::Clone(void * pArg)
{
	CFireAvatar_Rock*	pInstance = new CFireAvatar_Rock(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CFireAvatar_Rock"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFireAvatar_Rock::Free()
{
	__super::Free();

}
