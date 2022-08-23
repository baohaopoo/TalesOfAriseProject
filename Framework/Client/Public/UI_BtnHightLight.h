#pragma once
#include "Client_Define.h"
#include "UI.h"
#include "GameInstance.h"
BEGIN(Client)

class CUI_BtnHightLight final : public CUI
{
private:
	CUI_BtnHightLight(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CUI_BtnHightLight(const CUI_BtnHightLight& rhs);
	virtual ~CUI_BtnHightLight() = default;
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
	void RenderFunc(bool what) { m_isRender = what; }
private:
	bool m_isRender = false;
	bool m_isBattleOn = false;
	int texNum = 0;
	float effectTime = 0;
	//fade in out ��
	_float controlValv = 0;
	float rgb = 0.05f;
	//�ѹ��� fade in out �ϵ��� �Ϸ��� ��.
	bool m_isFirst = false;
	bool m_isChangeCut = false;					
	
	int UIkind = 0;

public:
public:
	typedef struct tagHIGHTDesc
	{
		UIINFO tUIInfo;
		int kind;

	}UIHIGHTDESC;
private:
	HRESULT SetUp_Texture();
public:
	static CUI_BtnHightLight* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END