#pragma once
#include "Client_Defines.h"
#include "Base.h"


BEGIN(Engine)
class CLayer;

END


BEGIN(Client)

class CTime_Manager final : public CBase
{

public:
	enum LAYER_TYPE { LAYER_PLAYER, LAYER_MONSTER, LAYER_MONSTER_EFFECT, LAYER_PLAYER_EFFECT, LAYER_END };


	DECLARE_SINGLETON(CTime_Manager)
private:
	CTime_Manager();
	virtual ~CTime_Manager() = default;




public:
	void Set_LayerTime(LAYER_TYPE eType, _double dSpeed);
	void Set_AllLayerTime(_double dSpeed);
	void Set_LargeHit(_double dSpeed);
	void Set_SmallHit(_double dSpeed);


private:
	//플레이어,몬스터,이펙트,UI

private:
	_bool m_bOnUI = false;
	_bool m_bEvade = false;
	_bool m_bTargeting = false;
	_bool m_bSwitching = false;

	CLayer* pPlayerLayer = nullptr;
	CLayer* pMonsterLayer = nullptr;
	CLayer* pEffectLayer = nullptr;
	CLayer* pUILayer = nullptr;


	_bool m_bSlow = false;
	_double m_dSlowTime = 0.0;

public:
	virtual void Free() override;
};

END