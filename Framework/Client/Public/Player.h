#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Player_Manager.h"
#include "Time_Manager.h"
#include "Battle_Manager.h"
#include "Equipment.h"
#include "Enemy.h"
#include "Camera_Default.h"

BEGIN(Engine)
class CShader;
class CCollider;
class CRenderer;
class CModel;
class CNavigation;
END

BEGIN(Client)

class CPlayer abstract : public CGameObject
{

protected:
	enum PLAYER_STATE
	{
		PS_SPECIAL, PS_ATTACK, PS_IDLE, PS_WALK, PS_FORWARD_EVADE, PS_BACKWARD_EVADE, PS_RUN, PS_DIE, PS_END
	};

	enum PLAYER_DIR
	{
		DIR_W, DIR_WD, DIR_D, DIR_DS, DIR_S, DIR_SA, DIR_A, DIR_AW, DIR_END
	};

	enum PLAYER_MOVE_AI_TYPE
	{
		PLAYER_MOVE_AI_CHASE, PLAYER_MOVE_AI_EVADE, PLAYER_MOVE_AI_MOVE, PLAYER_MOVE_AI_END
	};

	//typedef struct TagPlayerInfo
	//{

	//	_int m_iMaxHp = 0;
	//	_int m_iCurrentHp = 0;
	//	_int m_iComboHits = 0;
	//	_int m_iMaxAg = 0;
	//	_int m_iCurrentAg = 0;

	//}PLAYERINFO;


	//캐릭터 UI 관련 변수들 
	typedef struct TagPlayerInfo
	{
		_int m_iMaxHp = 0;
		_int m_iCurrentHp = 0;
		_int m_iComboHits = 0;
		_int m_iMaxAg = 0;
		_int m_iCurrentAg = 0;

	}PLAYERINFO;

	PLAYERINFO m_tPlayerInfo;

protected:
	CPlayer(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg, CTransform::TRANSFORMDESC* pTransformDesc);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_LightDepth() override;

protected:
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CModel*				m_pModelCom = nullptr;
	CModel*				m_pModelFaceCom = nullptr;
	CModel*				m_pModelHairCom = nullptr;
	CModel*				m_pModelSKLCom = nullptr;
	CCollider*			m_pOBBCom = nullptr;
	CCollider*			m_pSphereCom = nullptr;
	CCollider*			m_pAttackSphereCom = nullptr;
	CNavigation*		m_pNaviCom = nullptr;

public:
	CNavigation* SetUp_Navigation(const TCHAR* pTagNaviCompoent);	// 타고 움직일 Navi_Component를 설정하는 함수
	_bool Delete_NaviCom(void);		// 네비 컴포넌트를 삭제하는 함수

public:
	virtual CGameObject* Clone(void* pArg) override = 0;
	virtual void Free() override;

	void Set_EquipmentBattle(_bool bBattle);
	void Set_EquipmentField(_bool bField);
	void Set_LControlDown(_bool bDown) { m_bLControlDown = bDown; }

public:


	void System_Key_Input();



public:
	void Set_Auto(_bool bAuto) { m_bAuto = bAuto; }
	void Set_Reset();
	void Set_Stop(_bool bStop) { m_bStop = bStop; }
	void Set_Battle(_bool bBattle) { m_bBattle = bBattle; }
	_vector Get_PlayerPos() { return m_pTransformCom->Get_State(CTransform::STATE_POSITION); }
	_vector Get_PlayerLook() { return m_pTransformCom->Get_State(CTransform::STATE_LOOK); }
	void Set_PlayerPos(_fvector vPos) { m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos); }
	void Set_VecMonsters(vector<CEnemy*>* pVecMonster) { m_pVecMonsters = pVecMonster; }
	void Set_VecMonstersIter(vector<CEnemy*>::iterator iter) { m_iter = iter; }
	void Set_TargetEnemy(CEnemy* pEnemy) { m_pTargetEnemy = pEnemy; }
	void Change_Targeting(_bool bRight);
	void Set_IsHitedMonster(_uint iIndex, _bool m_bIsHit) { m_bIsHitedMonsterArray[iIndex] = m_bIsHit; }
	_bool Get_IsHitedMonster(_uint iIndex) { return m_bIsHitedMonsterArray[iIndex]; }
	_bool Get_Overlap() { return m_bOverlap; }
	_bool Get_Invincible() { return m_bInvincible; }


	PLAYERINFO Get_PlayerInfo() { return m_tPlayerInfo; }

	vector<CEnemy*>::iterator Get_Targeting() { return m_iter; }
	CEnemy* Get_TargetEnemy() { return m_pTargetEnemy; }

	CBattle_Manager::DAMAGEINFO Get_AttackDamageInfo() { return m_AttackDamageInfo; }
	void Set_HitDamageInfo(CBattle_Manager::DAMAGEINFO DamageInfo) { m_HitDamageInfo = DamageInfo; }
	_vector Get_TargetPos();


	void Set_Collision(_bool bCollision) { m_bAttackCollision = bCollision; }
	void Set_KnockBackDir(_fvector vDir) { XMStoreFloat3(&m_vKnockBackDir, vDir); }

protected:



	CPlayer_Manager* m_pPlayer_Manager = nullptr;
	CTime_Manager* m_pTime_Manager = nullptr;
	CBattle_Manager* m_pBattle_Manager = nullptr;

	_bool m_bStopAnimation = false;
	_bool m_bLoop = true;
	_double m_dAnimSpeed = 1.75;
	_double m_iLinearTime = 1.0;		// 선형보간 Term식
	_uint m_iCurrentAnimationIndex = 0;
	_uint m_iNextAnimationIndex = 0;
	_uint m_iWaitAnimationIndex = 0;
	_double m_dFallSpeed = 0.0;
	_double m_dJumpSpeed = 0.0;
	_double m_dGravityPower = 0.4;



	_bool m_bMove = false;
	_bool m_bAttack = false;
	_bool m_bAirAttack = false;
	_bool m_bAttackKeyInput = false;
	_bool m_bEvade = false;
	_bool m_bEvadeBack = false;
	_bool m_bSkill = false; //?
	_bool m_bStop = false;

	_bool m_bStartSkill = false;
	_bool m_bMoveBrake = false;
	_bool m_bLand = true;
	_bool m_bJump = false;
	_bool m_bAir = false; //공중인지 점프기//점프  피타격도 공중/지상 
	_bool m_bAIJump = false;
	_bool m_bAIJumpStart = false;
	_bool m_bSkillKeyDown = false;
	_bool m_bKeyInput = false;
	_bool m_bKeyInputE = false;
	_bool m_bKeyInputR = false;
	_bool m_bKeyInputF = false;
	CEnemy* m_pTargetEnemy = nullptr;


	//공중제한
	_uint m_iAbleJumpCount = 1;
	_uint m_iAbleAirEvadeCount = 1;
	_uint m_iAbleAirAttackCount = 1;


	_int Get_Hp() {return m_tPlayerInfo.m_iCurrentHp; }

	//충돌처리 관련 변수들

	CBattle_Manager::DAMAGEINFO m_AttackDamageInfo;
	_bool m_bOnAttackCollider = false;
	_bool m_bOverlap = false; // 관통 가능한지 아닌지.
	_bool m_bInvincible = false;  //무적
	CBattle_Manager::DAMAGEINFO m_HitDamageInfo;
	_bool m_bIsHitedMonsterArray[4]{ false };

	_float3 m_vKnockBackDir;
	_bool m_bAttackCollision = false;



	_bool m_bHit = false;
	_bool m_bBlow_Down_b = false;
	_bool m_bBlow_Down_f = false;
	_bool m_bBlow_Mighty = false;
	_bool m_bBlow_Up_b = false;
	_bool m_bBlow_Up_f = false;
	_bool m_bDamage_Air_Large_b = false;
	_bool m_bDamage_Air_Large_f = false;
	_bool m_bDamage_Air_Loop = false;
	_bool m_bDamage_Air_Small_b = false;
	_bool m_bDamage_Air_Small_f = false;
	_bool m_bDamage_Large_b = false;
	_bool m_bDamage_Large_f = false;
	_bool m_bDamage_Loop = false;
	_bool m_bDamage_Small_b = false;
	_bool m_bDamage_Small_f = false;
	_bool m_bDown_b = false;
	_bool m_bDown_f = false;




	//시스템키인풋
	_bool m_bLControlDown = false;



	//다수 캐릭터 관리 관련.
	_bool m_bAuto = false;	//Player가 ai인지 아닌지.
	_bool m_bBattle = false; //Battle중 객체를 이떄 렌더및 기능해야할듯.
	_bool m_bTargetChange = false;
	_bool m_bField = false;	//
	

	//AI변수들
	_bool m_bPattern = false;
	_bool m_bAIGroundSkillStart = false;
	_double m_MoveTime = 0.0;





	_float3 m_vMoveBrakeDir;
	PLAYER_STATE m_ePlayerState = PS_END;
	PLAYER_DIR	 m_ePlayerDir = DIR_END;


	vector<CEquipment*> vecEquipment;
	vector<CEnemy*>* m_pVecMonsters = nullptr;
	vector<CEnemy*>::iterator m_iter;

	_bool m_bBlur = false;



public:
	void Set_CurBattleMap_Height(_double dHeight) { m_dCurrBattleMap_Height = dHeight; }
	_double Get_CurBattleMap_Height(void) { return m_dCurrBattleMap_Height; }

protected:
	_double m_dCurrBattleMap_Height = 0.f;
};

END