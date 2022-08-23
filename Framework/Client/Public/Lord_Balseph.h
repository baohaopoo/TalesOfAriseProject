#pragma once
#include "Client_Defines.h"
#include "Enemy.h"


BEGIN(Client)

class CLord_Balseph final : public CEnemy
{
public:
	enum BEZ_ANIM_STATE
	{
		ARISE_B, ARISE_F, ATTACK_TRUN180, ATTACK_TURN90R, ATTACK_CRUSH, ATTACK_DOWN_QNIQUE_LOOP, ATTACK_FIRE_ROLL,
		ATTACK_FIRE_SWING_SECOND, ATTACK_GRENAXE, ATTACK_JUMP_CRUSH, ATTACK_JUMP_PRESS, ATTACK_JUMP_SAVE_LOOP, ATTACK_JUMP_SAVE_START,
		ATTACK_MOUNTAIN_BUSTER_END, ATTACK_MOUNTAIN_BUSTER_LOOP, ATTACK_MOUNTAIN_BUSTER_START, ATTACK_POWER_WAVE, ATTACK_SWING,
		BTL_DEAD, BTL_DOWN_B, BTL_DOWN_F, DOWN_UNIQUE, MASTER_CORE_LOOP, MASTER_CORE_START, MOVE_IDLE, MOVE_WALK_B, MOVE_WALK_F,
		MYSTIC_PYROCLASM_END, PYROCLASM_LOOP, PYROCLASM_START, TURN_L, TURN_POSE, TURN_R,
		COM_0000, COM_0001, COM_0010, COM_0021, COM_0030, COM_0040, COM_0050, COM_0060, COM_1021, COM_1050, COM_1051, COM_1060
	};


private:
	explicit CLord_Balseph(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CLord_Balseph(const CLord_Balseph& rhs);
	virtual ~CLord_Balseph() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;


private:
	// 체력이 절반이하로 떨어지면 MOVE_IDLE대신 COM_1060을 사용한다
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();
	void TurnLeft(_double TimeDelta);				// 모션이 마땅한 게 없어서 일단 보류
	void TurnRight(_double TimeDelta);				// 모션이 마땅한 게 없어서 일단 보류
	void AttackPattern1(_double TimeDelta);			// 도끼를 땅에 끌고가다가 위로 strike치는 모션
	void AttackPattern2(_double TimeDelta);			// 기 모으고 몸 주위에 에너지 발산하는 공격모션			// 발산시작하는 순간에 lookat Player해야한다
	void AttackPattern3(_double TimeDelta);			// 기 모으다가 위로 점프해서 도끼로 찍어내리는 모션		// 체력 4분의 1날아갔을 때도 이 모션이다
	void AttackPattern4(_double TimeDelta);			// 도끼 내려 찍고 한 바퀴 휘두르는 모션							// 그냥 도는거 좀 그럼... 4->17(키프레임 110) 모션 연속기 좋다
	void AttackPattern5(_double TimeDelta);			// 제자리에서 도끼 찍어내리는 모션
													// 여기까지가 보스1-1 맵에 나오는 동작
	void AttackPattern6(_double TimeDelta);			// 점프해서 발로 찍어내리는 모션	
	void AttackPattern7(_double TimeDelta);			// 손에 에너지 모으는 모션


	void HitPattern1(_double TimeDelta);			// 공격맞아서 뒷걸음질 치는 모션						// Transform 클래스에 있는 FrontPlayer()함수를 사용해서 앞에서 맞았는지 뒤에서 맞았는지 판별한다
	void HitPattern2(_double TimeDelta);			// 공격맞아 앞으로 고꾸라지며 무릎끓고있는 모션		

	void MovePattern();								// 플레이어를 바라보도록 하는 모션


	void StartScene();								// 처음 등장 씬			2번
private:
	CModel*				m_pModelCom = nullptr;
	CModel*				m_pModelFaceCom = nullptr;
	CModel*				m_pModelSKL = nullptr;
public:
	static CLord_Balseph* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
private:
	_uint m_NumAnim = 0;
	_uint m_iMotion = 0;
	_uint m_iHitMotion = 0;
	_double	m_TimeDelta = 0.0;
	_double m_Timepattern1 = 0.0;
	_double	m_makeCoreTime = 0.0;
	_double m_dStartScene = 0.0;
	_bool m_bStart = false;
	_bool m_bStartSkill = false;
	_bool m_bTime = false;
	_bool m_bOnce = false;
	_bool m_bCatch = false;
};

END
