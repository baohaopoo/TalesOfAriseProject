#include "stdafx.h"
#include "..\Public\Punisher.h"
#include "GameInstance.h"
#include "Model.h"
#include "Animation.h"
#include "Channel.h"

CPunisher::CPunisher(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CEnemy(pDeviceOut, pDeviceContextOut)
{

}

CPunisher::CPunisher(const CPunisher & rhs)
	: CEnemy(rhs)
{
}

HRESULT CPunisher::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPunisher::NativeConstruct(void * pArg)
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

void CPunisher::Tick(_double TimeDelta)
{
	m_bOnAttackCollider = false;
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	CTransform*		pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
	if (nullptr == pPlayerTransform)
		return;

	_vector pPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);


	if (!m_bBattle)  // 전투중이 아니라면 하는 행동패턴
	{
		m_TimeDelta += TimeDelta;

		if (m_TimeDelta > 5.0)
		{
			m_bStart = true;

			if (m_pTransformCom->Distance(pPlayerPos, 4.f))
				NotBattleMode1();

			else
				NotBattleMode2();
		}
	}

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

				if (m_iMotion == 8)
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
				AttackPattern3(TimeDelta);
				break;

			case 3:
				AttackPattern4();
				break;

			case 4:
				AttackPattern5();
				break;

			case 5:
				AttackPattern6();
				break;

			case 6:
				AttackPattern7();
				break;

			case 7:
				NothingAnim1();
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
		m_pModelCom->Monster_Update_NextAnim(TimeDelta*m_dAnimSpeed*m_dTimeSpeed, m_iDuration, m_iNextAnimationIndex, &m_iCurrentAnimationIndex);
	}

	//_matrix      Weapon1BoneMatrix = XMLoadFloat4x4(m_pModelCom->Get_CombinedTransformationMatrix("weapon_01"));  //대소문자 구별해야함... Weapon 하면터짐
	//_matrix      Weapon2BoneMatrix = XMLoadFloat4x4(m_pModelCom->Get_CombinedTransformationMatrix("weapon_02"));
	//Weapon1BoneMatrix.r[0] = XMVector3Normalize(Weapon1BoneMatrix.r[0]);
	//Weapon1BoneMatrix.r[1] = XMVector3Normalize(Weapon1BoneMatrix.r[1]);
	//Weapon1BoneMatrix.r[2] = XMVector3Normalize(Weapon1BoneMatrix.r[2]);

	//Weapon2BoneMatrix.r[0] = XMVector3Normalize(Weapon2BoneMatrix.r[0]);
	//Weapon2BoneMatrix.r[1] = XMVector3Normalize(Weapon2BoneMatrix.r[1]);
	//Weapon2BoneMatrix.r[2] = XMVector3Normalize(Weapon2BoneMatrix.r[2]);

	//Weapon1BoneMatrix = Weapon1BoneMatrix*XMLoadFloat4x4(&m_pModelCom->Get_PivotMatrix());
	//Weapon2BoneMatrix = Weapon2BoneMatrix*XMLoadFloat4x4(&m_pModelCom->Get_PivotMatrix());



	//m_pWeapon1AttackSphereCom->Update(m_pTransformCom->Get_WorldMatrix());
	//m_pWeapon2AttackSphereCom->Update(m_pTransformCom->Get_WorldMatrix());

	//_float4x4 Weapon1Matrix, Weapon2Matrix;
	//XMStoreFloat4x4(&Weapon1Matrix, Weapon1BoneMatrix);
	//XMStoreFloat4x4(&Weapon2Matrix, Weapon2BoneMatrix);



	Compute_Collider();

	m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());
	m_pSphereCom->Update(m_pTransformCom->Get_WorldMatrix());
	m_pAttackSphereCom->Update(XMMatrixIdentity());

	__super::Tick(TimeDelta);
	RELEASE_INSTANCE(CGameInstance);
}

void CPunisher::LateTick(_double TimeDelta)
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
				//if (m_bHit)
				//{
				//	m_pModelCom->Get_CurAnimation()->Reset_MonsterAnimation();
				//}

				m_bHit = true;
				m_bAttackCollision = false;
				m_bCutAnimation = false;

				if (!(m_pTransformCom->FrontPlayer(vPos)))
				{
					//m_bAfterColAnim = true;
					m_bCutAnimation = false;
					m_iNextAnimationIndex = DOWN_B;
					m_bOnce = false;
				}

				else
				{
					//m_bAfterColAnim = true;
					m_bCutAnimation = false;
					m_iNextAnimationIndex = DOWN_UNIQUE;
					m_bOnce = false;
				}
			}
		}
	}

	if (m_iCurrentAnimationIndex == DOWN_B || m_iCurrentAnimationIndex == DOWN_UNIQUE)
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
		m_dAnimSpeed = 2.0;
		m_iNextAnimationIndex = DEAD;
		m_bDeadOnce = true;
	}

	if (m_bDeadOnce)
	{
		m_dDeathTime += TimeDelta;
	}

	if ((m_iCurrentAnimationIndex == DEAD) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{
		if (m_dDeathTime > 7.f)
			m_bDead = true;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////
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


	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CPunisher::Render()
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

void CPunisher::AttackPattern1()
{
	if (!m_bOnce && m_bStart)
	{
		m_dAnimSpeed = 2.5;
		m_iDuration = 5.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_iNextAnimationIndex = IN_OUT_UPPER;
	}

	else if (m_iCurrentAnimationIndex == IN_OUT_UPPER)
	{

		if (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1)
		{
			m_dAnimSpeed = 2.0;
			m_bCutAnimation = true;
			m_iNextAnimationIndex = MOVE_IDLE;
			m_bOnce = false;
			m_TimeDelta = 0.0;
			m_bAttackRevenge = false;

		}


		if ((m_pModelCom->Get_CurAnimation()->Get_CenterChannel()->Get_CurrentKeyFrameIndex() >= 64 && m_pModelCom->Get_CurAnimation()->Get_CenterChannel()->Get_CurrentKeyFrameIndex() <= 90))
		{
			m_bOnAttackCollider = true;
			m_pAttackSphereCom->Set(m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_pTransformCom->Get_State(CTransform::STATE_LOOK) + XMVectorSet(0.f, 1.f, 0.f, 0.f), 1.8f);
		}



		else if (m_pModelCom->Get_CurAnimation()->Get_CenterChannel()->Get_CurrentKeyFrameIndex() >= 190 && m_pModelCom->Get_CurAnimation()->Get_CenterChannel()->Get_CurrentKeyFrameIndex() <= 215)
		{
			m_bOnAttackCollider = true;
			m_pAttackSphereCom->Set(m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_pTransformCom->Get_State(CTransform::STATE_LOOK) + XMVectorSet(0.f, 3.f, 0.f, 0.f), 1.8f);
		}
	}
}

void CPunisher::AttackPattern2()
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


	if ((m_iCurrentAnimationIndex == MULTI) && (0.12f < XMVectorGetX(XMVector3Length(Look))))
	{
		_vector LookF = XMVector3Normalize(Look) * 0.11f;
		vLook = XMVector3Normalize(vLook) - LookF;
		m_pTransformCom->Look(vLook);
	}
	else if (m_iCurrentAnimationIndex == MULTI)
	{
		m_pTransformCom->Look(NPos);
	}

	if (!m_bOnce && m_bStart)
	{
		m_dAnimSpeed = 2.5;
		m_iDuration = 5.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_iNextAnimationIndex = MULTI;
	}


	else if (m_iCurrentAnimationIndex == MULTI)
	{
		if (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1)
		{
			m_dAnimSpeed = 2.0;
			m_bCutAnimation = true;
			m_iNextAnimationIndex = MOVE_IDLE;
			m_bOnce = false;
			m_TimeDelta = 0.0;
			m_bAttackRevenge = false;
		}


		if ((m_pModelCom->Get_CurAnimation()->Get_CenterChannel()->Get_CurrentKeyFrameIndex() >= 100 && m_pModelCom->Get_CurAnimation()->Get_CenterChannel()->Get_CurrentKeyFrameIndex() <= 115)
			|| (m_pModelCom->Get_CurAnimation()->Get_CenterChannel()->Get_CurrentKeyFrameIndex() >= 123 && m_pModelCom->Get_CurAnimation()->Get_CenterChannel()->Get_CurrentKeyFrameIndex() <= 135)
			|| (m_pModelCom->Get_CurAnimation()->Get_CenterChannel()->Get_CurrentKeyFrameIndex() >= 148 && m_pModelCom->Get_CurAnimation()->Get_CenterChannel()->Get_CurrentKeyFrameIndex() <= 156)
			|| (m_pModelCom->Get_CurAnimation()->Get_CenterChannel()->Get_CurrentKeyFrameIndex() >= 168 && m_pModelCom->Get_CurAnimation()->Get_CenterChannel()->Get_CurrentKeyFrameIndex() <= 173)
			|| (m_pModelCom->Get_CurAnimation()->Get_CenterChannel()->Get_CurrentKeyFrameIndex() >= 187 && m_pModelCom->Get_CurAnimation()->Get_CenterChannel()->Get_CurrentKeyFrameIndex() <= 193)
			|| (m_pModelCom->Get_CurAnimation()->Get_CenterChannel()->Get_CurrentKeyFrameIndex() >= 212 && m_pModelCom->Get_CurAnimation()->Get_CenterChannel()->Get_CurrentKeyFrameIndex() <= 220))
		{
			m_bOnAttackCollider = true;
			m_pAttackSphereCom->Set(m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_pTransformCom->Get_State(CTransform::STATE_LOOK), 1.8f);
		}

		else if ((m_pModelCom->Get_CurAnimation()->Get_CenterChannel()->Get_CurrentKeyFrameIndex() >= 280 && m_pModelCom->Get_CurAnimation()->Get_CenterChannel()->Get_CurrentKeyFrameIndex() <= 310))
		{
			m_bOnAttackCollider = true;
			m_pAttackSphereCom->Set(m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_pTransformCom->Get_State(CTransform::STATE_LOOK)*1.3f, 1.f);
		}

	}

	RELEASE_INSTANCE(CGameInstance);
}

void CPunisher::AttackPattern3(_double TimeDelta)
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


	if ((m_iCurrentAnimationIndex == RUSH_LOOP) && (0.11f < XMVectorGetX(XMVector3Length(Look))))
	{
		_vector LookF = XMVector3Normalize(Look) * 0.1f;
		vLook = XMVector3Normalize(vLook) - LookF;
		m_pTransformCom->Look(vLook);
	}

	else if (m_iCurrentAnimationIndex == RUSH_LOOP)
	{
		m_pTransformCom->Look(NPos);
	}


	if (m_iCurrentAnimationIndex == RUSH_LOOP)		// 콜라이더 생성용 조건문
	{
		m_bOnAttackCollider = true;
		m_pAttackSphereCom->Set(m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_pTransformCom->Get_State(CTransform::STATE_LOOK), 1.8f);
	}


	if (!m_bOnce && m_bStart)
	{
		m_dAnimSpeed = 3.0;
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

	else if (m_iCurrentAnimationIndex == RUSH_LOOP)
	{
		//m_pTransformCom->LookAt(vPlayerPos);
		if (!m_pTransformCom->Distance(vPlayerPos, 3.f))
		{
			m_bCutAnimation = false;
			if (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1)
			{
				m_pModelCom->Get_CurAnimation()->Reset_MonsterAnimation();
			}
		}

		else
		{
			m_iDuration = 5.0;
			m_dAnimSpeed = 2.0;
			m_bCutAnimation = true;
			m_iNextAnimationIndex = MOVE_IDLE;
			m_bOnce = false;
			m_TimeDelta = 0.0;
			m_bAttackRevenge = false;
		}
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CPunisher::AttackPattern4()
{

	if (m_iCurrentAnimationIndex == HANDSTAND_FOOTPRESS)
	{
		if (m_pModelCom->Get_CurAnimation()->Get_CenterChannel()->Get_CurrentKeyFrameIndex() >= 150 && m_pModelCom->Get_CurAnimation()->Get_CenterChannel()->Get_CurrentKeyFrameIndex() <= 290)
		{
			m_bOnAttackCollider = true;
			m_pAttackSphereCom->Set(m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_pTransformCom->Get_State(CTransform::STATE_LOOK), 4.f);
		}
	}


	if (!m_bOnce && m_bStart)
	{
		m_dAnimSpeed = 2.5;
		m_iDuration = 5.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_iNextAnimationIndex = HANDSTAND_FOOTPRESS;
	}

	else if ((m_iCurrentAnimationIndex == HANDSTAND_FOOTPRESS) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{
		m_dAnimSpeed = 2.0;
		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
		m_TimeDelta = 0.0;
		m_bAttackRevenge = false;
	}
}

void CPunisher::AttackPattern5()
{
	if (!m_bOnce && m_bStart)
	{
		m_dAnimSpeed = 2.5;
		m_iDuration = 5.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_iNextAnimationIndex = SWING_START;
	}

	else if ((m_iCurrentAnimationIndex == SWING_START) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{
		m_iNextAnimationIndex = SWING_LOOP;
	}

	else if ((m_iCurrentAnimationIndex == SWING_LOOP) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{
		m_iNextAnimationIndex = SWING_END;
	}

	else if ((m_iCurrentAnimationIndex == SWING_END) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{
		m_dAnimSpeed = 2.0;
		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
		m_TimeDelta = 0.0;
		m_bAttackRevenge = false;
	}


	if (m_iCurrentAnimationIndex == SWING_START)
	{
		if (m_pModelCom->Get_CurAnimation()->Get_CenterChannel()->Get_CurrentKeyFrameIndex() >= 178 && m_pModelCom->Get_CurAnimation()->Get_CenterChannel()->Get_CurrentKeyFrameIndex() <= 181)
		{
			m_bOnAttackCollider = true;
			m_pAttackSphereCom->Set(m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_pTransformCom->Get_State(CTransform::STATE_LOOK), 5.f);
		}
	}

	else if (m_iCurrentAnimationIndex == SWING_LOOP)
	{
		if ((m_pModelCom->Get_CurAnimation()->Get_CenterChannel()->Get_CurrentKeyFrameIndex() >= 23 && m_pModelCom->Get_CurAnimation()->Get_CenterChannel()->Get_CurrentKeyFrameIndex() <= 29)
			|| (m_pModelCom->Get_CurAnimation()->Get_CenterChannel()->Get_CurrentKeyFrameIndex() >= 39 && m_pModelCom->Get_CurAnimation()->Get_CenterChannel()->Get_CurrentKeyFrameIndex() <= 45)
			|| (m_pModelCom->Get_CurAnimation()->Get_CenterChannel()->Get_CurrentKeyFrameIndex() >= 50 && m_pModelCom->Get_CurAnimation()->Get_CenterChannel()->Get_CurrentKeyFrameIndex() <= 54))
		{
			m_bOnAttackCollider = true;
			m_pAttackSphereCom->Set(m_pTransformCom->Get_State(CTransform::STATE_POSITION) - m_pTransformCom->Get_State(CTransform::STATE_RIGHT), 3.f);
		}
	}
}

void CPunisher::AttackPattern6()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CTransform*		pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
	if (nullptr == pPlayerTransform)
		return;

	_vector vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);

	//인중이가 알려준 코드
	_vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	//_vector vTargetPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_vector Pos = vPlayerPos - vPos;
	_vector NLook = XMVector3Normalize(vLook);
	_vector NPos = XMVector3Normalize(Pos);
	_vector Look = NLook - NPos;


	if ((m_iCurrentAnimationIndex == SWING_360) && (0.11f < XMVectorGetX(XMVector3Length(Look))))
	{
		_vector LookF = XMVector3Normalize(Look) * 0.1f;
		vLook = XMVector3Normalize(vLook) - LookF;
		m_pTransformCom->Look(vLook);
	}
	else if (m_iCurrentAnimationIndex == SWING_360)
	{
		m_pTransformCom->Look(NPos);
	}

	if (!m_bOnce && m_bStart)
	{
		m_dAnimSpeed = 2.5;
		m_iDuration = 5.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_iNextAnimationIndex = SWING_360;
	}

	else if ((m_iCurrentAnimationIndex == SWING_360) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{
		m_dAnimSpeed = 2.0;
		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
		m_TimeDelta = 0.0;
		m_bAttackRevenge = false;
	}

	if (m_iCurrentAnimationIndex == SWING_360)
	{
		if (m_pModelCom->Get_CurAnimation()->Get_CenterChannel()->Get_CurrentKeyFrameIndex() >= 127 && m_pModelCom->Get_CurAnimation()->Get_CenterChannel()->Get_CurrentKeyFrameIndex() <= 135)
		{
			m_bOnAttackCollider = true;
			m_pAttackSphereCom->Set(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 5.f);
		}
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CPunisher::AttackPattern7()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CTransform*		pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
	if (nullptr == pPlayerTransform)
		return;

	_vector vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);

	//인중이가 알려준 코드
	_vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	//_vector vTargetPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_vector Pos = vPlayerPos - vPos;
	_vector NLook = XMVector3Normalize(vLook);
	_vector NPos = XMVector3Normalize(Pos);
	_vector Look = NLook - NPos;


	if ((m_iCurrentAnimationIndex == SWORD_720_FIRST) && (0.11f < XMVectorGetX(XMVector3Length(Look))))
	{
		_vector LookF = XMVector3Normalize(Look) * 0.1f;
		vLook = XMVector3Normalize(vLook) - LookF;
		m_pTransformCom->Look(vLook);
	}
	else if (m_iCurrentAnimationIndex == SWORD_720_FIRST)
	{
		m_pTransformCom->Look(NPos);
	}

	if (!m_bOnce && m_bStart)
	{
		m_dAnimSpeed = 2.5;
		m_iDuration = 5.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_iNextAnimationIndex = SWORD_720_FIRST;
	}

	else if ((m_iCurrentAnimationIndex == SWORD_720_FIRST) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{
		m_dAnimSpeed = 2.0;
		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
		m_TimeDelta = 0.0;
		m_bAttackRevenge = false;
	}

	if (m_iCurrentAnimationIndex == SWORD_720_FIRST)
	{
		if (m_pModelCom->Get_CurAnimation()->Get_CenterChannel()->Get_CurrentKeyFrameIndex() >= 105 && m_pModelCom->Get_CurAnimation()->Get_CenterChannel()->Get_CurrentKeyFrameIndex() <= 150)
		{
			m_bOnAttackCollider = true;
			m_pAttackSphereCom->Set(m_pTransformCom->Get_State(CTransform::STATE_POSITION) + XMVectorSet(0.f, 3.f, 0.f, 0.f), 5.f);
		}
	}

	RELEASE_INSTANCE(CGameInstance);
}



void CPunisher::NothingAnim1()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CTransform*		pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
	if (nullptr == pPlayerTransform)
		return;

	_vector vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);

	//인중이가 알려준 코드
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
		m_iDuration = 5.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_iNextAnimationIndex = BRAVE;
	}

	else if ((m_iCurrentAnimationIndex == BRAVE) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{
		m_dAnimSpeed = 2.0;
		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
		m_TimeDelta = 0.0;
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CPunisher::HitPattern1()
{
	if (!m_bOnce && m_bStart)
	{
		m_iDuration = 5.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_iNextAnimationIndex = DOWN_B;
	}

	else if ((m_iCurrentAnimationIndex == DOWN_B) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{
		m_dAnimSpeed = 2.0;
		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
		m_TimeDelta = 0.0;
	}
}

void CPunisher::HitPattern2()
{
	if (!m_bOnce && m_bStart)
	{
		m_iDuration = 5.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_iNextAnimationIndex = DOWN_UNIQUE;
	}

	else if ((m_iCurrentAnimationIndex == DOWN_UNIQUE) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{
		m_dAnimSpeed = 2.0;
		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
		m_TimeDelta = 0.0;
	}
}

void CPunisher::NotBattleMode1()
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


	if ((m_iCurrentAnimationIndex == MOVE_IDLE) && (0.12f < XMVectorGetX(XMVector3Length(Look))))
	{
		_vector LookF = XMVector3Normalize(Look) * 0.11f;
		vLook = XMVector3Normalize(vLook) - LookF;
		m_pTransformCom->Look(vLook);
	}
	else if (m_iCurrentAnimationIndex == MOVE_IDLE)
	{
		m_pTransformCom->Look(NPos);
	}

	if (m_iCurrentAnimationIndex == LOOKOUT)
	{
		m_bOnce = false;
		m_bStart = true;
	}


	if (!m_bOnce && m_bStart)
	{
		m_dAnimSpeed = 1.0;
		m_iDuration = 10.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_iNextAnimationIndex = IDLE;
	}

	else if ((m_iCurrentAnimationIndex == IDLE) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{
		m_dAnimSpeed = 2.0;
		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
		m_TimeDelta = 0.0;
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CPunisher::NotBattleMode2()
{
	if (m_iCurrentAnimationIndex == IDLE)
	{
		m_bOnce = false;
		m_bStart = true;
	}

	if (!m_bOnce && m_bStart)
	{
		m_dAnimSpeed = 1.0;
		m_iDuration = 10.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_iNextAnimationIndex = LOOKOUT;
	}

	else if ((m_iCurrentAnimationIndex == LOOKOUT) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{
		m_dAnimSpeed = 2.0;
		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
		m_TimeDelta = 0.0;
	}
}

void CPunisher::StartScene()
{
	if (!m_bOnce && m_bStart)
	{
		m_iDuration = 5.0;
		m_bStart = false;
		m_bOnce = true;
		m_bCutAnimation = false;
		m_iNextAnimationIndex = SYMBOL_DETECT_IDLE;
	}

	else if ((m_iCurrentAnimationIndex == SYMBOL_DETECT_IDLE) && (m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_CurrentKeyFrameIndex() >= m_pModelCom->Get_CurAnimation()->Get_MainChannel()->Get_NumeKeyFrames() - 1))
	{
		m_dAnimSpeed = 2.0;
		m_bCutAnimation = true;
		m_iNextAnimationIndex = MOVE_IDLE;
		m_bOnce = false;
		m_TimeDelta = 0.0;
		m_bStartScene = true;
	}

}



HRESULT CPunisher::SetUp_Components()
{
	///* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnim"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Model"), LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_Punisher"), (CComponent**)&m_pModelCom)))
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

	/* For.Com_SPHERE */
	ColliderDesc.vPosition = _float3(0.f, 0.5f, 0.f);
	ColliderDesc.fRadius = 1.f;

	if (FAILED(__super::SetUp_Components(TEXT("Com_AttackSPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pAttackSphereCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPunisher::SetUp_ConstantTable()
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

CPunisher * CPunisher::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CPunisher*	pInstance = new CPunisher(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CPunisher"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPunisher::Clone(void * pArg)
{
	CPunisher*	pInstance = new CPunisher(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created Clone CPunisher"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPunisher::Free()
{
	__super::Free();
	Safe_Release(m_pModelCom);
}

//void CPunisher::Compute_Collider()
//{
//	m_pAttackSphereCom->Set_On(m_bOnAttackCollider);
//}
