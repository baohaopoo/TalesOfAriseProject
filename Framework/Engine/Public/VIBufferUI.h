#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBufferUI final : public CVIBuffer
{
protected:
	CVIBufferUI(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CVIBufferUI(const CVIBufferUI& rhs);
	virtual ~CVIBufferUI() = default;
public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
public:
	static CVIBufferUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END

