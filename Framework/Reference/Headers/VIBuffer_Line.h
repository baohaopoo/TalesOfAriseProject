#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Line final : public CVIBuffer
{
protected:
	CVIBuffer_Line(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CVIBuffer_Line(const CVIBuffer_Line& rhs);
	virtual ~CVIBuffer_Line() = default;
public:
	virtual HRESULT NativeConstruct_Prototype(const _float3* pPoints, _uint iNumPoints, const _float4* pColor);
	virtual HRESULT NativeConstruct(void* pArg);
	virtual HRESULT Render() override;
public:
	static CVIBuffer_Line* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _float3* pPoints, _uint iNumPoints, const _float4* pColor);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END

