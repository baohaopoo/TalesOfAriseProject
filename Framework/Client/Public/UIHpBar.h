#pragma once
#include "Client_Defines.h"
#include "progressui.h"
#include "GameInstance.h"
BEGIN(Client)

class UIHpBar final : public CProgressUI
{
public:
	typedef struct tagUIHpBarDesc
	{
		UIINFO tUIInfo;
		int kind;

	}UIHPBARDESC;
private:
	UIHpBar(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	UIHpBar(const UIHpBar& rhs);
	virtual ~UIHpBar() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;
	void BattleOn(bool what) { m_isBattleOn = what; }
private:
	bool m_isBattleOn = false;
	bool redgone = false;
	int cnt = 0;
	float damage = 10;
public:
	void ChangeCut(bool what) { m_isChangeCut = what; }
	void barSize(int size) { m_BarSize = size; }
private:
	//fade in out 용
	_float controlValv = 0;
	float rgb = 1;
	//한번만 fade in out 하도록 하려고 함.
	bool m_isFirst = false;
	bool m_isChangeCut = false;
	bool m_isRenderOff = false;
	int m_BarSize = 0;




	float time = 0;
private:
	HRESULT SetUp_Texture();
public:
	static UIHpBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);

	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END