#include "stdafx.h"
#include "..\Public\Boar.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Channel.h"
#include "Player.h"
#include "GameObject.h"

CBoar::CBoar(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CEnemy(pDeviceOut, pDeviceContextOut)
{

}

CBoar::CBoar(const CBoar & rhs)
	: CEnemy(rhs)
{
}

HRESULT CBoar::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoar::NativeConstruct(void * pArg)
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
	//_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	//vPos = XMVectorSetY(vPos, 0.f);

	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(20.f, 0.f, 10.f, 1.f));

	m_bOnce = false;
	m_bStart = true;
	m_bBattle = false;
	m_iEnemyInfo.m_iHp = 50;
	return S_OK;
}

void CBoar::Tick(_double TimeDelta)
{
	m_bOnAttackCollider = false;
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CTransform*		pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
	if (nullptr == pPlayerTransform)
		return;

	_vector pPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);

	if (!m_bBattle)  // 전투중이 아니라면 하는 행동패턴
	{
		m_TimeDelta += TimeDelta;

		if (m_TimeDelta > 5.0)
		{
			//int iRand = rand() % 2;
			m_bStart = true;

			if (m_pTransformCom->Distance(pPos, 10.f))
				WaitPlayer1();

			else
			{
				//if (iRand == 0)
				//	WaitPlayer2();

				//else
				WaitPlayer3();
			}
		}
	}

	/*if (!m_bBattle)
	{
	m_TimeDelta += TimeDelta;

	if (m_TimeDelta > 5.0)
	{
	m_bStart = true;

	if (m_iWaitMotion == 3)
	m_iWaitMotion = 0;

	else
	m_iWaitMotion++;
	}


	switch (m_iWaitMotion)
	{
	case 0:
	{
	if (m_pTransformCom->Distance(pPos, 10.f))
	WaitPlayer1();

	else
	{
	m_bCutAnimation = true;
	m_iNextAnimationIndex = MOVE_IDLE;
	}
	break;
	}
	case 1:
	{
	if (m_pTransformCom->Distance(pPos, 20.f))
	WaitPlayer2();

	else
	{
	m_bCutAnimation = true;
	m_iNextAnimationIndex = MOVE_IDLE;
	}

	break;
	}

	case 2:
	WaitPlayer3();
	break;

	default:
	return;
	}
	}*/

	if (!m_bStartScene && m_bBattle)
	{
		m_bStart = true;
		StartScene();
	}

	if ((m_iEnemyInfo.m_iHp > 0 && !m_bAfterColAnim) || m_bAttackRevenge)
	{
		if ((!m_bAttackCollision && m_bStartScene && m_bBattle) || m_bAttackRevenge)		// 플레이어가 공격을 하지 않는다면 동작 개시
		{
			m_TimeDelta += TimeDelta;

			if ((m_TimeDelta > 5.0) && (m_bOnce == false))
			{
				m_bStart = true;


				if (m_iMotion == 5)
					m_iMotion = 0;

				else
					m_iMotion++;
			}


			switch (m_iMotion)
			{
			case 0:
				AttackPattern1(TimeDelta);

				break;

			case 1:
			{
				if (!(m_pTransformCom->LeftPlayer(pPos)))
					AttackPattern2();

				else
					AttackPattern3();

				break;
			}

			case 2:
				AttackPattern4();
				break;


			case 3:
				AttackPattern5(TimeDelta);
				break;

			case 4:
				NothingAnim();
				break;

			default:
				return;
			}
		}
	}

	if (m_iCurrentAnimationIndex == m_iNextAnimationIndex)
	{
		m_pModelCom->Monster_Update(TimeDelta*m_dAnimSpeed, m_bCutAnimation);

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
		m_pModelCom->Monster_Update_NextAnim(TimeDelta*m_dAnimSpeed, m_iDuration, m_iNextAnimationIndex, &m_iCurrentAnimationIndex);
	}

	Compute_Collider();

	m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());
	m_pSphereCom->Update(m_pTransformCom->Get_WorldMatrix());
	m_pAttackSphereCom->Update(XMMatrixIdentity());

	__super::Tick(TimeDelta);
	RELEASE_INSTANCE(CGameInstance);
}

void CBoar::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CTransform*		pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
	if (nullptr == pPlayerTransform)
		return;

	_vector vPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_float yPos = XMVectorGetY(vPos);
	_vector boarPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	boarPos = XMVectorSetY(boarPos, yPos);

	if (m_iCurrentAnimationIndex == DOWN_UNIQUE || m_iCurrentAnimationIndex == DOWN_F)
	{
		m_dAnimSpeed = 1.5;
	}


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
					m_iNextAnimationIndex = DOWN_B;
					m_bOnce = false;
				}

				else
				{
					m_iDuration = 10.0;
					m_bCutAnimation = false;
					m_iNextAnimationIndex = DOWN_F;
					m_bOnce = false;
				}
			}
		}
	}


	if (m_iCurrentAnimationIndex == DOWN_B || m_iCurrentAnimationIndex == DOWN_F)
	{
		m_bAfterColAnim = true;
		if (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1)
		{
			m_iDuration = 10.0;
			m_bCutAnimation = true;
			m_bOnce = false;
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
			if (m_dColTime > 5.f)
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


	//if (m_bHit /*&& 특정기술사용시(만약 공중으로 띄우는 공격에 맞았다면)*/)
	//{
	//	m_bSky = true;
	//	HitPattern1();
	//	m_pTransformCom->Set_State(CTransform::STATE_POSITION, boarPos);
	//}

	////else if (!m_bHit && (XMVectorGetY(boarPos) < 2.5f && XMVectorGetY(boarPos) > 2.f))	// 잠시 수치값좀 바꿔놓자. 나중엔 감소시켜야 한다.
	////{
	////	m_bStart = true;
	////	FallToGround();
	////}

	//else if (!m_bHit && m_bSky && XMVectorGetY(boarPos) > 0.2f)
	//{
	//	m_pTransformCom->Set_State(CTransform::STATE_POSITION, boarPos - XMVectorSet(0.f, 0.001f, 0.f, 0.f));
	//	FallToGround();
	//}

	//else if (m_bSky && (XMVectorGetY(boarPos) < 0.2f))
	//{
	//	boarPos = XMVectorSetY(boarPos, 0.f);
	//	m_pTransformCom->Set_State(CTransform::STATE_POSITION, boarPos);
	//	m_bSky = false;
	//}

	//if (m_bHit && !m_bSky && m_pTransformCom->LeftPlayer(vPos))	// 플레이어에게 맞고, 땅에서, 왼쪽에서 맞았다면
	//{
	//	HitPattern2();
	//}

	//else if (m_bHit && !m_bSky && !(m_pTransformCom->LeftPlayer(vPos)))
	//{
	//	HitPattern3();
	//}


	////if (m_iEnemyInfo.m_iBreakCount > 20)		// 플레이어에게 공격당한 횟수가 20이상이면 공격개시
	////{
	////	m_bHit = false;
	////	m_iEnemyInfo.m_iBreakCount = 0;
	////}

	RELEASE_INSTANCE(CGameInstance);
}


void CBoar::StartScene()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (!m_bOnce && m_bStart)
	{
		m_iDuration = 5.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_iNextAnimationIndex = ADVENT;
	}

	else if ((m_iCurrentAnimationIndex == ADVENT) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{
		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
		m_TimeDelta = 0.0;
		m_bStartScene = true;
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CBoar::AttackPattern1(_double TimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (!m_bOnce && m_bStart)
	{
		m_dAnimSpeed = 2.0;
		m_iDuration = 5.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_iNextAnimationIndex = RUSH_START;
	}

	else if ((m_iCurrentAnimationIndex == RUSH_START) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{
		m_iNextAnimationIndex = RUSH_LOOP;
	}

	else if ((m_iCurrentAnimationIndex == RUSH_LOOP) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{
		m_iNextAnimationIndex = RUSH_END;
	}

	else if ((m_iCurrentAnimationIndex == RUSH_END) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{
		m_bFront = false;
		m_dAnimSpeed = 2.0;
		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
		m_TimeDelta = 0.0;
		m_bAttackRevenge = false;
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CBoar::AttackPattern2()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	if (m_iCurrentAnimationIndex == TACKLE_RIGHT)
	{
		if (m_pModelCom->Get_CurAnimation()->Get_CenterChannel()->Get_CurrentKeyFrameIndex() >= 60 && m_pModelCom->Get_CurAnimation()->Get_CenterChannel()->Get_CurrentKeyFrameIndex() <= 88)
		{
			m_bOnAttackCollider = true;
			m_pAttackSphereCom->Set(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 1.f);
		}
	}


	if (!m_bOnce && m_bStart)
	{
		m_iDuration = 5.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_iNextAnimationIndex = TACKLE_RIGHT;
	}

	else if ((m_iCurrentAnimationIndex == TACKLE_RIGHT) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
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


void CBoar::AttackPattern3()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (m_iCurrentAnimationIndex == TACKLE_LEFT)
	{
		if (m_pModelCom->Get_CurAnimation()->Get_CenterChannel()->Get_CurrentKeyFrameIndex() >= 63 && m_pModelCom->Get_CurAnimation()->Get_CenterChannel()->Get_CurrentKeyFrameIndex() <= 90)
		{
			m_bOnAttackCollider = true;
			m_pAttackSphereCom->Set(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 1.f);
		}
	}

	if (!m_bOnce && m_bStart)
	{
		m_dAnimSpeed = 2.0;
		m_iDuration = 5.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_iNextAnimationIndex = TACKLE_LEFT;
	}

	else if ((m_iCurrentAnimationIndex == TACKLE_LEFT) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{
		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
		m_TimeDelta = 0.0;
		m_bAttackRevenge = false;
	}


	RELEASE_INSTANCE(CGameInstance);
}

void CBoar::AttackPattern4()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (m_iCurrentAnimationIndex == RUSH_SHORT)
	{
		if (m_pModelCom->Get_CurAnimation()->Get_CenterChannel()->Get_CurrentKeyFrameIndex() >= 177 && m_pModelCom->Get_CurAnimation()->Get_CenterChannel()->Get_CurrentKeyFrameIndex() <= 195)
		{
			m_bOnAttackCollider = true;
			m_pAttackSphereCom->Set(m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_pTransformCom->Get_State(CTransform::STATE_LOOK)*0.8f, 1.f);
		}
	}


	if (!m_bOnce && m_bStart)
	{
		m_dAnimSpeed = 2.0;
		m_iDuration = 5.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_iNextAnimationIndex = RUSH_SHORT;
	}

	else if ((m_iCurrentAnimationIndex == RUSH_SHORT) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{

		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
		m_TimeDelta = 0.0;
		m_bAttackRevenge = false;
	}



	RELEASE_INSTANCE(CGameInstance);
}

void CBoar::AttackPattern5(_double TimeDelta)
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


	if ((m_iCurrentAnimationIndex == GIGNAT_RUSH_START) && (0.16f < XMVectorGetX(XMVector3Length(Look))))
	{
		_vector LookF = XMVector3Normalize(Look) * 0.15f;
		vLook = XMVector3Normalize(vLook) - LookF;
		m_pTransformCom->Look(vLook);
	}
	else if (m_iCurrentAnimationIndex == GIGNAT_RUSH_START)
	{
		m_pTransformCom->Look(NPos);
	}

	if (!m_bOnce && m_bStart)
	{
		m_dAnimSpeed = 2.0;
		m_iDuration = 5.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_iNextAnimationIndex = GIGNAT_RUSH_START;
	}

	else if ((m_iCurrentAnimationIndex == GIGNAT_RUSH_START) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{
		m_iNextAnimationIndex = GIGANT_RUSH_LOOP;
	}

	else if ((m_iCurrentAnimationIndex == GIGANT_RUSH_LOOP) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{
		m_iDuration = 1.0;
		m_iNextAnimationIndex = GIGANT_RUSH_END;
	}

	else if ((m_iCurrentAnimationIndex == GIGANT_RUSH_END) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{

		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
		m_TimeDelta = 0.0;
		m_bAttackRevenge = false;
	}

	if (m_iCurrentAnimationIndex == GIGANT_RUSH_LOOP)
	{
		m_bOnAttackCollider = true;
		m_pAttackSphereCom->Set(m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_pTransformCom->Get_State(CTransform::STATE_LOOK)*0.5f, 1.f);
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CBoar::FallToGround()
{
	if (!m_bOnce && m_bStart)
	{
		m_dAnimSpeed = 2.0;
		m_iDuration = 5.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_iNextAnimationIndex = DOWN_F;
	}

	else if ((m_iCurrentAnimationIndex == DOWN_F) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{

		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
		m_TimeDelta = 0.0;
		m_bSky = false;
	}

}

void CBoar::HitPattern1()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CTransform*		pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
	if (nullptr == pPlayerTransform)
		return;

	_vector pPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);

	if (m_bSky /* && y축으로 계속 증가할 때 */)		//안에 조건식 다시 짜야한다
	{
		m_dAnimSpeed = 2.0;
		m_iDuration = 5.0;
		m_bOnce = true;
		m_bCutAnimation = true;
		m_iNextAnimationIndex = DAMAGE_AIR_LOOP;
	}

	else // y축이 감소하기 시작할 때
	{
		m_bCutAnimation = false;
		m_iNextAnimationIndex = DOWN_F;
		m_bOnce = false;
		m_TimeDelta = 0.0;
		m_bSky = false;
	}

	//else if ((m_iCurrentAnimationIndex == DAMAGE_AIR_LOOP) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	//{

	//	m_bCutAnimation = true;
	//	m_iNextAnimationIndex = DOWN_F;
	//	m_bOnce = false;
	//	m_TimeDelta = 0.0;
	//}

	RELEASE_INSTANCE(CGameInstance);
}

void CBoar::HitPattern2()
{
	if (!m_bOnce)
	{
		m_dAnimSpeed = 2.0;
		m_iDuration = 5.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_iNextAnimationIndex = DAMAGE_LARGE_R;
	}

	else if ((m_iCurrentAnimationIndex == DAMAGE_LARGE_R) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{

		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
	}
}

void CBoar::HitPattern3()
{
	if (!m_bOnce)
	{
		m_dAnimSpeed = 2.0;
		m_iDuration = 5.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_iNextAnimationIndex = DAMAGE_LARGE_L;
	}

	else if ((m_iCurrentAnimationIndex == DAMAGE_LARGE_L) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{

		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
	}
}

void CBoar::NothingAnim()
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


	if ((m_iCurrentAnimationIndex == BRAVE) && (0.16f < XMVectorGetX(XMVector3Length(Look))))
	{
		_vector LookF = XMVector3Normalize(Look) * 0.15f;
		vLook = XMVector3Normalize(vLook) - LookF;
		m_pTransformCom->Look(vLook);
	}
	else if (m_iCurrentAnimationIndex == BRAVE)
	{
		m_pTransformCom->Look(NPos);
	}

	if (!m_bOnce && m_bStart)
	{
		m_dAnimSpeed = 2.0;
		m_iDuration = 5.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_iNextAnimationIndex = BRAVE;
	}

	else if ((m_iCurrentAnimationIndex == BRAVE) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{

		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CBoar::WaitPlayer1()
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

	if (m_iCurrentAnimationIndex == SYMBOL_LOOKOUT || m_iCurrentAnimationIndex == SYMBOL_IDLE)
	{
		m_bOnce = false;
		m_bStart = true;
	}

	if (!m_bOnce && m_bStart)
	{
		m_dAnimSpeed = 2.0;
		m_iDuration = 3.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_iNextAnimationIndex = DETECT_IDLE;
	}

	else if ((m_iCurrentAnimationIndex == DETECT_IDLE) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{
		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
		m_TimeDelta = 0.0;
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CBoar::WaitPlayer2()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (m_iCurrentAnimationIndex == DETECT_IDLE || m_iCurrentAnimationIndex == SYMBOL_IDLE)
	{
		m_bOnce = false;
		m_bStart = true;
	}

	if (!m_bOnce && m_bStart)
	{
		m_dAnimSpeed = 2.0;
		m_iDuration = 3.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_iNextAnimationIndex = SYMBOL_LOOKOUT;
	}

	else if ((m_iCurrentAnimationIndex == SYMBOL_LOOKOUT) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{

		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
		m_TimeDelta = 0.0;
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CBoar::WaitPlayer3()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (m_iCurrentAnimationIndex == DETECT_IDLE || m_iCurrentAnimationIndex == SYMBOL_LOOKOUT)
	{
		m_bOnce = false;
		m_bStart = true;
	}

	if (!m_bOnce && m_bStart)
	{
		m_dAnimSpeed = 2.0;
		m_iDuration = 3.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_iNextAnimationIndex = SYMBOL_IDLE;
	}

	else if ((m_iCurrentAnimationIndex == SYMBOL_IDLE) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{

		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
		m_TimeDelta = 0.0;
	}

	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CBoar::Render()
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

	return S_OK;
}



HRESULT CBoar::SetUp_Components()
{
	///* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnim"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Model"), LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_Boar"), (CComponent**)&m_pModelCom)))
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

HRESULT CBoar::SetUp_ConstantTable()
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

CBoar * CBoar::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CBoar*	pInstance = new CBoar(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CBoar"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBoar::Clone(void * pArg)
{
	CBoar*	pInstance = new CBoar(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created Clone CBoar"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoar::Free()
{
	__super::Free();
	Safe_Release(m_pModelCom);
}