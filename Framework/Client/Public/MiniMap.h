#pragma once
#include "Client_Defines.h"
#include "UI.h"
#include "GameInstance.h"
BEGIN(Client)

class MiniMap final : public CUI
{
private:
	MiniMap(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	MiniMap(const MiniMap& rhs);
	virtual ~MiniMap() = default;
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

	}MINIMAPDESC;
private:
	//fade in out ��
	_float controlValv = 0;
	float rgb = 0;
	//�ѹ��� fade in out �ϵ��� �Ϸ��� ��.
	bool m_isFirst = false;
	bool m_isChangeCut = false;
	bool m_isRender = false;
	float time = 0;
private:
	HRESULT SetUp_Texture();
public:
	static MiniMap* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END