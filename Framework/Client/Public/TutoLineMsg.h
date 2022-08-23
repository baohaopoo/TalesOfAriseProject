#pragma once
#include "Client_Defines.h"
#include "UI.h"
#include "GameInstance.h"
BEGIN(Client)

class TutoLineMsg final : public CUI
{
private:
	TutoLineMsg(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	TutoLineMsg(const TutoLineMsg& rhs);
	virtual ~TutoLineMsg() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void ChangeCut(bool what) { m_isChangeCut = what; }

public:
	typedef struct tagUILineMsgUI
	{
		UIINFO uidesc;
		int sprite;
	}MSGLINEUIDESC;

private:
	
	//fade in out 용
	int acc =3;
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
	static TutoLineMsg* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END