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
	// ü���� �������Ϸ� �������� MOVE_IDLE��� COM_1060�� ����Ѵ�
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();
	void TurnLeft(_double TimeDelta);				// ����� ������ �� ��� �ϴ� ����
	void TurnRight(_double TimeDelta);				// ����� ������ �� ��� �ϴ� ����
	void AttackPattern1(_double TimeDelta);			// ������ ���� �����ٰ� ���� strikeġ�� ���
	void AttackPattern2(_double TimeDelta);			// �� ������ �� ������ ������ �߻��ϴ� ���ݸ��			// �߻�����ϴ� ������ lookat Player�ؾ��Ѵ�
	void AttackPattern3(_double TimeDelta);			// �� �����ٰ� ���� �����ؼ� ������ ������ ���		// ü�� 4���� 1���ư��� ���� �� ����̴�
	void AttackPattern4(_double TimeDelta);			// ���� ���� ��� �� ���� �ֵθ��� ���							// �׳� ���°� �� �׷�... 4->17(Ű������ 110) ��� ���ӱ� ����
	void AttackPattern5(_double TimeDelta);			// ���ڸ����� ���� ������ ���
													// ��������� ����1-1 �ʿ� ������ ����
	void AttackPattern6(_double TimeDelta);			// �����ؼ� �߷� ������ ���	
	void AttackPattern7(_double TimeDelta);			// �տ� ������ ������ ���


	void HitPattern1(_double TimeDelta);			// ���ݸ¾Ƽ� �ް����� ġ�� ���						// Transform Ŭ������ �ִ� FrontPlayer()�Լ��� ����ؼ� �տ��� �¾Ҵ��� �ڿ��� �¾Ҵ��� �Ǻ��Ѵ�
	void HitPattern2(_double TimeDelta);			// ���ݸ¾� ������ ��ٶ����� ���������ִ� ���		

	void MovePattern();								// �÷��̾ �ٶ󺸵��� �ϴ� ���


	void StartScene();								// ó�� ���� ��			2��
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
