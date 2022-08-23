#include "stdafx.h"
#include "..\Public\Fire_Avatar.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Channel.h"
#include "Sword.h"
#include "FireAvatar_Rock.h"
CFire_Avatar::CFire_Avatar(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CEnemy(pDeviceOut, pDeviceContextOut)
{

}

CFire_Avatar::CFire_Avatar(const CFire_Avatar & rhs)
	: CEnemy(rhs)
{
}

HRESULT CFire_Avatar::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CFire_Avatar::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 1.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_dAnimSpeed = 0.0;
	m_pModelCom->Set_AnimationIndex(ATTACK_APPERANCE_SUB);
	m_pModelSKL->Set_AnimationIndex(ATTACK_APPERANCE_SUB);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(20.f, -28.f, 0.f, 1.f));
	return S_OK;
}

void CFire_Avatar::Tick(_double TimeDelta)
{

	if (m_iCurrentAnimationIndex == m_iNextAnimationIndex)
	{
		m_pModelCom->Update(TimeDelta*m_dAnimSpeed, m_bCutAnimation);
		m_pModelSKL->Update(TimeDelta*m_dAnimSpeed, m_bCutAnimation);
		_matrix WorldMatrix = m_pTransformCom->Get_WorldMatrix();
		_vector MoveVector = XMVector3TransformCoord(XMLoadFloat3(&m_pModelCom->Get_CurAnimation()->Get_MoveVector()), m_pModelCom->Get_PivotMatrix());



		_matrix		BoneMatrix = WorldMatrix;

		BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
		BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
		BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);
		BoneMatrix.r[3] = XMVectorSet(0.f, 0.f, 0.f, 1.f);


		MoveVector = XMVector3TransformCoord(MoveVector, BoneMatrix);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetW(m_pTransformCom->Get_State(CTransform::STATE_POSITION) + MoveVector, 1.f));

	}

	else
	{
		m_pModelCom->Update_NextAnim(TimeDelta*m_dAnimSpeed, m_iDuration, m_iNextAnimationIndex, &m_iCurrentAnimationIndex);
		m_pModelSKL->Update_NextAnim(TimeDelta*m_dAnimSpeed, m_iDuration, m_iNextAnimationIndex, &m_iCurrentAnimationIndex);
	}

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (pGameInstance->Key_Down(DIK_3))
	{
		m_bGo = true;
	}

	if (m_bGo && !m_bAppearanceEnd)
	{
		Apperance();		// 첫번째 동작
	}


	if (!m_bHit)
	{
		m_TimeDelta += TimeDelta;


		if ((m_TimeDelta > 5.0) && (m_bOnce == false) && m_bAppearanceEnd)
		{
			m_bStart = true;

			m_iMotion = 1;
			//if (m_iMotion == 5)
			//	m_iMotion = 1;

			//else
			//	m_iMotion++;
		}

		switch (m_iMotion)
		{
		case 0:
			SweepGround(TimeDelta);
			break;

		case 1:
			RockFalling(TimeDelta);
			break;

		case 2:
			Punch_Ground(TimeDelta);
			break;

		case 3:
			Fire_Ball(TimeDelta);
			break;

		case 4:
			TwoHandPunch_Ground(TimeDelta);
			break;

		default:
			return;
		}
	}

	m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());
	m_pSphereCom->Update(m_pTransformCom->Get_WorldMatrix());

	__super::Tick(TimeDelta);
	RELEASE_INSTANCE(CGameInstance);
}

void CFire_Avatar::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CFire_Avatar::Render()
{
	if (nullptr == m_pShaderCom || nullptr == m_pModelCom || nullptr == m_pModelSKL)
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

		if (FAILED(m_pModelCom->Render(m_pShaderCom, "g_BoneMatrices", i, 0)))
			return E_FAIL;
	}

	return S_OK;
}


HRESULT CFire_Avatar::SetUp_Components()
{
	///* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnim"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Model"), LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_FireAvatar"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_ModelSKL */
	if (FAILED(__super::SetUp_Components(TEXT("Com_AvatarSKL"), LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_FireAvatarSKL"), (CComponent**)&m_pModelSKL)))
		return E_FAIL;

	/* For.Com_Collider */
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	/* For.Com_OBB */
	ColliderDesc.vPosition = _float3(0.f, 0.f, 20.f);
	ColliderDesc.vSize = _float3(0.7f, 1.4f, 0.7f);

	if (FAILED(__super::SetUp_Components(TEXT("Com_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_SPHERE */
	ColliderDesc.vPosition = _float3(0.f, 0.f, 20.f);
	ColliderDesc.fRadius = 1.f;

	if (FAILED(__super::SetUp_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CFire_Avatar::SetUp_ConstantTable()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransformCom->Bind_WorldMatrixOnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_vCamPosition", &pGameInstance->Get_CamPositionFloat4(), sizeof(_float4))))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CFire_Avatar::Apperance()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (!m_bOnce)
	{
		m_dAnimSpeed = 2.0;
		m_iDuration = 5.0;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_iNextAnimationIndex = ATTACK_APPERANCE_SUB;
	}

	else if ((m_iCurrentAnimationIndex == ATTACK_APPERANCE_SUB) && (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 2))
	{
		m_bAppearanceEnd = true;
		m_dAnimSpeed = 2.0;
		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
		m_TimeDelta = 0.0;
	}

	//if ((m_iCurrentAnimationIndex == ATTACK_APPERANCE_SUB) && (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 80
	//	&& (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 90)))
	//{
	//	m_bAppearanceEnd = true;
	//}
	RELEASE_INSTANCE(CGameInstance);
}

void CFire_Avatar::Fire_Ball(_double TimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (!m_bOnce && m_bStart)
	{
		m_dAnimSpeed = 2.0;
		m_iDuration = 5.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_iNextAnimationIndex = FIRE_BALL_PILLAR_START;
	}

	else if ((m_iCurrentAnimationIndex == FIRE_BALL_PILLAR_START) && (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 1))
	{
		m_iNextAnimationIndex = FIRE_BALL_PILLAR_LOOP;
	}

	else if ((m_iCurrentAnimationIndex == FIRE_BALL_PILLAR_LOOP) && (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 1))
	{
		m_iNextAnimationIndex = FIRE_BALL_PILLAR_END;
	}

	else if ((m_iCurrentAnimationIndex == FIRE_BALL_PILLAR_END) && (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 1))
	{
		m_iNextAnimationIndex = FIRE_BALL;
	}

	else if ((m_iCurrentAnimationIndex == FIRE_BALL) && (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 1))
	{
		m_dAnimSpeed = 2.0;
		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
		m_TimeDelta = 0.0;
	}

	RELEASE_INSTANCE(CGameInstance);

}

void CFire_Avatar::SweepGround(_double TimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (!m_bOnce && m_bStart)
	{
		m_dAnimSpeed = 2.0;
		m_iDuration = 5.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_iNextAnimationIndex = CLEAVE_RIGHT;
	}

	else if ((m_iCurrentAnimationIndex == CLEAVE_RIGHT) && (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 1))
	{
		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
		m_TimeDelta = 0.0;
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CFire_Avatar::RockFalling(_double TimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (!m_bOnce && m_bStart)
	{
		m_bCrashRock = false;
		m_bCreateRock = false;
		m_dAnimSpeed = 2.0;
		m_iDuration = 5.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_iNextAnimationIndex = ATTACK_ROCK_THROW;
	}

	else if ((m_iCurrentAnimationIndex == ATTACK_ROCK_THROW) && (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 1))
	{
		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
		m_TimeDelta = 0.0;
	}

	if ((m_iCurrentAnimationIndex == ATTACK_ROCK_THROW) && !m_bCreateRock)
	{
		if (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 220 && m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() <= 227)
		{
			CSword::SOCKETDESC			SocketDesc;
			ZeroMemory(&SocketDesc, sizeof(CSword::SOCKETDESC));

			SocketDesc.pModelCom = (CModel*)pGameInstance->Get_Component(LEVEL_TUTORIAL, TEXT("Layer_FireAvatar"), TEXT("Com_AvatarSKL"));
			SocketDesc.pBoneName = "KK_R";

			if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, TEXT("Layer_FireAvatar_Rock"), TEXT("Prototype_GameObject_FireAvatar_Rock"), &SocketDesc)))
				return;

			m_bCreateRock = true;
		}
	}


	else if ((m_iCurrentAnimationIndex == ATTACK_ROCK_THROW) && (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= 594) && m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() <= 605)
	{
		if (!m_bCrashRock)
		{
			CGameObject* pRock = pGameInstance->Get_GameObject(LEVEL_TUTORIAL, TEXT("Layer_FireAvatar_Rock"), 0);
			m_vStorePos = dynamic_cast<CFireAvatar_Rock*>(pRock)->Get_Position();
		}
		m_bCrashRock = true;
		/*	if (!m_bCreateSmallRock)
		{
		float j = XMVectorGetX(m_vStorePos) + (rand() % 15 / 100.f);
		float i = XMVectorGetZ(m_vStorePos) + (rand() % 10 / 100.f);
		for (int k = 0; k < 5; ++k)
		{
		m_vStorePos = XMVectorSet(i, 5.91f, j, 0.f);
		if (nullptr == (pGameInstance->Add_GameObjectToLayer(LEVEL_TUTORIAL, TEXT("Layer_Falling_Rock"), TEXT("Prototype_GameObject_Falling_Rock"), &m_vStorePos)))
		return;
		}
		m_bCreateSmallRock = true;
		}*/
	}


	RELEASE_INSTANCE(CGameInstance);
}

void CFire_Avatar::Punch_Ground(_double TimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (!m_bOnce && m_bStart)
	{
		m_dAnimSpeed = 2.0;
		m_iDuration = 5.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_iNextAnimationIndex = CRUSH_CENTER;
	}

	else if ((m_iCurrentAnimationIndex == CRUSH_CENTER) && (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 1))
	{
		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
		m_TimeDelta = 0.0;
	}


	RELEASE_INSTANCE(CGameInstance);
}

void CFire_Avatar::TwoHandPunch_Ground(_double TimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (!m_bOnce && m_bStart)
	{
		m_dAnimSpeed = 2.0;
		m_iDuration = 5.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_iNextAnimationIndex = CHRUSH_BOTH_ARMS;
	}

	else if ((m_iCurrentAnimationIndex == CHRUSH_BOTH_ARMS) && (m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_PelvisChannel()->Get_NumeKeyFrames() - 1))
	{
		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
		m_TimeDelta = 0.0;
	}

	RELEASE_INSTANCE(CGameInstance);
}

CFire_Avatar * CFire_Avatar::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CFire_Avatar*	pInstance = new CFire_Avatar(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CFire_Avatar"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CFire_Avatar::Clone(void * pArg)
{
	CFire_Avatar*	pInstance = new CFire_Avatar(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created Clone CFire_Avatar"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFire_Avatar::Free()
{
	Safe_Release(m_pModelSKL);
	Safe_Release(m_pModelCom);
	__super::Free();
}