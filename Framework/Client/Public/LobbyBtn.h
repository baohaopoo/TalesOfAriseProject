#pragma once
#include "Client_Defines.h"
#include "GameInstance.h"
#include "Button_Super.h"

BEGIN(Client)
class LobbyBtn final : public CButton_Super
{
public:
	LobbyBtn(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	LobbyBtn(const LobbyBtn& rhs);
	virtual ~LobbyBtn() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;



public:
	void Set_Kind(int num) { kind = num; }
	void ChangeTouch();
	void RenderFunc(bool what) { m_isRender = what; }
private:
	bool m_isRender = false;
	HRESULT SetUp_Texture();
	bool m_isTouched = false;
	float rgb = 1;
	int kind = 0;
public:

	static LobbyBtn* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END
