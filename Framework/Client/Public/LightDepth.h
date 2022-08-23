#pragma once
#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CLightDepth final : public CCamera
{
private:
	CLightDepth(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CLightDepth(const CLightDepth& rhs);
	virtual ~CLightDepth() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta);
	virtual void LateTick(_double TimeDelta);
	virtual HRESULT Render();
public:
	static CLightDepth* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END