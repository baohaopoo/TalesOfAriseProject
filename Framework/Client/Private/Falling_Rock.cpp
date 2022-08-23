#include "stdafx.h"
#include "..\Public\Falling_Rock.h"
#include "GameInstance.h"

CFalling_Rock::CFalling_Rock(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{

}

CFalling_Rock::CFalling_Rock(const CFalling_Rock & rhs)
	: CGameObject(rhs)
{
}

HRESULT CFalling_Rock::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CFalling_Rock::NativeConstruct(void* pArg)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_iCreateRand = rand() % 3 + 1;

	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.f);

	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (pArg != nullptr)
	{
		_vector pPos = *(_vector*)pArg;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, pPos);
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CFalling_Rock::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	//m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), TimeDelta *(-1.f));

	_float fPos = XMVectorGetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	if (fPos < 0.f)
	{
		m_bDead = true;
	}


	m_pSphereCom->Update(m_pTransformCom->Get_WorldMatrix());
}

void CFalling_Rock::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
}

HRESULT CFalling_Rock::Render()
{
	if (nullptr == m_pShaderCom/* || nullptr == m_pModelCom1 || nullptr == m_pModelCom2 || nullptr == m_pModelCom3*/)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	if (m_iCreateRand == 1)
	{
		_uint		iNumMeshContainers = m_pModelCom1->Get_NumMeshContainer();

		for (_uint i = 0; i < iNumMeshContainers; ++i)
		{
			if (FAILED(m_pModelCom1->Bind_Material_OnShader(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", i)))
				return E_FAIL;

			if (FAILED(m_pModelCom1->Bind_Material_OnShader(m_pShaderCom, aiTextureType_NORMALS, "g_NormalTexture", i)))
				return E_FAIL;

			if (FAILED(m_pModelCom1->Render(m_pShaderCom, "g_BoneMatrices", i, 0)))
				return E_FAIL;
		}
	}

	else if (m_iCreateRand == 2)
	{
		_uint		iNumMeshContainers = m_pModelCom2->Get_NumMeshContainer();

		for (_uint i = 0; i < iNumMeshContainers; ++i)
		{
			if (FAILED(m_pModelCom2->Bind_Material_OnShader(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", i)))
				return E_FAIL;

			if (FAILED(m_pModelCom2->Bind_Material_OnShader(m_pShaderCom, aiTextureType_NORMALS, "g_NormalTexture", i)))
				return E_FAIL;

			if (FAILED(m_pModelCom2->Render(m_pShaderCom, "g_BoneMatrices", i, 0)))
				return E_FAIL;
		}
	}

	else if (m_iCreateRand == 3)
	{
		_uint		iNumMeshContainers = m_pModelCom3->Get_NumMeshContainer();

		for (_uint i = 0; i < iNumMeshContainers; ++i)
		{
			if (FAILED(m_pModelCom3->Bind_Material_OnShader(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", i)))
				return E_FAIL;

			if (FAILED(m_pModelCom3->Bind_Material_OnShader(m_pShaderCom, aiTextureType_NORMALS, "g_NormalTexture", i)))
				return E_FAIL;

			if (FAILED(m_pModelCom3->Render(m_pShaderCom, "g_BoneMatrices", i, 0)))
				return E_FAIL;
		}
	}



#ifdef _DEBUG
	m_pSphereCom->Render();
#endif // _DEBUG


	return S_OK;
}

HRESULT CFalling_Rock::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNonAnim"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (m_iCreateRand == 1)
	{
		if (FAILED(__super::SetUp_Components(TEXT("Com_Model1"), LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_BurningRock1"), (CComponent**)&m_pModelCom1)))
			return E_FAIL;
	}

	else if (m_iCreateRand == 2)
	{
		if (FAILED(__super::SetUp_Components(TEXT("Com_Model2"), LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_BurningRock2"), (CComponent**)&m_pModelCom2)))
			return E_FAIL;
	}

	else if (m_iCreateRand == 3)
	{
		if (FAILED(__super::SetUp_Components(TEXT("Com_Model3"), LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_BurningRock3"), (CComponent**)&m_pModelCom3)))
			return E_FAIL;
	}

	/* For.Com_Collider */
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	ColliderDesc.fRadius = 1.f;

	if (FAILED(__super::SetUp_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CFalling_Rock::SetUp_ConstantTable()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);




	if (FAILED(m_pTransformCom->Bind_WorldMatrixOnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	//if (FAILED(m_pShaderCom->Set_RawValue("g_vCamPosition", &pGameInstance->Get_CamPositionFloat4(), sizeof(_float4))))
	//	return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CFalling_Rock * CFalling_Rock::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CFalling_Rock*	pInstance = new CFalling_Rock(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CFalling_Rock"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CFalling_Rock::Clone(void * pArg)
{
	CFalling_Rock*	pInstance = new CFalling_Rock(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone CFalling_Rock"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFalling_Rock::Free()
{
	__super::Free();

	Safe_Release(m_pSphereCom);
	Safe_Release(m_pShaderCom);
	if (m_iCreateRand == 1)
		Safe_Release(m_pModelCom1);

	else if (m_iCreateRand == 2)
		Safe_Release(m_pModelCom2);

	else if (m_iCreateRand == 3)
		Safe_Release(m_pModelCom3);

	Safe_Release(m_pRendererCom);
}