#pragma once
#include "Client_Defines.h"
#include "UI.h"
#include "GameInstance.h"
BEGIN(Client)

class CUI_Smash final : public CUI
{
private:
	CUI_Smash(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CUI_Smash(const CUI_Smash& rhs);
	virtual ~CUI_Smash() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;
public:
	void ChangeTex(int Texnum) { texNum = Texnum; }
public:
	void ChangeCut(bool what) { m_isChangeCut = what; }


private:
	float time = 0; // effect 생성타임
	int texNum = 0;
	float x, y;
	//fade in out 용
	_float controlValv = 0;
	float rgb = 0;
	//한번만 fade in out 하도록 하려고 함.
	bool m_isFirst = false;
	bool m_isChangeCut = false;
	int spirtekind;

	bool m_isBattleOn = false;
public:
	void BattleOn(bool what) { m_isBattleOn = what; }
	typedef struct tagUISmashDesc
	{
		UIINFO tUIInfo;
		int kind;

	}UISMDESC;
	
	
private:
	HRESULT SetUp_Texture();
public:
	static CUI_Smash* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END