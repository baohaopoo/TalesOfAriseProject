#pragma once
#include "Client_Defines.h"
#include "Base.h"


BEGIN(Client)

class CPlayer_Manager final : public CBase
{
	DECLARE_SINGLETON(CPlayer_Manager)
private:
	CPlayer_Manager();
	virtual ~CPlayer_Manager() = default;


	//숫자 누르면 합동 공격, 메인캐릭터 변경시 나머지 오토, 일단. 

public:
	void Push_Player(class CPlayer* pPlayer) { m_vecPlayers.push_back(pPlayer); }
	void Set_MainPlayer(_uint iIndex);
	void Set_FieldPlayer(_uint iIndex);
	void Set_Camera(class CCamera_Default* pCamera) { m_pCamera = pCamera; }
	void Set_StopPlayer(_bool bStop);
	class CCamera_Default* Get_Camera() { return m_pCamera; }

	class CPlayer* Get_MainPlayer() { return m_pMainPlayer; }
	class CPlayer* Get_FieldPlayer() { return m_pFieldPlayer; }
	_uint Get_MainPlayerIndex() { return m_iMainPlayerIndex; }
	vector<class CPlayer*>* Get_VecPlayers() { return &m_vecPlayers; }

private:
	vector<class CPlayer*> m_vecPlayers;   //
	class CPlayer* m_pMainPlayer = nullptr;
	class CPlayer* m_pFieldPlayer = nullptr;
	_uint m_iMainPlayerIndex = 0; //일단 주현이꺼랑 합치기전에 쓰자그냥 인덱스로 나중에 겟게임오브젝트로 카메라 들고시작할것.
	class CCamera_Default* m_pCamera = nullptr;

public:
	virtual void Free() override;
};

END