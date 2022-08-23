#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Texture.h"
#include "VIBuffer_Rect.h"
#include"VIBufferUI.h"

BEGIN(Client)

class CUI abstract : public CGameObject
{
public:
	typedef struct tagUIInfo {
		_float fX;
		_float fY;
		_float fCX;
		_float fCY;
	}UIINFO;

protected:
	CUI(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CUI(const CUI& rhs);
	virtual ~CUI() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;
public:
	UIINFO Get_UIInfo();
	void RenderChange(bool what) { m_isRender = what; }

protected:
	HRESULT SetUp_Components_Basic();		// Renderer, Shader, ViBuffer만 여기에 넣고 Texture는 하위 클래스들에서 선언해준다


protected:
	CTexture*		m_pTextureCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;
	CShader*		m_pShaderCom = nullptr;
	CVIBuffer_Rect* m_pVIBufferCom = nullptr;
	CVIBufferUI* m_pVIBufferUICom = nullptr;

protected:
	UIINFO m_tUIInfo;
	_float4x4 m_ProjMatrix;
	_float m_fWinCX = 0.f, m_fWinCY = 0.f;
	_uint m_iSprite = 0, m_iShaderPass = 0;
	bool m_isRender = true;
	//int m_iShaderPass = 15;
	//int rgbda =0;
public:
	virtual CGameObject* Clone(void* pArg) override = 0;
	virtual void Free() override;
};

END