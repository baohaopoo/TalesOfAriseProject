#pragma once
#include "Client_Defines.h"
#include "Base.h"


BEGIN(Engine)
class CLayer;

END


BEGIN(Client)

class CPlayer;

class CBattle_Manager final : public CBase
{

public:
	enum PLAYER_TYPE { PLAYER_ALPHEN, PLAYER_SHIONNE, PLAYER_KISARA, PLAYER_RINWELL, PLAYER_END };

	typedef struct tagDamageINFO
	{
		unsigned int iDamage = 0;   //데미지  혹은공격력
		DAMAGE_TYPE e_DamageType = DAMAGE_TYPE_END;  //데미지종류
		DAMAGE_DIR e_DamageDir = DAMAGE_DIR_END;      //데미지방향
		float fPowerUp = 0.f;
		float fPowerBack = 0.f;
		double dCollisionCoolTime = 0.0;
		PLAYER_TYPE e_PlayerType = PLAYER_END;
	}DAMAGEINFO;

	DECLARE_SINGLETON(CBattle_Manager)

private:
	CBattle_Manager();
	virtual ~CBattle_Manager() = default;

public:
	HRESULT NativeConstruct();
public:
	void SetCP(float newcp) { CurrentCp = newcp; }
	float GetCP() { return CurrentCp; }
	float GetMAXCP() { return MaxCp; }
private:
	float CurrentCp = 100.f;
	float MaxCp = 50;
	float ComboHit = 0;
public:
	void Battle_Enter(class CEnemy* pEnemy);
	void Battle_End();
	void Tick(_double TimeDelta);
	void Set_CameraShake(DAMAGE_TYPE eDamageType);
	_int Get_Cp() { return m_iCp; }
	_bool Get_Battle() { return m_bBattle; }

	vector<class CEnemy*>* Get_VecMonster() { return &m_vecMonsters; }



private:
	HRESULT Ready_Map(const char * pModelFilePath, const char * pModelFileName);
	HRESULT Enemy_MoveToMap(CEnemy* pEnemy, _float3 fPos, const TCHAR * pTagNaviCompoent);		// 1인자 오브젝트(Enemy)에게 3인자의 네비를 생성하고, 2인자의 위치로 이동하여 y값을 설정한다.
	HRESULT Player_MoveToMap(CPlayer* pPlayer, _float3 fPos, const TCHAR * pTagNaviCompoent);	// 1인자 오브젝트(Player)에게 3인자의 네비를 생성하고, 2인자의 위치로 이동하여 y값을 설정한다.



private:
	vector<class CMapObject*> m_vMapObject;



private:
	vector<class CEnemy*> m_vecMonsters;  //플레이어한테 넘겨줘야할 값 
	vector<class CPlayer*>* m_pVecPlayers;

	class CCamera_Default* m_pCamera = nullptr;

	CLayer* m_pMonsterLayer = nullptr;
	CLayer* m_pPlayerLayer = nullptr;
	CLayer* m_pPlayerBulletLayer = nullptr;
	_float4 m_vPrePlayerPos;

	_bool m_bBattle = false;

	_int m_iCp = 0;

public:
	virtual void Free() override;
};

END