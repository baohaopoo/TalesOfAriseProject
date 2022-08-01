#pragma once

#include "Base.h"

BEGIN(Engine)

class CFrustum final : public CBase
{
	DECLARE_SINGLETON(CFrustum)
public:
	CFrustum();
	virtual ~CFrustum() = default;

public: 
	HRESULT NativeConstruct();
	HRESULT Transform_ToWorldSpace();
	HRESULT Transform_ToLocalSpace(_fmatrix WorldMatrix);
public:
	_bool isIn_WorldSpace(_vector vWorldPos, _float fRange);
	_bool isIn_LocalSpace(_vector vLocalPos, _float fRange);

private:
	_float3			m_vPoints[8];
	_float3			m_vPointInWorld[8];
	_float4			m_vPlaneInWorld[6];
	_float4			m_vPlaneInLocal[6];

public:
	virtual void Free() override;
};

END