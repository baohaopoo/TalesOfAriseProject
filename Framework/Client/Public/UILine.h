#pragma once
#include "Client_Defines.h"
#include "UI.h"
#include "GameInstance.h"
BEGIN(Client)

class UILine final : public CUI
{
private:
	UILine(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	UILine(const UILine& rhs);
	virtual ~UILine() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;


public:
	typedef struct LINEDESC
	{
		UIINFO uidesc;
		int kind;




	}LINEDESC;
public:
	void ChangeCut(bool what) { m_isChangeCut = what; }

	void BattleOn(bool what) { m_isBattleOn = what; }
private:
	bool m_isBattleOn = false;
private:
	//fade in out 용
	_float controlValv = 0;
	float rgb = 1;
	//한번만 fade in out 하도록 하려고 함.
	bool m_isFirst = false;
	bool m_isChangeCut = false;
	bool m_isRenderOff = false;
	float time = 0;
private:
	HRESULT SetUp_Texture();
public:
	static UILine* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);

	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END