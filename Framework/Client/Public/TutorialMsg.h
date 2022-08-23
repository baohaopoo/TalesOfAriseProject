#pragma once
#include "Client_Defines.h"
#include "UI.h"
#include "GameInstance.h"
BEGIN(Client)

class TutorialMsg final : public CUI
{
private:
	TutorialMsg(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	TutorialMsg(const TutorialMsg& rhs);
	virtual ~TutorialMsg() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void ChangeCut(bool what) { m_isChangeCut = what; }


public:
	typedef struct TutorialMsgDesc
	{
		UIINFO uidesc;
		int kind;

	}TMSGDESC;
private:
	//fade in out 용
	_float controlValv = 0;
	float rgb = 0;
	//한번만 fade in out 하도록 하려고 함.
	bool m_isFirst = false;
	bool m_isChangeCut = false;
	bool m_isRender = false;
	float time = 0;
	bool changeSprite = false;
	
private:
	HRESULT SetUp_Texture();
public:
	static TutorialMsg* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END