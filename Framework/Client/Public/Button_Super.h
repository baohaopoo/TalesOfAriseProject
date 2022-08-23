#pragma once
#include "Client_Defines.h"
#include "UI.h"
#include "GameInstance.h"

BEGIN(Client)

class CButton_Super abstract : public CUI
{
protected:
	CButton_Super(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CButton_Super(const CButton_Super& rhs);
	virtual ~CButton_Super() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;
protected:
	void Compute_Rect();
protected:
	RECT m_tRect;
public:
	virtual CGameObject* Clone(void* pArg) override = 0;
	virtual void Free() override;
};

END