#pragma once
#include "Client_Defines.h"
#include "UI.h"
#include "GameInstance.h"
BEGIN(Client)

class TutoLineMsg final : public CUI
{
private:
	TutoLineMsg(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	TutoLineMsg(const TutoLineMsg& rhs);
	virtual ~TutoLineMsg() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void ChangeCut(bool what) { m_isChangeCut = what; }

public:
	typedef struct tagUILineMsgUI
	{
		UIINFO uidesc;
		int sprite;
	}MSGLINEUIDESC;

private:
	
	//fade in out ��
	int acc =3;
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
	static TutoLineMsg* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END