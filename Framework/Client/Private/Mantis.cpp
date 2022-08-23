#include "stdafx.h"
#include "..\Public\Mantis.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Channel.h"
#include "UI.h"
CMantis::CMantis(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CEnemy(pDeviceOut, pDeviceContextOut)
{

}

CMantis::CMantis(const CMantis & rhs)
	: CEnemy(rhs)
{
}

HRESULT CMantis::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMantis::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;


	m_pModelCom->Set_AnimationIndex(MOVE_IDLE);

	m_bOnce = false;
	m_bStart = true;
	m_bBattle = false;
	m_iEnemyInfo.m_iHp = 50;
	return S_OK;
}

void CMantis::Tick(_double TimeDelta)
{
	m_bOnAttackCollider = false;
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CTransform*		pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
	if (nullptr == pPlayerTransform)
		return;

	_vector pPlayerPos = pPlayerTransform -> Get_State(CTransform::STATE_POSITION);

	if (!m_bBattle)  //전투중이 아니라면 하는 행동패턴
	{
		m_TimeDelta += TimeDelta;

		if (m_TimeDelta > 5.0)
		{
			m_bStart = true;

			if (m_pTransformCom->Distance(pPlayerPos, 10.f))
				NotBattleMode1();

			else
				NotBattleMode2();
		}
	}

	if (!m_bStartScene && m_bBattle)
	{
		m_bStart = true;
		StartScene();

		////Tutorial 레벨 다 꺼버리기
		CLayer* TutorialLayer = pGameInstance->Find_Layer(LEVEL_TUTORIAL, TEXT("Layer_TutorialUI"));
		list<CGameObject*>& TutorialList = TutorialLayer->Get_ObjectList();

		for (auto& iter : TutorialList)
		{
			dynamic_cast<CUI*>(iter)->RenderChange(false);
		}

		//BattleUI 생성하기




	}


	if ((m_iEnemyInfo.m_iHp > 0 && !m_bAfterColAnim) || m_bAttackRevenge)
	{
		if ((!m_bAttackCollision && m_bStartScene && m_bBattle) || m_bAttackRevenge)		// 플레이어가 공격을 하지 않는다면 동작 개시
		{
			m_TimeDelta += TimeDelta;

			if ((m_TimeDelta > 5.0) && (m_bOnce == false))
			{
				m_bStart = true;

				if (m_iMotion == 6)
					m_iMotion = 0;

				else
					m_iMotion++;
			}

			switch (m_iMotion)
			{
			case 0:
				AttackPattern1();
				break;

			case 1:
				AttackPattern2();
				break;

			case 2:
				AttackPattern3();
				break;

			case 3:
				AttackPattern4();
				break;

			case 4:
				AttackPattern5();
				break;

			case 5:
				NothingAnim();
				break;

			default:
				return;
			}
		}
	}

	if (m_iCurrentAnimationIndex == m_iNextAnimationIndex)
	{
		m_pModelCom->Monster_Update(TimeDelta*m_dAnimSpeed * m_dTimeSpeed, m_bCutAnimation);

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
		m_pModelCom->Monster_Update_NextAnim(TimeDelta*m_dAnimSpeed * m_dTimeSpeed, m_iDuration, m_iNextAnimationIndex, &m_iCurrentAnimationIndex);
	}


	Compute_Collider();

	m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());
	m_pSphereCom->Update(m_pTransformCom->Get_WorldMatrix());
	m_pAttackSphereCom->Update(XMMatrixIdentity());

	__super::Tick(TimeDelta);
	RELEASE_INSTANCE(CGameInstance);
}

void CMantis::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CTransform*		pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
	if (nullptr == pPlayerTransform)
		return;

	_vector vPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);

	if (!m_bAttackRevenge)
	{
		if (m_iEnemyInfo.m_iHp > 0 && !m_bAfterColAnim)
		{
			if (m_bAttackCollision && m_bBattle)
			{
				m_bHit = true;
				m_bAttackCollision = false;
				m_bCutAnimation = false;

				if (!(m_pTransformCom->FrontPlayer(vPos)))
				{
					m_iDuration = 10.0;
					m_bCutAnimation = false;
					m_iNextAnimationIndex = DOWN_UNIQUE;
					m_bOnce = false;
				}

				else
				{
					m_bCutAnimation = false;
					m_iNextAnimationIndex = DAMAGE_LOOP;
					m_bOnce = false;
				}
			}
		}
	}

	if (m_iCurrentAnimationIndex == DAMAGE_LOOP || m_iCurrentAnimationIndex == DOWN_UNIQUE)
	{
		m_bAfterColAnim = true;
		if (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1)
		{
			m_iDuration = 10.0;
			m_bCutAnimation = true;
			m_iNextAnimationIndex = MOVE_IDLE;
			m_bAfterColAnim = false;
		}
	}

	if (m_iEnemyInfo.m_iHp <= 0)
	{
		m_bHit = false;
		m_bCutAnimation = false;
		m_dAnimSpeed = 1.0;
		m_iNextAnimationIndex = DEAD;
		m_bDeadOnce = true;
	}

	if (m_bDeadOnce)
	{
		m_dDeathTime += TimeDelta;
	}

	if ((m_iCurrentAnimationIndex == DEAD) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{
		if (m_dDeathTime > 6.f)
			m_bDead = true;
	}


	if (m_iCurrentAnimationIndex != DEAD)
	{
		if (m_bAfterColAnim)			// 몬스터가 계속 맞기만 하면 공격을 할 수 없기에 반격할 기회를 주기위해 만든 함수
		{
			m_dColTime += TimeDelta;
			if (m_dColTime > 4.f)
			{
				m_bAfterColAnim = false;
				m_bAttackRevenge = true;
			}
		}

		else
		{
			m_dColTime = 0.0;
		}
	}
	//if (m_bAttackCollision && m_bBattle)
	//{
	//	m_TimeDelta += TimeDelta;

	//	if (m_TimeDelta > 5.0)
	//	{
	//		m_bStart = true;

	//		if (m_iHitMotion == 3)
	//			m_iHitMotion = 0;

	//		else
	//			m_iHitMotion++;
	//	}

	//	switch (m_iHitMotion)
	//	{
	//	case 0:
	//		HitPattern1();
	//		break;

	//	case 1:
	//		HitPattern2();
	//		break;

	//	case 2:
	//		HitPattern3();
	//		break;
	//	}
	//}

	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CMantis::Render()
{
	if (nullptr == m_pShaderCom || nullptr == m_pModelCom)
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

	if (m_bOnAttackCollider)
	{
		m_pAttackSphereCom->Render();
	}

	return S_OK;
}

void CMantis::AttackPattern1()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CTransform*		pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
	if (nullptr == pPlayerTransform)
		return;

	_vector vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	//_vector vTargetPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_vector Pos = vPlayerPos - vPos;
	_vector NLook = XMVector3Normalize(vLook);
	_vector NPos = XMVector3Normalize(Pos);
	_vector Look = NLook - NPos;


	if ((m_iCurrentAnimationIndex == CUT_IN_TWO) && (0.12f < XMVectorGetX(XMVector3Length(Look))))
	{
		_vector LookF = XMVector3Normalize(Look) * 0.11f;
		vLook = XMVector3Normalize(vLook) - LookF;
		m_pTransformCom->Look(vLook);
	}
	else if (m_iCurrentAnimationIndex == CUT_IN_TWO)
	{
		m_pTransformCom->Look(NPos);
	}

	if (!m_bOnce && m_bStart)
	{
		m_iDuration = 5.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_iNextAnimationIndex = CUT_IN_TWO;
	}

	else if ((m_iCurrentAnimationIndex == CUT_IN_TWO) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{
		m_dAnimSpeed = 2.0;
		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
		m_TimeDelta = 0.0;
		m_bAttackRevenge = false;
	}

	if (m_iCurrentAnimationIndex == CUT_IN_TWO)
	{
		if (m_pModelCom->Get_CurAnimation()->Get_CenterChannel()->Get_CurrentKeyFrameIndex() >= 148 && m_pModelCom->Get_CurAnimation()->Get_CenterChannel()->Get_CurrentKeyFrameIndex() <= 155)
		{
			m_bOnAttackCollider = true;
			m_pAttackSphereCom->Set(m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_pTransformCom->Get_State(CTransform::STATE_LOOK)*1.1f, 3.f);
		}
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CMantis::AttackPattern2()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CTransform*		pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
	if (nullptr == pPlayerTransform)
		return;

	_vector vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	//_vector vTargetPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_vector Pos = vPlayerPos - vPos;
	_vector NLook = XMVector3Normalize(vLook);
	_vector NPos = XMVector3Normalize(Pos);
	_vector Look = NLook - NPos;


	if ((m_iCurrentAnimationIndex == LEFT_SICKLE) && (0.12f < XMVectorGetX(XMVector3Length(Look))))
	{
		_vector LookF = XMVector3Normalize(Look) * 0.1f;
		vLook = XMVector3Normalize(vLook) - LookF;
		m_pTransformCom->Look(vLook);
	}
	else if (m_iCurrentAnimationIndex == LEFT_SICKLE)
	{
		m_pTransformCom->Look(NPos);
	}

	if (!m_bOnce && m_bStart)
	{
		m_iDuration = 0.25;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_iNextAnimationIndex = LEFT_SICKLE;
	}

	else if ((m_iCurrentAnimationIndex == LEFT_SICKLE) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{
		m_dAnimSpeed = 2.5;
		m_iNextAnimationIndex = RIGHT_SICKLE;
	}

	else if ((m_iCurrentAnimationIndex == RIGHT_SICKLE) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{
		m_dAnimSpeed = 2.0;
		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
		m_TimeDelta = 0.0;
		m_bAttackRevenge = false;
	}

	if (m_iCurrentAnimationIndex == LEFT_SICKLE)
	{
		if (m_pModelCom->Get_CurAnimation()->Get_CenterChannel()->Get_CurrentKeyFrameIndex() >= 75 && m_pModelCom->Get_CurAnimation()->Get_CenterChannel()->Get_CurrentKeyFrameIndex() <= 83)
		{
			m_bOnAttackCollider = true;
			m_pAttackSphereCom->Set(m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_pTransformCom->Get_State(CTransform::STATE_LOOK)*1.1f + XMVectorSet(0.f, 3.f, 0.f, 0.f), 2.f);
		}
	}

	else if (m_iCurrentAnimationIndex == RIGHT_SICKLE)
	{
		if (m_pModelCom->Get_CurAnimation()->Get_CenterChannel()->Get_CurrentKeyFrameIndex() >= 49 && m_pModelCom->Get_CurAnimation()->Get_CenterChannel()->Get_CurrentKeyFrameIndex() <= 60)
		{
			m_bOnAttackCollider = true;
			m_pAttackSphereCom->Set(m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_pTransformCom->Get_State(CTransform::STATE_LOOK)*1.1f + XMVectorSet(0.f, 3.f, 0.f, 0.f), 2.f);
		}
	}



	RELEASE_INSTANCE(CGameInstance);
}

void CMantis::AttackPattern3()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CTransform*		pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
	if (nullptr == pPlayerTransform)
		return;

	_vector vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	//_vector vTargetPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_vector Pos = vPlayerPos - vPos;
	_vector NLook = XMVector3Normalize(vLook);
	_vector NPos = XMVector3Normalize(Pos);
	_vector Look = NLook - NPos;


	if ((m_iCurrentAnimationIndex == HUM) && (0.12f < XMVectorGetX(XMVector3Length(Look))))
	{
		_vector LookF = XMVector3Normalize(Look) * 0.11f;
		vLook = XMVector3Normalize(vLook) - LookF;
		m_pTransformCom->Look(vLook);
	}
	else if (m_iCurrentAnimationIndex == HUM)
	{
		m_pTransformCom->Look(NPos);
	}

	if (!m_bOnce && m_bStart)
	{
		m_iDuration = 5.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_iNextAnimationIndex = HUM;
	}

	else if ((m_iCurrentAnimationIndex == HUM) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{
		m_dAnimSpeed = 2.0;
		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
		m_TimeDelta = 0.0;
		m_bAttackRevenge = false;
	}

	if (m_iCurrentAnimationIndex == HUM)
	{
		m_bOnAttackCollider = true;
		m_pAttackSphereCom->Set(m_pTransformCom->Get_State(CTransform::STATE_POSITION) + XMVectorSet(0.f, 2.f, 0.f, 0.f), 3.f);
	}


	RELEASE_INSTANCE(CGameInstance);
}

void CMantis::AttackPattern4()
{
	if (!m_bOnce && m_bStart)
	{
		m_iDuration = 5.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_iNextAnimationIndex = MAGIC_START;
	}

	else if ((m_iCurrentAnimationIndex == MAGIC_START) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{
		m_iNextAnimationIndex = MAGIC_LOOP;
	}

	else if ((m_iCurrentAnimationIndex == MAGIC_LOOP) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{
		m_iNextAnimationIndex = MAGIC_EMIT;
	}

	else if ((m_iCurrentAnimationIndex == MAGIC_EMIT) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{
		m_iNextAnimationIndex = MAGIC_END;
	}

	else if ((m_iCurrentAnimationIndex == MAGIC_END) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{
		m_dAnimSpeed = 2.0;
		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
		m_TimeDelta = 0.0;
		m_bAttackRevenge = false;
	}

	if (m_iCurrentAnimationIndex == MAGIC_LOOP)
	{
		m_bOnAttackCollider = true;
		m_pAttackSphereCom->Set(m_pTransformCom->Get_State(CTransform::STATE_POSITION) + XMVectorSet(0.f, 2.f, 0.f, 0.f), 4.f);
	}


}

void CMantis::AttackPattern5()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CTransform*		pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
	if (nullptr == pPlayerTransform)
		return;

	_vector vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	//_vector vTargetPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_vector Pos = vPlayerPos - vPos;
	_vector NLook = XMVector3Normalize(vLook);
	_vector NPos = XMVector3Normalize(Pos);
	_vector Look = NLook - NPos;


	if ((m_iCurrentAnimationIndex == SHOCK_WAVE) && (0.12f < XMVectorGetX(XMVector3Length(Look))))
	{
		_vector LookF = XMVector3Normalize(Look) * 0.11f;
		vLook = XMVector3Normalize(vLook) - LookF;
		m_pTransformCom->Look(vLook);
	}
	else if (m_iCurrentAnimationIndex == SHOCK_WAVE)
	{
		m_pTransformCom->Look(NPos);
	}

	if (!m_bOnce && m_bStart)
	{
		m_iDuration = 5.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_dAnimSpeed = 2.3;
		m_iNextAnimationIndex = SHOCK_WAVE;
	}

	else if ((m_iCurrentAnimationIndex == SHOCK_WAVE) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{
		m_dAnimSpeed = 2.0;
		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
		m_TimeDelta = 0.0;
		m_bAttackRevenge = false;
	}

	if (m_iCurrentAnimationIndex == SHOCK_WAVE)
	{
		if (m_pModelCom->Get_CurAnimation()->Get_CenterChannel()->Get_CurrentKeyFrameIndex() >= 224 && m_pModelCom->Get_CurAnimation()->Get_CenterChannel()->Get_CurrentKeyFrameIndex() <= 240)
		{
			m_bOnAttackCollider = true;
			m_pAttackSphereCom->Set(m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_pTransformCom->Get_State(CTransform::STATE_LOOK)*1.f + m_pTransformCom->Get_State(CTransform::STATE_RIGHT) + XMVectorSet(0.f, 2.f, 0.f, 0.f), 3.f);
		}
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CMantis::NothingAnim()
{

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CTransform*		pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
	if (nullptr == pPlayerTransform)
		return;

	_vector vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	//_vector vTargetPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_vector Pos = vPlayerPos - vPos;
	_vector NLook = XMVector3Normalize(vLook);
	_vector NPos = XMVector3Normalize(Pos);
	_vector Look = NLook - NPos;


	if ((m_iCurrentAnimationIndex == PREDATION_LOOP) && (0.14f < XMVectorGetX(XMVector3Length(Look))))
	{
		_vector LookF = XMVector3Normalize(Look) * 0.13f;
		vLook = XMVector3Normalize(vLook) - LookF;
		m_pTransformCom->Look(vLook);
		m_pTransformCom->Chase(vPlayerPos, 0.1);
	}
	else if (m_iCurrentAnimationIndex == PREDATION_LOOP)
	{
		m_pTransformCom->Look(NPos);
	}


	if (!m_bOnce && m_bStart)
	{
		m_iDuration = 5.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_iNextAnimationIndex = PREDATION_START;
	}

	else if ((m_iCurrentAnimationIndex == PREDATION_START) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{
		m_iNextAnimationIndex = PREDATION_LOOP;
	}

	else if ((m_iCurrentAnimationIndex == PREDATION_LOOP) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{
		m_iNextAnimationIndex = PREDATION_END;
	}

	else if ((m_iCurrentAnimationIndex == PREDATION_END) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{
		m_dAnimSpeed = 2.0;
		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
		m_TimeDelta = 0.0;
		m_bAttackRevenge = false;
	}


	RELEASE_INSTANCE(CGameInstance);
}

void CMantis::MovePattern()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CTransform*		pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
	if (nullptr == pPlayerTransform)
		return;

	_vector vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	//_vector vTargetPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_vector Pos = vPlayerPos - vPos;
	_vector NLook = XMVector3Normalize(vLook);
	_vector NPos = XMVector3Normalize(Pos);
	_vector Look = NLook - NPos;


	if ((m_iCurrentAnimationIndex == MOVE_WALK_F) && (0.21f < XMVectorGetX(XMVector3Length(Look))))
	{
		_vector LookF = XMVector3Normalize(Look) * 0.2f;
		vLook = XMVector3Normalize(vLook) - LookF;
		m_pTransformCom->Look(vLook);
		m_pTransformCom->Chase(vPlayerPos, 0.1);
	}
	else if (m_iCurrentAnimationIndex == MOVE_WALK_F)
	{
		m_pTransformCom->Look(NPos);
	}

	if (!m_bOnce && m_bStart)
	{
		m_iDuration = 5.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_dAnimSpeed = 2.3;
		m_iNextAnimationIndex = MOVE_WALK_F;
	}

	else if ((m_iCurrentAnimationIndex == MOVE_WALK_F) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{
		m_dAnimSpeed = 2.0;
		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
		m_TimeDelta = 0.0;
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CMantis::HitPattern1()
{
	if (m_iCurrentAnimationIndex != DAMAGE_LOOP)
	{
		m_bOnce = false;
		m_bStart = true;
	}

	if (!m_bOnce && m_bStart)
	{
		m_iDuration = 5.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_dAnimSpeed = 2.3;
		m_iNextAnimationIndex = DAMAGE_LOOP;
	}

	else if ((m_iCurrentAnimationIndex == DAMAGE_LOOP) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{
		m_dAnimSpeed = 2.0;
		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
		m_TimeDelta = 0.0;
	}
}

void CMantis::HitPattern2()
{
	if (m_iCurrentAnimationIndex != DAMAGE_SMALL_B)
	{
		m_bOnce = false;
	}

	if (!m_bOnce && m_bStart)
	{
		m_iDuration = 5.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_dAnimSpeed = 2.3;
		m_iNextAnimationIndex = DAMAGE_SMALL_B;
	}

	else if ((m_iCurrentAnimationIndex == DAMAGE_SMALL_B) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{
		m_dAnimSpeed = 2.0;
		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
		m_TimeDelta = 0.0;
	}
}


void CMantis::HitPattern3()
{
	if (m_iCurrentAnimationIndex != DAMAGE_SMALL_F)
	{
		m_bOnce = false;
	}

	if (!m_bOnce && m_bStart)
	{
		m_iDuration = 5.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_dAnimSpeed = 2.3;
		m_iNextAnimationIndex = DAMAGE_SMALL_F;
	}

	else if ((m_iCurrentAnimationIndex == DAMAGE_SMALL_F) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{
		m_dAnimSpeed = 2.0;
		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
		m_TimeDelta = 0.0;
	}
}

void CMantis::NotBattleMode1()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CTransform*		pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
	if (nullptr == pPlayerTransform)
		return;

	_vector vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	//_vector vTargetPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_vector Pos = vPlayerPos - vPos;
	_vector NLook = XMVector3Normalize(vLook);
	_vector NPos = XMVector3Normalize(Pos);
	_vector Look = NLook - NPos;

	if (m_iCurrentAnimationIndex == IDLE)
	{
		m_bOnce = false;
		m_bStart = true;
	}


	if (!m_bOnce && m_bStart)
	{
		m_iDuration = 5.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_dAnimSpeed = 2.0;
		m_iNextAnimationIndex = DETECT_IDLE;
	}

	else if ((m_iCurrentAnimationIndex == DETECT_IDLE) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{
		//m_bCutAnimation = true;
		m_iNextAnimationIndex = DETECT_STOP;
	}

	else if ((m_iCurrentAnimationIndex == DETECT_STOP) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{
		m_dAnimSpeed = 2.0;
		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
		m_bStart = true;
		m_TimeDelta = 0.0;
	}

	if ((m_iCurrentAnimationIndex == DETECT_IDLE) && (0.11f < XMVectorGetX(XMVector3Length(Look))))
	{
		_vector LookF = XMVector3Normalize(Look) * 0.1f;
		vLook = XMVector3Normalize(vLook) - LookF;
		m_pTransformCom->Look(vLook);
	}
	else if (m_iCurrentAnimationIndex == DETECT_IDLE)
	{
		m_pTransformCom->Look(NPos);
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CMantis::NotBattleMode2()
{
	if (m_iCurrentAnimationIndex == DETECT_IDLE || m_iCurrentAnimationIndex == DETECT_STOP)
	{
		m_bOnce = false;
		m_bStart = true;
	}

	if (!m_bOnce && m_bStart)
	{
		m_iDuration = 5.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_dAnimSpeed = 2.0;
		m_iNextAnimationIndex = IDLE;
	}

	else if ((m_iCurrentAnimationIndex == IDLE) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{
		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
		m_TimeDelta = 0.0;
		m_bStart = true;
	}

}

void CMantis::StartScene()
{
	if (!m_bOnce && m_bStart)
	{
		m_iDuration = 5.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_dAnimSpeed = 2.0;
		m_iNextAnimationIndex = ADVENT;
	}

	else if ((m_iCurrentAnimationIndex == ADVENT) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{
		m_dAnimSpeed = 2.0;
		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
		m_TimeDelta = 0.0;
		m_bStartScene = true;
	}

}

HRESULT CMantis::SetUp_Components()
{
	///* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnim"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Model"), LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_Mantis"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_Collider */
	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	/* For.Com_OBB */
	ColliderDesc.vPosition = _float3(0.f, 0.7f, 0.f);
	ColliderDesc.vSize = _float3(0.7f, 1.4f, 0.7f);

	if (FAILED(__super::SetUp_Components(TEXT("Com_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_SPHERE */
	ColliderDesc.vPosition = _float3(0.f, 0.5f, 0.f);
	ColliderDesc.fRadius = 1.f;

	if (FAILED(__super::SetUp_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
		return E_FAIL;

	ColliderDesc.vPosition = _float3(0.f, 0.5f, 0.f);
	ColliderDesc.fRadius = 1.f;

	if (FAILED(__super::SetUp_Components(TEXT("Com_AttackSPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pAttackSphereCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMantis::SetUp_ConstantTable()
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

CMantis * CMantis::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CMantis*	pInstance = new CMantis(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CMantis"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMantis::Clone(void * pArg)
{
	CMantis*	pInstance = new CMantis(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created Clone CMantis"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMantis::Free()
{
	__super::Free();
	Safe_Release(m_pModelCom);
}