#pragma once
#include "Client_Defines.h"
#include "UI.h"
#include "GameInstance.h"
BEGIN(Client)

class UILine final : public CUI
{
private:
	UILine(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	UILine(const UILine& rhs);
	virtual ~UILine() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;


public:
	typedef struct LINEDESC
	{
		UIINFO uidesc;
		int kind;




	}LINEDESC;
public:
	void ChangeCut(bool what) { m_isChangeCut = what; }

	void BattleOn(bool what) { m_isBattleOn = what; }
private:
	bool m_isBattleOn = false;
private:
	//fade in out ��
	_float controlValv = 0;
	float rgb = 1;
	//�ѹ��� fade in out �ϵ��� �Ϸ��� ��.
	bool m_isFirst = false;
	bool m_isChangeCut = false;
	bool m_isRenderOff = false;
	float time = 0;
private:
	HRESULT SetUp_Texture();
public:
	static UILine* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);

	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END