#pragma once
#include "Client_Defines.h"
#include "Enemy.h"

BEGIN(Client)

class CBoar final : public CEnemy
{
	enum BOAR_ANIM_STATE
	{
		ADVENT, ARISE_B, ARISE_F, BRAVE, GIGANT_DOUBLE_RUSH, GIGANT_RUSH_END, GIGANT_RUSH_END_DUP, GIGANT_RUSH_LOOP, GIGNAT_RUSH_LOOP_DUP, GIGNAT_RUSH_START,
		HOWLING, PUSH_UP, RUSH_END, RUSH_LOOP, RUSH_SHORT, RUSH_START, RUSH_TURN, STEP_ON, TACKLE_LEFT, TACKLE_RIGHT, BLOW_DOWN_B, BLOW_DOWN_F, BLOW_UP_B, BLOW_UP_F,
		DAMAGE_AIR_LARGE_B, DAMAGE_AIR_LARGE_F, DAMAGE_AIR_LARGE_L, DAMAGE_AIR_LARGE_R, DAMAGE_AIR_LOOP, DAMAGE_AIR_SMALL_B, DAMAGE_AIR_SMALL_F, DAMAGE_AIR_SMALL_L, DAMAGE_AIR_SMALL_R,
		DAMAGE_LARGE_B, DAMAGE_LARGE_F, DAMAGE_LARGE_L, DAMAGE_LARGE_R, DAMAGE_LOOP, DAMAGE_SMALL_B, DAMAGE_SMALL_F, DAMAGE_SMALL_L, DAMAGE_SMALL_R, DEAD, DOWN_B, DOWN_F, DOWN_UNIQUE, MOVE_IDLE, MOVE_RUN,
		WALK_B, WALK_F, TURN_L, TURN_POSE, TURN_R, DETECT_IDLE, DETECT_STOP, SYMBOL_IDLE, SYMBOL_LOOKOUT, SYMBOL_RUN, SYMBOL_STOP, TURN_LEFT, SYMBOL_TURN_POSE, TURN_RIGHT, WALK
	};

private:
	explicit CBoar(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CBoar(const CBoar& rhs);
	virtual ~CBoar() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	void StartScene();		// 첫 등장씬 돌진해서 다가오다가 두 다리 다 든다
	void AttackPattern1(_double TimeDelta);	// 돌진 공격(플레이어가 앞에 있을 때)
	void AttackPattern2();	// 오른쪽 몸통박치기 공격
	void AttackPattern3();	// 왼쪽 몸통박치기 공격
	void AttackPattern4();	// 제자리에서 짧게 돌진  14번만 사용
	void AttackPattern5(_double TimeDelta);	// 완전 돌진(9->7->5)

	void FallToGround();
	void HitPattern1();		// 플레이어에게 공격받으면 위로 뜨는 장면
	void HitPattern2();		// 플레이어에게 공격받으면 왼쪽으로 간다		36
	void HitPattern3();		// 플레이어에게 공격받으면 오른쪽으로 간다		35
	void NothingAnim();		// 괴성 지르는 모션 3번

	void WaitPlayer1();		// 플레이어가 가까이 왔다면 땅을 발로 파고 바라본다 53번
	void WaitPlayer2();		// 두리번 두리번 거리는 모션 56번
	void WaitPlayer3();		// 제자리에 가만히 있는 것 55번
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();

private:
	CModel*				m_pModelCom = nullptr;

public:
	static CBoar* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

private:
	_double	m_TimeDelta = 0.0;
	_double m_dAnimTime = 0.0;
	_double m_dHitTime = 0.0;
	_bool m_bFirstPosition = false;	// 함수 발동되자마자 bool변수 저장하고 더 이상 안바뀌게 한다
	_bool m_bStoreOnce = false;
	_bool m_bTime = false;
	_bool m_bStart = false;
	_bool m_bStartScene = false;
	_bool m_bOnce = false;
	_bool m_bSky = false; // 공중으로 뜨는 공격맞았다면 m_bSky=true가 된다
	_bool m_bFront = false;
	_bool m_bLeft = true;
	_uint m_iMotion = 0;
	_uint m_iWaitMotion = 0;
	_uint m_iHitMotion = 0;
};

END