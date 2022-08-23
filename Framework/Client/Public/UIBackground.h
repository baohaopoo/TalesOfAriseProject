#pragma once
#include "Client_Defines.h"
#include "UI.h"


BEGIN(Client)

class CUIBackground final : public CUI
{
private:
	CUIBackground(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CUIBackground(const CUIBackground& rhs);
	virtual ~CUIBackground() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;
public:
	typedef struct tagBackUiDesc
	{
		UIINFO tUIInfo;
		int kind;

	}UIBACKDESC;

public:
	void ChangeCut(bool what) { m_isChangeCut = what; }

private:
	int kind = 0;
	//fade in out 용
	_float controlValv = 0;
	float rgb = 0;
	//한번만 fade in out 하도록 하려고 함.
	bool m_isFirst = false;
	bool m_isChangeCut = false;

private:
	HRESULT SetUp_Texture();
public:
	static CUIBackground* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);

	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END