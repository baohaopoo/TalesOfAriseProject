#pragma once
#include "Client_Define.h"
#include "UI.h"
#include "GameInstance.h"
BEGIN(Client)

class CUI_Inven final : public CUI
{
private:
	CUI_Inven(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CUI_Inven(const CUI_Inven& rhs);
	virtual ~CUI_Inven() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

	HRESULT Ready_layer_InvenBtnGlow(const _tchar * pLayerTag);

public:
	void ChangeCut(bool what) { m_isChangeCut = what; }
	void ClickInvenBtn(bool what) { m_isInvenBtn = what; }


private:
	bool m_isFirstload = false;
	//fade in out 용
	_float controlValv = 0;
	float rgb = 0.6;
	bool m_isInvenBtn = false;
	//한번만 fade in out 하도록 하려고 함.
	bool m_isFirst = false;
	bool m_isChangeCut = false;

private:
	HRESULT SetUp_Texture();
public:
	static CUI_Inven* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END