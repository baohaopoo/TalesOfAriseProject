#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Triangle final : public CVIBuffer
{
public:
	CVIBuffer_Triangle(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CVIBuffer_Triangle(const CVIBuffer_Triangle& rhs);
	virtual ~CVIBuffer_Triangle() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual HRESULT Render(class CShader* pShader, _uint iPassIndex);

public:
	HRESULT Update(const _float3* pPoints);
	HRESULT Change_Color(_uint iTriangleIndex, _float4 vColor);



public:
	static CVIBuffer_Triangle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END