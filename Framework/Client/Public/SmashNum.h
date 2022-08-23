#pragma once
#include "Client_Defines.h"
#include "Button_Super.h"
#include "GameInstance.h"

BEGIN(Client)
class SmashNum final : public CButton_Super
{
public:
	SmashNum(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	SmashNum(const SmashNum& rhs);
	virtual ~SmashNum() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;


public:
	typedef struct tagSmashNumDesc
	{
		UIINFO tUIInfo;
		int kind;

	}UISMNUMDESC;
public:
	void ChangeTouch(bool what) { m_isTouched = what; }
public:
	int kind = 0;
	void RenderOn(bool what) { m_isRender = what; };
	void BattleOn(bool what) { m_isBattleOn = what; }
private:
	bool m_isBattleOn = false;
	bool m_isRender = true;
	HRESULT SetUp_Texture();
	bool m_isTouched = false;
	float rgb = 0.f;
	float charTalk1Time = 0;
	float charTalkTime2 = 1;
public:

	static SmashNum* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END
