#pragma once
#include "Client_Defines.h"
#include "UI.h"
#include "GameInstance.h"
BEGIN(Client)

class CUI_SmashEffect final : public CUI
{
private:
	CUI_SmashEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CUI_SmashEffect(const CUI_SmashEffect& rhs);
	virtual ~CUI_SmashEffect() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;
public:
	void ChangeTex(int Texnum) { texNum = Texnum; }
public:
	void ChangeCut(bool what) { m_isChangeCut = what; }
	void BattleOn(bool what) { m_isBattleOn = what; }

private:
	bool m_isBattleOn = false;
	int texNum = 0;
	float effectTime = 0;
	//fade in out ��
	_float controlValv = 0;
	float rgb = 1.f;
	//�ѹ��� fade in out �ϵ��� �Ϸ��� ��.
	bool m_isFirst = false;
	bool m_isChangeCut = false;					
	

private:
	HRESULT SetUp_Texture();
public:
	static CUI_SmashEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END