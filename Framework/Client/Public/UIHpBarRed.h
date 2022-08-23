#pragma once
#include "Client_Defines.h"
#include "progressui.h"

#include "GameInstance.h"
#include<random>
BEGIN(Client)

class UIHpBarRed final : public CProgressUI
{
public:
	typedef struct tagUIHpBarDesc
	{
		UIINFO tUIInfo;
		int kind;

	}UIHPBARDESC;
private:
	UIHpBarRed(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	UIHpBarRed(const UIHpBarRed& rhs);
	virtual ~UIHpBarRed() = default;
private:
	uniform_int_distribution<>uid{ 0,12 };
	default_random_engine dre;


public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;
	void BattleOn(bool what) { m_isBattleOn = what; }
private:
	bool m_isBattleOn = false;
public:
	void ChangeCut(bool what) { m_isChangeCut = what; }
	void barSize(int size) { m_BarSize = size; }
private:
	//fade in out 용
	float HpGreen = 0;
	float damage = 0;
	float traildamage = 0;
	_float controlValv = 0;
	float rgb = 1;
	//한번만 fade in out 하도록 하려고 함.
	bool m_isFirst = false;
	bool m_isChangeCut = false;
	bool m_isRenderOff = false;
	int m_BarSize = 0;
	bool m_isRedgone = false;
	float m_isRendTime = 0;


	float time = 0;
private:
	HRESULT SetUp_Texture();
public:
	static UIHpBarRed* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);

	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END