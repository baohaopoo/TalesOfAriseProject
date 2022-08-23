#pragma once
#include "Client_Defines.h"
#include "UI.h"
#include "GameInstance.h"
BEGIN(Client)

class CUIStatusBall final : public CUI
{
private:
	CUIStatusBall(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CUIStatusBall(const CUIStatusBall& rhs);
	virtual ~CUIStatusBall() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;



private:


	bool m_isBattleOn = false;

public:
	void BattleOn(bool what) { m_isBattleOn = what; }
private:
	HRESULT SetUp_Texture();
public:
	static CUIStatusBall* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);

	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END