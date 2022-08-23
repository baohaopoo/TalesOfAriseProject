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
	void AttackPattern1();		// 4��
	void AttackPattern2();		// 11��
	void AttackPattern3(_double TimeDelta);		//14~12 �ϰ� �������� ����. �ϴ��� �ӽ� �������� lookat�Լ� �������
	void AttackPattern4();		// 10��
	void AttackPattern5();		// 17~15�� ���Ĵ� ���
	void AttackPattern6();		// 18�� 360�� ȸ�� ����
	void AttackPattern7();		// 19->20�� 720�� ȸ��


	void NothingAnim1();			// �� �� �Ǹ� ȭ���� ���� 2��

	void HitPattern1();				// 22��
	void HitPattern2();				// 24��

	void NotBattleMode1();
	void NotBattleMode2();


	void StartScene();				// 32��
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
	_bool m_bStartScene = false;	// ���� ���ӿ����� false�� �����ؾ� ��
	_bool m_bOnce = false;
	_bool m_bFront = false;
	_uint m_iMotion = 0;
	_uint m_iHitMotion = 0;
	_uint m_Cnt = 0;
};

END