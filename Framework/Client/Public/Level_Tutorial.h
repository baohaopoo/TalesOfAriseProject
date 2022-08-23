#pragma once
#include "Client_Defines.h"
#include "Level.h"
#include "Battle_Manager.h"

BEGIN(Client)

class CMapObject;
class CWayPoint;
class CPlayer;

END

BEGIN(Client)

class CLevel_Tutorial final : public CLevel
{
private:
	explicit CLevel_Tutorial(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual ~CLevel_Tutorial() = default;

public:
	virtual HRESULT NativeConstruct() override;
	virtual void Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Lights();
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Balseph(const _tchar * pLayerTag);
	HRESULT Ready_Layer_Boar(const _tchar * pLayerTag);
	HRESULT Ready_Layer_Practice(const _tchar * pLayerTag);
	HRESULT Ready_Layer_Punisher(const _tchar * pLayerTag);
	HRESULT Ready_Layer_FireAvatar(const _tchar * pLayerTag);
	HRESULT Ready_Layer_Mantis(const _tchar * pLayerTag);


	//UI
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Effect(const _tchar* pLayerTag);
	//UI모듬전
	HRESULT Ready_Layer_Tutorial_UI();
	HRESULT Ready_Layer_Battle_UI();
	HRESULT Ready_Layer_Inventory_UI();
	HRESULT Ready_Layer_Inventory_Itme_UI();

	HRESULT Ready_Layer_UI();
	HRESULT Ready_Layer_EmblemUI(const _tchar* pLayerTag);
	HRESULT Ready_Layer_ExUI(const _tchar* pLayerTag);
	HRESULT Ready_Layer_SmashUI();
	HRESULT Ready_Layer_SmashEffect(const _tchar * pLayerTag);
	HRESULT Ready_Layer_Line(const _tchar* pLayerTag);
	HRESULT Ready_Layer_NumberFont(const _tchar* pLayerTag);
	HRESULT Ready_Layer_DamageFont(const _tchar* pLayerTag);
	HRESULT Ready_Layer_HPFont(const _tchar* pLayerTag);
	HRESULT Ready_Layer_StatusFont(const _tchar* pLayerTag);
	HRESULT Ready_Layer_StatusFont2(const _tchar* pLayerTag);
	HRESULT Ready_Layer_NameUI(const _tchar* pLayerTag);
	HRESULT Ready_Layer_StatusBallUI(const _tchar* pLayerTag);
	HRESULT Ready_Layer_StatusUI(const _tchar* pLayerTag);
	HRESULT Ready_Layer_SmashNum(const _tchar* pLayerTag);
	HRESULT Ready_Layer_AlphaTexture(const _tchar* pLayerTag); //char talk
	HRESULT Ready_Layer_AutoTargetUI(const _tchar* pLayerTag);

	HRESULT Ready_Layer_AllAboutHp(const _tchar* pLayerTag);
	HRESULT Ready_Layer_MonsterHp(const _tchar* pLayerTag);
	HRESULT Ready_Layer_BarUI(const _tchar* pLayerTag);
	HRESULT Ready_Layer_InfoFadeUI(const _tchar* pLayerTag);
	HRESULT Ready_Layer_LineMsgUI(const _tchar* pLayerTag);
	HRESULT Ready_Fonts();
private:
	HRESULT Ready_Map(const char * pModelFilePath, const char * pModelFileName, _vector vCenterPos);
	HRESULT Ready_Map_Battle02(_vector vCenterPos);		// CenterPos를 기준으로 Battle02의 오브젝트들을 생성
	HRESULT Ready_Map_Battle04(_vector vCenterPos);		// CenterPos를 기준으로 Battle04의 오브젝트들을 생성

private:
	_float4x4  matBattle02World;		// Prototype_Component_Model_Map_GrandYork_Battle_02
	_float4x4  matBattle04World;		// Prototype_Component_Model_Map_GrandYork_Battle_04

private:
	vector<CMapObject*>	m_vMapObject;	// 배치된 맵 오브젝트 vector
	vector<CWayPoint*>	m_vWayPoints;	// 배치된 맵 오브젝트 vector

public:
	CBattle_Manager* m_pBattle_Manager = nullptr;
	typedef struct tagUIInfo {
		_float fX;
		_float fY;
		_float fCX;
		_float fCY;

	}UIINFO;

private:
	CPlayer* m_pPlayerAlphen = nullptr;

private:
	_tchar HitMsg[MAX_PATH] = TEXT("");
	_tchar DamageMsg[MAX_PATH] = TEXT("");
	_tchar StatusLine[MAX_PATH] = TEXT("");
	_tchar ComboHit[MAX_PATH] = TEXT("");
	_tchar DamageHit[MAX_PATH] = TEXT("");
	int combohitNum = 0;
	int damageNum = 0;
	bool m_isComboHit = false;
	bool m_isInvenOpen = false;
	bool m_isFirst = true;
	_float smashuiTime = 0;
	int next = 0;
	int LineMsgSprite = 1;
	bool m_isBattle = false;
	bool LineMsgCreate = false;
	bool m_isBattleOn = false;
	float fontpos = 0;
	bool m_isHit = false;
	float HitPos, DamagePos;
	bool m_bFirstBattleUI = false;
public:
	static CLevel_Tutorial* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual void Free() override;
};

END