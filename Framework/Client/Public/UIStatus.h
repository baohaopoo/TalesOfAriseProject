#pragma once
#include "Client_Defines.h"
#include "UI.h"

#include "GameInstance.h"
BEGIN(Client)

class CUIStatus final : public CUI
{
private:
	CUIStatus(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CUIStatus(const CUIStatus& rhs);
	virtual ~CUIStatus() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	typedef struct STATUSUIDESC {

		UIINFO uidesc;
		int kind;
	}STATUSDESC;

private:

	float m_fPercentage = 0.7;
	int maxcp = 0;
	bool m_isRed = false;
	bool m_isBattleOn = false;
	int cp = 0;
	_tchar HitEffect[MAX_PATH] = TEXT("");
	_tchar HitCurrent[MAX_PATH] = TEXT("");
public:
	void BattleOn(bool what) { m_isBattleOn = what; }
private:
	HRESULT SetUp_Texture();
public:
	static CUIStatus* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);

	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END