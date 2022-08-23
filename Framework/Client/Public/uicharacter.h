#pragma once
#include "Client_Defines.h"
#include "UI.h"
#include "GameInstance.h"
BEGIN(Client)

class CUICharacter final : public CUI
{
private:
	CUICharacter(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CUICharacter(const CUICharacter& rhs);
	virtual ~CUICharacter() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;
	void RenderOn(bool what) { m_isRender = what; }	

public:
	void ChangeCut(bool what) { m_isChangeCut = what; }

private:
	//fade in out 용
	_float controlValv = 0;
	float rgb = 0;
	//한번만 fade in out 하도록 하려고 함.
	bool m_isFirst = false;
	bool m_isChangeCut = false;
	bool m_isBattleOn = false;
	bool m_isRender = true;
	float cursortime = 0;
public:
	void BattleOn(bool what) { m_isBattleOn = what; }
	typedef struct tagUIExDesc
	{
		UIINFO tUIInfo;
		int kind;

	}UICHDESC;

	int UIKind = 0;
private:
	HRESULT SetUp_Texture();
public:
	static CUICharacter* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END