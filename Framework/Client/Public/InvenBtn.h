#pragma once
#include "Client_Define.h"
#include "Button_Super.h"
#include "GameInstance.h"

BEGIN(Client)
class InvenBtn final : public CButton_Super
{
public:
	InvenBtn(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	InvenBtn(const InvenBtn& rhs);
	virtual ~InvenBtn() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;
public:
	void manageSize(bool what) { m_iSizeChange = what; }
	void smaller();
	void bigger();
private:

	bool m_isBtnSmaller = false;
	bool m_iSizeChange = false;
	int size = 0;
public:
	typedef struct tagUIInvenBtn
	{
		UIINFO tUIInfo;
		int texnum;

	}UIINVENBTNDESC;
			
private:
	float rgb = 0.7;
	int tex = 0;
public:
	void ChangeTouch(bool what) { m_isTouched = what; }
private:
	HRESULT SetUp_Texture();
	bool m_isTouched = false;
public:

	static InvenBtn* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END
