#pragma once
#include "Client_Define.h"
#include "UI.h"

BEGIN(Client)

class CFadeInOut final : public CUI
{
public:
	CFadeInOut(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CFadeInOut(const CFadeInOut& rhs);
	virtual ~CFadeInOut()= default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;
	void ChangeEnterrgb(bool changergb) { m_isEnterRgb = changergb; }
	void Changebyergb(bool changergb) { m_isByeRgb = changergb; }

private:
	HRESULT SetUp_Texture();
	bool m_isEnterRgb = false;
	bool m_isByeRgb = false;
private:
	_float controlValv = 0;
	float rgb = 0;
public:
	static CFadeInOut* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END