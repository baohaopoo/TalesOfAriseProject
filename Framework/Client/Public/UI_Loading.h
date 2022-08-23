#pragma once
#include "Client_Define.h"
#include "UI.h"
#include "GameInstance.h"
BEGIN(Client)

class CUI_Loading final : public CUI
{
private:
	CUI_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CUI_Loading(const CUI_Loading& rhs);
	virtual ~CUI_Loading() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;
private:


	HRESULT SetUp_Texture();

	//fade in out ��
	_float controlValv = 0;
	float rgb = 0;
	//�ѹ��� fade in out �ϵ��� �Ϸ��� ��.
	bool m_isFirst = false;

public:
	static CUI_Loading* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END