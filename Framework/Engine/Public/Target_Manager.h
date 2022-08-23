#pragma once
#include "Base.h"

BEGIN(Engine)

class CTarget_Manager final : public CBase
{
	DECLARE_SINGLETON(CTarget_Manager)
private:
	CTarget_Manager();
	virtual ~CTarget_Manager() = default;

public:
	ID3D11ShaderResourceView* Get_SRV(const _tchar* pTargetTag) const;

public:
	HRESULT Add_RenderTarget(const _tchar* pRenderTargetTag, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vColor);
	HRESULT Add_MRT(const _tchar* pMRTTag, const _tchar* pRenderTargetTag);

	HRESULT Create_DSV(ID3D11Device* pDevice, _uint iCX, _uint iCY);

	HRESULT Begin_MRT(ID3D11DeviceContext* pDeviceContext, const _tchar* pMRTTag);
	HRESULT End_MRT(ID3D11DeviceContext* pDeviceContext, const _tchar* pMRTTag);

	HRESULT Begin_LightDepth_MRT(ID3D11DeviceContext* pDeviceContext, const _tchar* pMRTTag);

	HRESULT Begin_NonClear_MRT(ID3D11DeviceContext* pDeviceContext, const _tchar* pMRTTag);

	HRESULT Clear_MRT(ID3D11DeviceContext* pDeviceContext, const _tchar* pMRTTag);
#ifdef _DEBUG
public:
	HRESULT Ready_DebugBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	HRESULT Ready_DebugDesc(const _tchar* pRenderTargetTag, _float fTopX, _float fTopY, _float fWidth, _float fHeight);
	HRESULT Render_Debug(const _tchar* pMRTTag);
#endif // _DEBUG

private:
	map<const _tchar*, class CRenderTarget*>				m_RenderTargets;
	typedef map<const _tchar*, class CRenderTarget*>		RENDERTARGETS;

private:
	map<const _tchar*, list<class CRenderTarget*>>				m_MRTs;
	typedef map<const _tchar*, list<class CRenderTarget*>>		MRTS;

private:
	ID3D11RenderTargetView*				m_pBackBufferRTV = nullptr;
	ID3D11DepthStencilView*				m_pDepthStencilView = nullptr;
	D3D11_VIEWPORT						m_tOrigin_ViewPort;

	ID3D11DepthStencilView*				m_pLightDepth_DSV = nullptr;
	D3D11_VIEWPORT						m_tLightDepth_ViewPort;
#ifdef _DEBUG
private:
	class CShader*						m_pShader = nullptr;
	class CVIBuffer_Rect*				m_pDebugBuffer = nullptr;
	_float4x4							m_ViewMatrix, m_ProjMatrix;
#endif // _DEBUG

public:
	class CRenderTarget* Find_RenderTarget(const _tchar* pRenderTargetTag) const;
	list<class CRenderTarget*>* Find_MRT(const _tchar* pMRTTag);

public:
	virtual void Free() override;
};

END