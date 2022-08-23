#include "stdafx.h"
#include "..\Public\MeshEffect.h"
#include "GameInstance.h"

CMeshEffect::CMeshEffect(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CBlendObject(pDeviceOut, pDeviceContextOut)
{

}

CMeshEffect::CMeshEffect(const CMeshEffect & rhs)
	: CBlendObject(rhs)
{
}

HRESULT CMeshEffect::NativeConstruct_Prototype()
{

	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMeshEffect::NativeConstruct(void * pArg)
{
	if (pArg != nullptr)
	{
		m_EffectDesc_Mesh = *(EFFECTDESC_MESH*)pArg;
	}

	XMStoreFloat4x4(&m_LocalMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ParentsMatrix, XMMatrixIdentity());
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;	
	
	return S_OK;
}

void CMeshEffect::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	m_Time += (_float)TimeDelta*m_MultipleTime;

	_float Ratio = 0.f;


	
	
	if (m_EffectDesc_Mesh.fMaxTime < m_Time)
	{
		//m_bFinish = true;
		m_Time = m_EffectDesc_Mesh.fMaxTime;
		m_bDead = true;
		return;
	}

	_vector vShader, vScale, vRotation, vPosition;
	_vector vSourShader, vSourScale, vSourRotation, vSourPosition;
	_vector vDestShader, vDestScale, vDestRotation, vDestPosition;

	
	
		if (m_EffectDesc_Mesh.KeyFram_1_TimeEnd >= m_Time)
		{
			Ratio = m_Time / m_EffectDesc_Mesh.KeyFram_1_TimeEnd;
			Alpha = m_EffectDesc_Mesh.KeyFram_0_Alpha + (m_EffectDesc_Mesh.KeyFram_1_Alpha - m_EffectDesc_Mesh.KeyFram_0_Alpha) * Ratio;

			vSourShader = XMLoadFloat3(&m_EffectDesc_Mesh.KeyFram_0_Shader);
			vSourScale = XMLoadFloat3(&m_EffectDesc_Mesh.KeyFram_0_Scale);
			vSourRotation = XMLoadFloat3(&m_EffectDesc_Mesh.KeyFram_0_Rotation);
			vSourPosition = XMLoadFloat3(&m_EffectDesc_Mesh.KeyFram_0_Position);

			vDestShader = XMLoadFloat3(&m_EffectDesc_Mesh.KeyFram_1_Shader);
			vDestScale = XMLoadFloat3(&m_EffectDesc_Mesh.KeyFram_1_Scale);
			vDestRotation = XMLoadFloat3(&m_EffectDesc_Mesh.KeyFram_1_Rotation);
			vDestPosition = XMLoadFloat3(&m_EffectDesc_Mesh.KeyFram_1_Position);

			vShader = XMVectorLerp(vSourShader, vDestShader, Ratio);
			vScale = XMVectorLerp(vSourScale, vDestScale, Ratio);
			vRotation = XMVectorLerp(vSourRotation, vDestRotation, Ratio);
			//vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, Ratio);
			vPosition = XMVectorLerp(vSourPosition, vDestPosition, Ratio);
			vPosition = XMVectorSetW(vPosition, 1.f);
		}
		else if (m_EffectDesc_Mesh.KeyFram_2_TimeEnd >= m_Time)
		{
			Ratio = (m_Time - m_EffectDesc_Mesh.KeyFram_1_TimeEnd) / (m_EffectDesc_Mesh.KeyFram_2_TimeEnd - m_EffectDesc_Mesh.KeyFram_1_TimeEnd);
			Alpha = m_EffectDesc_Mesh.KeyFram_1_Alpha + (m_EffectDesc_Mesh.KeyFram_2_Alpha - m_EffectDesc_Mesh.KeyFram_1_Alpha) * Ratio;

			vSourShader = XMLoadFloat3(&m_EffectDesc_Mesh.KeyFram_1_Shader);
			vSourScale = XMLoadFloat3(&m_EffectDesc_Mesh.KeyFram_1_Scale);
			vSourRotation = XMLoadFloat3(&m_EffectDesc_Mesh.KeyFram_1_Rotation);
			vSourPosition = XMLoadFloat3(&m_EffectDesc_Mesh.KeyFram_1_Position);

			vDestShader = XMLoadFloat3(&m_EffectDesc_Mesh.KeyFram_2_Shader);
			vDestScale = XMLoadFloat3(&m_EffectDesc_Mesh.KeyFram_2_Scale);
			vDestRotation = XMLoadFloat3(&m_EffectDesc_Mesh.KeyFram_2_Rotation);
			vDestPosition = XMLoadFloat3(&m_EffectDesc_Mesh.KeyFram_2_Position);

			vShader = XMVectorLerp(vSourShader, vDestShader, Ratio);
			vScale = XMVectorLerp(vSourScale, vDestScale, Ratio);
			vRotation = XMVectorLerp(vSourRotation, vDestRotation, Ratio);
			//vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, Ratio);
			vPosition = XMVectorLerp(vSourPosition, vDestPosition, Ratio);
			vPosition = XMVectorSetW(vPosition, 1.f);
		}
		else if (m_EffectDesc_Mesh.KeyFram_3_TimeEnd >= m_Time)
		{
			Ratio = (m_Time - m_EffectDesc_Mesh.KeyFram_2_TimeEnd) / (m_EffectDesc_Mesh.KeyFram_3_TimeEnd - m_EffectDesc_Mesh.KeyFram_2_TimeEnd);
			Alpha = m_EffectDesc_Mesh.KeyFram_2_Alpha + (m_EffectDesc_Mesh.KeyFram_3_Alpha - m_EffectDesc_Mesh.KeyFram_2_Alpha) * Ratio;

			vSourShader = XMLoadFloat3(&m_EffectDesc_Mesh.KeyFram_2_Shader);
			vSourScale = XMLoadFloat3(&m_EffectDesc_Mesh.KeyFram_2_Scale);
			vSourRotation = XMLoadFloat3(&m_EffectDesc_Mesh.KeyFram_2_Rotation);
			vSourPosition = XMLoadFloat3(&m_EffectDesc_Mesh.KeyFram_2_Position);

			vDestShader = XMLoadFloat3(&m_EffectDesc_Mesh.KeyFram_3_Shader);
			vDestScale = XMLoadFloat3(&m_EffectDesc_Mesh.KeyFram_3_Scale);
			vDestRotation = XMLoadFloat3(&m_EffectDesc_Mesh.KeyFram_3_Rotation);
			vDestPosition = XMLoadFloat3(&m_EffectDesc_Mesh.KeyFram_3_Position);

			vShader = XMVectorLerp(vSourShader, vDestShader, Ratio);
			vScale = XMVectorLerp(vSourScale, vDestScale, Ratio);
			vRotation = XMVectorLerp(vSourRotation, vDestRotation, Ratio);
			//vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, Ratio);
			vPosition = XMVectorLerp(vSourPosition, vDestPosition, Ratio);
			vPosition = XMVectorSetW(vPosition, 1.f);
		}
		else if (m_EffectDesc_Mesh.KeyFram_4_TimeEnd >= m_Time)
		{
			Ratio = (m_Time - m_EffectDesc_Mesh.KeyFram_3_TimeEnd) / (m_EffectDesc_Mesh.KeyFram_4_TimeEnd - m_EffectDesc_Mesh.KeyFram_3_TimeEnd);
			Alpha = m_EffectDesc_Mesh.KeyFram_3_Alpha + (m_EffectDesc_Mesh.KeyFram_4_Alpha - m_EffectDesc_Mesh.KeyFram_3_Alpha) * Ratio;

			vSourShader = XMLoadFloat3(&m_EffectDesc_Mesh.KeyFram_3_Shader);
			vSourScale = XMLoadFloat3(&m_EffectDesc_Mesh.KeyFram_3_Scale);
			vSourRotation = XMLoadFloat3(&m_EffectDesc_Mesh.KeyFram_3_Rotation);
			vSourPosition = XMLoadFloat3(&m_EffectDesc_Mesh.KeyFram_3_Position);

			vDestShader = XMLoadFloat3(&m_EffectDesc_Mesh.KeyFram_4_Shader);
			vDestScale = XMLoadFloat3(&m_EffectDesc_Mesh.KeyFram_4_Scale);
			vDestRotation = XMLoadFloat3(&m_EffectDesc_Mesh.KeyFram_4_Rotation);
			vDestPosition = XMLoadFloat3(&m_EffectDesc_Mesh.KeyFram_4_Position);

			vShader = XMVectorLerp(vSourShader, vDestShader, Ratio);
			vScale = XMVectorLerp(vSourScale, vDestScale, Ratio);
			vRotation = XMVectorLerp(vSourRotation, vDestRotation, Ratio);
			//vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, Ratio);
			vRotation = XMVectorLerp(vSourRotation, vDestRotation, Ratio);
			vPosition = XMVectorLerp(vSourPosition, vDestPosition, Ratio);
			vPosition = XMVectorSetW(vPosition, 1.f);
		}

	
	
		
	XMStoreFloat3(&m_Shader, vShader);
	_matrix PivotMatrix;// = XMMatrixIdentity();
	PivotMatrix = XMMatrixScalingFromVector(vScale);
	PivotMatrix = PivotMatrix * XMMatrixRotationX(XMConvertToRadians(XMVectorGetX(vRotation)));
	PivotMatrix = PivotMatrix * XMMatrixRotationY(XMConvertToRadians(XMVectorGetY(vRotation)));
	PivotMatrix = PivotMatrix * XMMatrixRotationAxis(PivotMatrix.r[2], XMConvertToRadians(XMVectorGetZ(vRotation)));

	//PivotMatrix = PivotMatrix * XMMatrixRotationAxis(PivotMatrix.r[2], XMConvertToRadians(XMVectorGetZ(vRotation)));
	//PivotMatrix = PivotMatrix * XMMatrixRotationAxis(PivotMatrix.r[0], XMConvertToRadians(XMVectorGetX(vRotation)));
	//PivotMatrix = PivotMatrix * XMMatrixRotationAxis(PivotMatrix.r[1], XMConvertToRadians(XMVectorGetY(vRotation)));
	
	
	PivotMatrix.r[3] = XMVectorAdd(PivotMatrix.r[3], vPosition);
	
	//PivotMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);
	


	XMStoreFloat4x4(&m_LocalMatrix, PivotMatrix);
	
}

void CMeshEffect::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
	
	if (nullptr != m_pRendererCom && m_PassTime <= 0)
	{
		Compute_CamDistance();
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONLIGHT, this);
	}
		
}

HRESULT CMeshEffect::Render()
{
	if (nullptr == m_pShaderCom || nullptr == m_pModel)
	{
		MSG_BOX(L"Failed To CModel_Object : Render : nullptr == m_pShader || nullptr == m_pModel");
		return E_FAIL;
	}
	if (FAILED(SetUp_ConstantTable()))
	{
		MSG_BOX(L"Failed To CModel_Object : Render : SetUp_ConstantTable");
		return E_FAIL;
	}

	m_iNumMeshContainers = m_pModel->Get_NumMeshContainer();

	if (FAILED(m_pTextureCom->SetUp_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", m_EffectDesc_Mesh.iTexture)))
		return E_FAIL;
	if (m_EffectDesc_Mesh.bBillboard)
	{
		if (FAILED(m_pTextureCom->SetUp_ShaderResourceView(m_pShaderCom, "g_DissolveTexture", m_EffectDesc_Mesh.iTexture1)))
			return E_FAIL;
		//if (FAILED(m_pTextureCom->SetUp_ShaderResourceView(m_pShaderCom, "g_NormalTexture", m_EffectDesc_Mesh.iTexture2)))
		//	return E_FAIL;
	}
	
	
	if (FAILED(m_pModel->Render(m_pShaderCom, "g_BoneMatrices", m_EffectDesc_Mesh.iMesh, m_EffectDesc_Mesh.iShader)))
	{
		MSG_BOX(L"Failed To CModel_Object : Render : m_pModel->Render");
		return E_FAIL;
	}
	
	return S_OK;
}



HRESULT CMeshEffect::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxEffect"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Mesh"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Texture */
	//if (FAILED(__super::SetUp_Component(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_MeshEffect"), (CComponent**)&m_pTextureCom)))
	//	return E_FAIL;
	
	/* For.Com_Model */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_Effect1"), (CComponent**)&m_pModel)))
		return E_FAIL;

	//if (FAILED(m_pTextureCom->SetUp_ShaderResourceView(m_pShaderCom, "g_DissolveTexture", 64)))
	//	return E_FAIL;
	
	return S_OK;
}

HRESULT CMeshEffect::SetUp_ConstantTable()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_float4x4 WorldMatrix;
	XMStoreFloat4x4(&WorldMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_LocalMatrix) * XMLoadFloat4x4(&m_ParentsMatrix)));
	//XMMatrixTranspose
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &WorldMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Set_RawValue("g_Color1", &m_EffectDesc_Mesh.vColor1, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_Color2", &m_EffectDesc_Mesh.vColor2, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_Shader", &m_Shader, sizeof(_float3))))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Set_RawValue("g_UVMax", &m_EffectDesc_Mesh.fMaxUv, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_SubUV", &m_EffectDesc_Mesh.fmultipleTime, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_Dissolve", &Alpha, sizeof(_float))))
		return E_FAIL;

	



	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CMeshEffect::Reset()
{
	m_EffectDesc_Mesh.bBillboard = false;
	
	float			fmultipleTime = 1.f;

	float			KeyFram_0_Alpha = 0.f;
	float			KeyFram_1_Alpha = 0.f;
	float			KeyFram_2_Alpha = 0.f;
	float			KeyFram_3_Alpha = 0.f;
	float			KeyFram_4_Alpha = 0.f;

	XMFLOAT3		KeyFram_0_Scale = XMFLOAT3(1.f, 1.f, 1.f);
	XMFLOAT3		KeyFram_1_Scale = XMFLOAT3(1.f, 1.f, 1.f);
	XMFLOAT3		KeyFram_2_Scale = XMFLOAT3(1.f, 1.f, 1.f);
	XMFLOAT3		KeyFram_3_Scale = XMFLOAT3(1.f, 1.f, 1.f);
	XMFLOAT3		KeyFram_4_Scale = XMFLOAT3(1.f, 1.f, 1.f);

	XMFLOAT3		KeyFram_0_Shader = XMFLOAT3(0.f, 0.f, 1.f);
	XMFLOAT3		KeyFram_1_Shader = XMFLOAT3(0.f, 0.f, 1.f);
	XMFLOAT3		KeyFram_2_Shader = XMFLOAT3(0.f, 0.f, 1.f);
	XMFLOAT3		KeyFram_3_Shader = XMFLOAT3(0.f, 0.f, 1.f);
	XMFLOAT3		KeyFram_4_Shader = XMFLOAT3(0.f, 0.f, 1.f);

	XMFLOAT3		KeyFram_0_Rotation = XMFLOAT3(0.f, 0.f, 0.f);
	XMFLOAT3		KeyFram_1_Rotation = XMFLOAT3(0.f, 0.f, 0.f);
	XMFLOAT3		KeyFram_2_Rotation = XMFLOAT3(0.f, 0.f, 0.f);
	XMFLOAT3		KeyFram_3_Rotation = XMFLOAT3(0.f, 0.f, 0.f);
	XMFLOAT3		KeyFram_4_Rotation = XMFLOAT3(0.f, 0.f, 0.f);

	XMFLOAT3		KeyFram_0_Position = XMFLOAT3(0.f, 0.f, 0.f);
	XMFLOAT3		KeyFram_1_Position = XMFLOAT3(0.f, 0.f, 0.f);
	XMFLOAT3		KeyFram_2_Position = XMFLOAT3(0.f, 0.f, 0.f);
	XMFLOAT3		KeyFram_3_Position = XMFLOAT3(0.f, 0.f, 0.f);
	XMFLOAT3		KeyFram_4_Position = XMFLOAT3(0.f, 0.f, 0.f);

	XMFLOAT4		vColor1 = XMFLOAT4(0.f, 0.f, 0.f, 1.f);
	XMFLOAT4		vColor2 = XMFLOAT4(0.f, 0.f, 0.f, 1.f);

}

void CMeshEffect::KeyFram_Reset()
{
	m_EffectDesc_Mesh.KeyFram_0_TimeEnd = 0.f;
	m_EffectDesc_Mesh.KeyFram_1_TimeEnd = (m_EffectDesc_Mesh.fMaxTime * 1) / 4; 
	m_EffectDesc_Mesh.KeyFram_2_TimeEnd = (m_EffectDesc_Mesh.fMaxTime * 2) / 4; 
	m_EffectDesc_Mesh.KeyFram_3_TimeEnd = (m_EffectDesc_Mesh.fMaxTime * 3) / 4;
	m_EffectDesc_Mesh.KeyFram_4_TimeEnd = (m_EffectDesc_Mesh.fMaxTime * 4) / 4;
}

CMeshEffect * CMeshEffect::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CMeshEffect*	pInstance = new CMeshEffect(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CMeshEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CBlendObject * CMeshEffect::Clone(void * pArg)
{
	CMeshEffect*	pInstance = new CMeshEffect(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CMeshEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMeshEffect::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pModel);

}
