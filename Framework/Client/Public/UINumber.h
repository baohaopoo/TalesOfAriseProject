#pragma once
#include "Client_Defines.h"
#include "UI.h"
#include "GameInstance.h"
BEGIN(Client)

class CUINumber final : public CUI
{
private:
	CUINumber(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CUINumber(const CUINumber& rhs);
	virtual ~CUINumber() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void BattleOn(bool what) { m_isBattleOn = what; }
	void ChangeCut(bool what) { m_isChangeCut = what; }
public:
	typedef struct tagUINumDesc
	{
		UIINFO tUIInfo;
		int kind;

	}UINUMDESC;
private:

	bool m_isBattleOn = false;
	//fade in out ��
	_float controlValv = 0;
	float rgb = 0;
	int numKind = 0;
	//�ѹ��� fade in out �ϵ��� �Ϸ��� ��.
	bool m_isFirst = false;
	bool m_isChangeCut = false;
	bool m_isGlowFirst = true;
private:
	HRESULT SetUp_Texture();
public:
	static CUINumber* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END