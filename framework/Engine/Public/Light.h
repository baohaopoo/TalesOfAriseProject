#pragma once

#include "Base.h"

BEGIN(Engine)

class CLight final : public CBase
{
private:
	CLight(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CLight() = default; 

public:
	const LIGHTDESC* Get_LightDesc() {
		return &m_LightDesc;
	}

public:
	HRESULT NativeConstruct(const LIGHTDESC& LightDesc);
	HRESULT Render(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);

private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;
	LIGHTDESC				m_LightDesc;

public:
	static CLight* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const LIGHTDESC& LightDesc);
	virtual void Free() override;
};

END