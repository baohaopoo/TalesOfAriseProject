#pragma once
#include "Client_Defines.h"
#include "UI.h"
#include "GameInstance.h"
BEGIN(Client)

class LineSprite final : public CUI
{
private:
	LineSprite(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	LineSprite(const LineSprite& rhs);
	virtual ~LineSprite() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void ChangeCut(bool what) { m_isChangeCut = what; }


public:
	typedef struct MiniMapDesc
	{
		UIINFO uidesc;
		int kind;
		int level;
	}MINIMAPDESC;
private:
	//fade in out 용
	_float controlValv = 0;
	float rgb = 0;
	int level = 0;
	//한번만 fade in out 하도록 하려고 함.
	bool m_isFirst = false;
	bool m_isChangeCut = false;
	bool m_isRender = false;
	int lineangle = 0;
	_float m_Speed = 0;
	_float Turn = 0;
	float time = 0;
private:
	HRESULT SetUp_Texture();
public:
	static LineSprite* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END