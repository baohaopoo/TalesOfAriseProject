#pragma once
#include "Client_Defines.h"
#include "Base.h"

#include"GameObject.h"
BEGIN(Engine)
class CLayer;

END

BEGIN(Client)


class CUI_Manager final : public CBase
{
	DECLARE_SINGLETON(CUI_Manager)

	enum LEVELUI {LEVEL_TUTORIAL, LEVEL_INVEN_LOBBY, LEVEL_INVEN_ITEM, LEVEL_INVEN_ARMY, LEVEL_BOSS1, LEVEL_BOSS2, LEVEL_END};
	enum UIPLAYER {PLAYER_ALPHEN, PLAYER_SION, PLAYER_RINWELL, PLAYRE_KAKURA, PLAYER_END};
	enum UIKIND {ACQUIRE, KIND_END};
private:
	CUI_Manager();
	virtual ~CUI_Manager() = default;



private:
	class CPlayer* pPlayer[4];

public:
	void SetPlayer(int index, CPlayer* newPlayer) { pPlayer[index] = newPlayer; }
	class CPlayer* GetPlayer(int index) { return pPlayer[index]; }

public:
	virtual void Free() override;
};

END