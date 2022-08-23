#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Texture.h"
#include "VIBuffer_Rect.h"
#include"VIBufferUI.h"

BEGIN(Client)

class CProgressUI abstract : public CGameObject
{
public:
	typedef struct tagUIInfo {
		_float fX;
		_float fY;
		_float fCX;
		_float fCY;
	}UIINFO;


protected:
	CProgressUI(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CProgressUI(const CProgressUI& rhs);
	virtual ~CProgressUI() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;
public:
	UIINFO Get_UIInfo();
protected:
	HRESULT SetUp_Components_Basic();
protected:
	CRenderer* m_pRenderer = nullptr;
	CShader* m_pShader = nullptr;
	CVIBuffer_Rect* m_pVIBuffer = nullptr;
	
	CTexture* m_pTexture = nullptr;
protected:
	UIINFO m_tUIInfo;
	_float4x4 m_ProjMatrix;
	_float m_fWinCX = 0.f, m_fWinCY = 0.f;
	_uint m_iSprite = 0, m_iShaderPass = 0;
public:
	virtual CGameObject* Clone(void* pArg) override = 0;
	virtual void Free() override;
};

END