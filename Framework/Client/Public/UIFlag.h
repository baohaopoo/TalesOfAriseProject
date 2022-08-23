#pragma once
#include "Client_Defines.h"
#include "UI.h"
#include "GameInstance.h"
BEGIN(Client)

class UIFlag final : public CUI
{
private:
	UIFlag(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	UIFlag(const UIFlag& rhs);
	virtual ~UIFlag() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;


public:
	typedef struct tagUIFlagDesc
	{
		UIINFO tUIInfo;
		int kind;

	}UIFLAGDESC;
public:
	void ChangeFlag(int nkind) { kind = nkind; }
private:

	int kind = 0;
private:
	HRESULT SetUp_Texture();
public:
	static UIFlag* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END