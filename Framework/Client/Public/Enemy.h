#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Battle_Manager.h"
#include "Player_Manager.h"

BEGIN(Engine)
class CShader;
class CCollider;
class CRenderer;
class CModel;
class CNavigation;
END

BEGIN(Client)

class CEnemy abstract : public CGameObject
{
public:
	typedef struct _tagEnemyDesc {
		_vector vPos;
		MAP_TYPE eMapType = MAP_END;
	}EnemyDesc;

protected:
	CEnemy(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	CEnemy(const CEnemy& rhs);
	virtual ~CEnemy() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg, CTransform::TRANSFORMDESC* pTransformDesc);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;


public:
	_int Get_Hp() { return m_iEnemyInfo.m_iHp; }
	void Set_Battle(_bool bBattle) { m_bBattle = bBattle; }
	_vector Get_EnemyPos() { return m_pTransformCom->Get_State(CTransform::STATE_POSITION); }
	_vector Get_EnemyLook() { return m_pTransformCom->Get_State(CTransform::STATE_LOOK); }
	void	Set_EnemyPos(_fvector vPos) { m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos); }
	void Set_Damage(_int iDamage) { m_iEnemyInfo.m_iHp -= iDamage; }




	void Set_CollisionDistance(_float fDistance) { m_fCollisionDistance = fDistance; }

	void Set_Collision(_bool bCollision) { m_bAttackCollision = bCollision; }
	void Set_KnockBackDir(_fvector vDir) { XMStoreFloat3(&m_vKnockBackDir, vDir); }

	//����׿� ���߿� ����� 
	void Set_Hits() { m_iHits++; }
	_uint Get_Hits() { return m_iHits; }


	void Set_NewCollisionAlphen(_bool bNewCollision) { m_bNewCollisionAlphen = bNewCollision; }
	void Set_NewCollisionShionne(_bool bNewCollision) { m_bNewCollisionShionne = bNewCollision; }
	void Set_NewCollisionKisara(_bool bNewCollision) { m_bNewCollisionKisara = bNewCollision; }
	void Set_NewCollisionRinwell(_bool bNewCollision) { m_bNewCollisionRinwell = bNewCollision; }

	_bool Get_NewCollisionAlphen() { return m_bNewCollisionAlphen; }
	_bool Get_NewCollisionShionne() { return  m_bNewCollisionShionne; }
	_bool Get_NewCollisionKisara() { return m_bNewCollisionKisara; }
	_bool Get_NewCollisionRinwell() { return  m_bNewCollisionRinwell; }


	void Set_PowerUp(_float fPower) { m_fPowerUp = fPower; }
	void Set_PowerKnocKBack(_float fPower) { m_fPowerKnockBack = fPower; }

	_bool Get_Invincible() { return m_bInvincible; }

	void Set_HitDamageInfo(CBattle_Manager::DAMAGEINFO DamageInfo) { m_HitDamageInfo = DamageInfo; }
	CBattle_Manager::DAMAGEINFO Get_AttackDamageInfo() { return m_AttackDamageInfo; }


protected:
	void Compute_Gravity(_double TimeDelta);
	void Compute_Collider();


protected:
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CCollider*			m_pOBBCom = nullptr;
	CCollider*			m_pSphereCom = nullptr;
	CCollider*			m_pAttackSphereCom = nullptr;
	CNavigation*		m_pNaviCom = nullptr;

public:
	CNavigation* SetUp_Navigation(const TCHAR* pTagNaviCompoent);	// Ÿ�� ������ Navi_Component�� �����ϴ� �Լ�
	_bool Delete_NaviCom(void);		// �׺� ������Ʈ�� �����ϴ� �Լ�

public:
	virtual CGameObject* Clone(void* pArg) override = 0;
	virtual void Free() override;

	_bool Get_IsBoss() { return m_isBoss; }
	void Set_StartScene() { m_bStartScene = true; }

protected:
	CBattle_Manager* m_pBattle_Manager = nullptr;
	CPlayer_Manager* m_pPlayer_Manager = nullptr;

	CBattle_Manager::DAMAGEINFO m_HitDamageInfo;
	CBattle_Manager::DAMAGEINFO m_AttackDamageInfo;


	//����׿� �������
	_uint m_iHits = 0;


	//���������
	_float m_fPowerUp = 0.f;
	_float m_fPowerKnockBack = 0.f;


	//�浹����
	_bool m_bInvincible = false;
	_bool m_bNewCollisionAlphen = false;
	_bool m_bNewCollisionShionne = false;
	_bool m_bNewCollisionKisara = false;
	_bool m_bNewCollisionRinwell = false;
	_bool m_bAfterColAnim = false;			// �°� �ִ� �ִϸ��̼� ������ ���� �� ������ �ִϸ��̼� �ȳ����� �ʹݿ� ���� �����ٰ� ���Ƽ� �´� �ִϸ��̼� ������ ���� ���� �ִϸ��̼� �ϰԲ� ����� ����
	_bool m_bOnAttackCollider = false;
	_bool m_bBattle = false;			// ��Ʋ���� �ƴ� ���͵��� �ʿ��� �÷��̾ ��ٸ��� �ִٸ� false, �÷��̾�� �������̶�� true
	_bool m_bCutAnimation = true;		// true�̸� ��� �����̰� false�̸� �����
	_bool m_bHit = false;				// �÷��̾�� ���ݴ����� ���
	_uint m_iHitCount = 0;				// �÷��̾�� ���ݴ��� Ƚ���� Ư�� Ƚ���� �Ǹ� �÷��̾ �����ϴ��� ���͵� break���°� �ƴ� ������ �Ѵ�
	_double m_dAnimSpeed = 2.0;
	_double m_iDuration = 5.0;		// �������� Term��
	_uint m_iCurrentAnimationIndex = 0;
	_uint m_iNextAnimationIndex = 0;
	_float m_fCollisionDistance = 0.f;
	_bool m_bAttackCollision = false;
	_bool m_bDeadOnce = false;
	_double m_dDownTime = 0.0;
	_float3 m_vKnockBackDir;
	_double m_dDeathTime = 0.0;
	_double m_dColTime = 0.0;		// ���Ͱ� ��� �±⸸ �ϸ� ������ �� �� ���⿡ �ݰ��� ��ȸ�� �ֱ����� ���� �ð�����
	_bool m_bAttackRevenge = false;
	typedef struct tagEnemyInfo
	{
		_int m_iHp = 0;					// ���� ü��
		_uint m_iBreakCount = 0;			// �÷��̾�� ���� Ƚ���� �����ð� ���� 20�� �̻��̶�� m_bHit�� true�� ��ȯ�ȴ�
	}ENEMYINFO;

	ENEMYINFO m_iEnemyInfo;

	_bool m_isBoss = false;
	_bool m_bStartScene = false;



public:
	MAP_TYPE Get_MapType(void) { return m_eMapType; }

private:
	MAP_TYPE m_eMapType = MAP_END;
};

END