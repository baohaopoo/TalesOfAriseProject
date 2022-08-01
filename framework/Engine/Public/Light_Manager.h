#pragma once

#include "Base.h"

BEGIN(Engine)

class CLight_Manager final : public CBase
{
	DECLARE_SINGLETON(CLight_Manager)
public:
	CLight_Manager();
	virtual ~CLight_Manager() = default;

public:
	const LIGHTDESC* Get_LightDesc(_uint iIndex);

public:
	HRESULT NativeConstruct(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	HRESULT Add_Lights(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const LIGHTDESC& LightDesc);
	HRESULT Render_Lights();

private:
	list<class CLight*>				m_Lights;
	typedef list<class CLight*>		LIGHTS;

private:
	class CVIBuffer_Rect*			m_pVIBuffer = nullptr;
	class CShader*					m_pShader = nullptr;

	_float4x4			m_WorldMatrix, m_ViewMatrix, m_ProjMatrix;


public:
	virtual void Free() override;
};

END