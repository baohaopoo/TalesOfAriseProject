#pragma once
#include "Client_Defines.h"
#include "Enemy.h"

BEGIN(Client)

class CPunisher final : public CEnemy
{
	enum MANTIS_ANIM_STATE
	{
		ARISE_B, ARISE_F, BRAVE, HEAD_BEAM, IN_OUT_UPPER, SHOOT_DROP_CEILING, SHOOT_DROP_DESCEND_LOOP, SHOOT_DROP_END, DROP_RISE_LOOP,
		SHOOT_DROP_START, HANDSTAND_FOOTPRESS, MULTI, RUSH_END, RUSH_LOOP, RUSH_START, SWING_END, SWING_LOOP, SWING_START, SWING_360,
		SWORD_720_FIRST, SWORD_720_LAST, DEAD, DOWN_B, DOWN_F, DOWN_UNIQUE, EVENT_ADVENT, MOVE_IDLE, WALK_B, WALK_F, TURN_L, TURN_POSE, TURN_R,
		SYMBOL_DETECT_IDLE, SYMBOL_DETECT_STOP, IDLE, LOOKOUT, RUN, STOP, TURN_LEFT, SYMBOL_TURN_POSE, TURN_RIGHT, WALK
	};

private:
	explicit CPunisher(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CPunisher(const CPunisher& rhs);
	virtual ~CPunisher() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	void AttackPattern1();		// 4번
	void AttackPattern2();		// 11번
	void AttackPattern3(_double TimeDelta);		//14~12 하고 마지막에 돈다. 일단은 임시 방편으로 lookat함수 사용했음
	void AttackPattern4();		// 10번
	void AttackPattern5();		// 17~15번 땅파는 모션
	void AttackPattern6();		// 18번 360도 회전 공격
	void AttackPattern7();		// 19->20번 720도 회전


	void NothingAnim1();			// 몸 쭉 피며 화내는 동작 2번

	void HitPattern1();				// 22번
	void HitPattern2();				// 24번

	void NotBattleMode1();
	void NotBattleMode2();


	void StartScene();				// 32번
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();

private:
	CModel*				m_pModelCom = nullptr;

public:
	static CPunisher* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

	//private:
	//	void Compute_Collider();

private:
	_double	m_TimeDelta = 0.0;
	_bool m_bTime = false;
	_bool m_bStart = false;
	_bool m_bStartScene = false;	// 실제 게임에서는 false로 시작해야 함
	_bool m_bOnce = false;
	_bool m_bFront = false;
	_uint m_iMotion = 0;
	_uint m_iHitMotion = 0;
	_uint m_Cnt = 0;
};

END