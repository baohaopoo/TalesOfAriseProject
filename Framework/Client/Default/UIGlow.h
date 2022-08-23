#pragma once
#include "Client_Defines.h"
#include "UI.h"
#include "GameInstance.h"

BEGIN(Client)

class UIGlow final : public CUI
{
private:
	UIGlow(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	UIGlow(const UIGlow& rhs);
	virtual ~UIGlow() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void ChangeCut(bool what) { m_isChangeCut = what; }
public:
	typedef struct tagUIGlowDesc
	{
		UIINFO tUIInfo;
		int kind;

	}UIGLOWDESC;
private:
	int glowkind = 0;
	//fade in out ��
	_float controlValv = 0;
	float rgb = 0;
	//�ѹ��� fade in out �ϵ��� �Ϸ��� ��.
	bool m_isFirst = false;
	bool m_isChangeCut = false;
	bool m_isRenderOff = false;
private:
	HRESULT SetUp_Texture();
public:
	static UIGlow* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);

	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END