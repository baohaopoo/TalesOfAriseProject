#pragma once
#include "Client_Defines.h"
#include "Enemy.h"

BEGIN(Client)

class CMantis final : public CEnemy
{
	enum MANTIS_ANIM_STATE
	{
		ADVENT, ARISE_B, ARISE_F, BLOW_UP, CUT_IN_TWO, HUM, LEFT_SICKLE, PREDATION_END, PREDATION_LOOP, PREDATION_START, RIGHT_SICKLE, SHOCK_WAVE,
		BLOW_DOWN_B, BLOW_DOWN_F, BLOW_UP_B, BLOW_UP_F, DAMAGE_AIR_LARGE_B, DAMAGE_AIR_LARGE_F, DAMAGE_AIR_LARGE_L, DAMAGE_AIR_LARGE_R, DAMAGE_AIR_LOOP,
		DAMAGE_AIR_SMALL_B, DAMAGE_AIR_SMALL_F, DAMAGE_AIR_SMALL_L, DAMAGE_AIR_SMALL_R, DAMAGE_DEAD, DAMAGE_LARGE_B, DAMAGE_LARGE_F, DAMAGE_LARGE_L, DAMAGE_LARGE_R, DAMAGE_LOOP,
		DAMAGE_SMALL_B, DAMAGE_SMALL_F, DAMAGE_SMALL_L, DAMAGE_SMALL_R, DEAD, DOWN_B, DOWN_F, DOWN_UNIQUE, MAGIC_EMIT, MAGIC_END, MAGIC_LOOP, MAGIC_START, MOVE_IDLE, MOVE_RUN,
		MOVE_WALK_B, MOVE_WALK_F, TURN_L, TURN_POSE, TURN_R, DETECT_IDLE, DETECT_STOP, IDLE, RUN, STOP, TURN_LEFT, TURN_POSE2, TURN_RIGHT, WALK
	};

private:
	explicit CMantis(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CMantis(const CMantis& rhs);
	virtual ~CMantis() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	void StartScene();			// �����

	void AttackPattern1();		// �μ� ������ ������� 4������
	void AttackPattern2();		// ���߿��� ��, ���������� �ֵθ��� 6, 10�� ���� �̾����� �ϱ�
	void AttackPattern3();		// ���Ƽ� �߻�  5�� ����
	void AttackPattern4();		// �������� 42~39
	void AttackPattern5();		// ũ�� �ѹ� �ֵθ��� 11��
								//void AttackPattern6();	// �޷��ͼ� �����ġ��		// 44

	void NothingAnim();			// �ƹ� ����ȿ�� ����. �׳� ����������
	void MovePattern();			// �÷��̾ �ٶ󺸵��� �ϴ� ���

	void HitPattern1();			// �´� ���� 30��
	void HitPattern2();			// 31��
	void HitPattern3();			// 32��		

	void NotBattleMode1();
	void NotBattleMode2();
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();

private:
	CModel*				m_pModelCom = nullptr;

public:
	static CMantis* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

private:
	_double	m_TimeDelta = 0.0;
	_bool m_bStartScene = false;		// ���� ���ӿ����� false�� �����ؾ� ��
	_bool m_bTime = false;
	_bool m_bStart = false;
	_bool m_bOnce = false;
	_bool m_bFront = false;
	_uint m_iMotion = 0;
	_uint m_iNoBattleMotion = 0;
	_uint m_iHitMotion = 0;
};

END