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
	void StartScene();		// ù ����� �����ؼ� �ٰ����ٰ� �� �ٸ� �� ���
	void AttackPattern1(_double TimeDelta);	// ���� ����(�÷��̾ �տ� ���� ��)
	void AttackPattern2();	// ������ �����ġ�� ����
	void AttackPattern3();	// ���� �����ġ�� ����
	void AttackPattern4();	// ���ڸ����� ª�� ����  14���� ���
	void AttackPattern5(_double TimeDelta);	// ���� ����(9->7->5)

	void FallToGround();
	void HitPattern1();		// �÷��̾�� ���ݹ����� ���� �ߴ� ���
	void HitPattern2();		// �÷��̾�� ���ݹ����� �������� ����		36
	void HitPattern3();		// �÷��̾�� ���ݹ����� ���������� ����		35
	void NothingAnim();		// ���� ������ ��� 3��

	void WaitPlayer1();		// �÷��̾ ������ �Դٸ� ���� �߷� �İ� �ٶ󺻴� 53��
	void WaitPlayer2();		// �θ��� �θ��� �Ÿ��� ��� 56��
	void WaitPlayer3();		// ���ڸ��� ������ �ִ� �� 55��
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
	_bool m_bFirstPosition = false;	// �Լ� �ߵ����ڸ��� bool���� �����ϰ� �� �̻� �ȹٲ�� �Ѵ�
	_bool m_bStoreOnce = false;
	_bool m_bTime = false;
	_bool m_bStart = false;
	_bool m_bStartScene = false;
	_bool m_bOnce = false;
	_bool m_bSky = false; // �������� �ߴ� ���ݸ¾Ҵٸ� m_bSky=true�� �ȴ�
	_bool m_bFront = false;
	_bool m_bLeft = true;
	_uint m_iMotion = 0;
	_uint m_iWaitMotion = 0;
	_uint m_iHitMotion = 0;
};

END