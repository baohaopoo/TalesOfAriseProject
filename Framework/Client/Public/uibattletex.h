#pragma once
#include "Client_Defines.h"
#include "UI.h"
#include "GameInstance.h"
BEGIN(Client)

class UIBattleTex final : public CUI
{
private:
	UIBattleTex(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	UIBattleTex(const UIBattleTex& rhs);
	virtual ~UIBattleTex() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;


public:
	typedef struct tagUIBTDesc
	{
		UIINFO tUIInfo;
		int kind;

	}UIBATTLETEX;
public:
	void ChangeCut(bool what) { m_isChangeCut = what; }
	void ChangeRender(bool what) { m_isRender = what; }
private:
	_float time = 0;
	//fade in out 용
	_float controlValv = 0;
	float rgb = 0;
	//한번만 fade in out 하도록 하려고 함.
	bool m_isFirst = false;
	bool m_isChangeCut = false;
	bool m_isRender = false;
	bool m_isBattleTexRender = true;
	int UIKind = 0;
private:
	HRESULT SetUp_Texture();
public:
	static UIBattleTex* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END