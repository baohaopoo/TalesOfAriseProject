#include "..\Public\Target_Manager.h"
#include "RenderTarget.h"
#include "VIBuffer_Rect.h"
#include "Shader.h"

IMPLEMENT_SINGLETON(CTarget_Manager)

CTarget_Manager::CTarget_Manager()
{
}

ID3D11ShaderResourceView * CTarget_Manager::Get_SRV(const _tchar * pTargetTag) const
{
	CRenderTarget*			pRenderTarget = Find_RenderTarget(pTargetTag);

	if (nullptr == pRenderTarget)
		return nullptr;

	return pRenderTarget->Get_SRV();	
}

HRESULT CTarget_Manager::Add_RenderTarget(const _tchar * pRenderTargetTag, ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vColor)
{
	if (nullptr != Find_RenderTarget(pRenderTargetTag))
		return E_FAIL;

	CRenderTarget*		pRenderTarget = CRenderTarget::Create(pDevice, pDeviceContext, iWidth, iHeight, eFormat, vColor);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	m_RenderTargets.emplace(pRenderTargetTag, pRenderTarget);

	return S_OK;
}

HRESULT CTarget_Manager::Add_MRT(const _tchar * pMRTTag, const _tchar * pRenderTargetTag)
{
	
	CRenderTarget*		pRenderTarget = Find_RenderTarget(pRenderTargetTag);

	if (nullptr == pRenderTarget)
		return E_FAIL;

	list<CRenderTarget*>*		pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
	{
		list<CRenderTarget*>		MRTList;
		MRTList.push_back(pRenderTarget);

		m_MRTs.emplace(pMRTTag, MRTList);
	}
	else
		pMRTList->push_back(pRenderTarget);

	Safe_AddRef(pRenderTarget);

	return S_OK;
}

HRESULT CTarget_Manager::Create_DSV(ID3D11Device * pDevice, _uint iCX, _uint iCY)
{
	if (nullptr == pDevice)
		return E_FAIL;

	ID3D11Texture2D*		pDepthStencilTexture = nullptr;

	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = iCX;
	TextureDesc.Height = iCY;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	if (FAILED(pDevice->CreateTexture2D(&TextureDesc, nullptr, &pDepthStencilTexture)))
		return E_FAIL;

	if (FAILED(pDevice->CreateDepthStencilView(pDepthStencilTexture, nullptr, &m_pLightDepth_DSV)))
		return E_FAIL;

	Safe_Release(pDepthStencilTexture);

	ZeroMemory(&m_tLightDepth_ViewPort, sizeof(D3D11_VIEWPORT));
	m_tLightDepth_ViewPort.TopLeftX = 0.f;
	m_tLightDepth_ViewPort.TopLeftY = 0.f;
	m_tLightDepth_ViewPort.Width = (_float)g_iLightCX;
	m_tLightDepth_ViewPort.Height = (_float)g_iLightCY;
	m_tLightDepth_ViewPort.MinDepth = 0.f;
	m_tLightDepth_ViewPort.MaxDepth = 1.f;

	return S_OK;
}

HRESULT CTarget_Manager::Begin_MRT(ID3D11DeviceContext* pDeviceContext, const _tchar * pMRTTag)
{
	list<CRenderTarget*>*		pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	_uint iNumView = 1;
	pDeviceContext->OMGetRenderTargets(iNumView, &m_pBackBufferRTV, &m_pDepthStencilView);

	_uint iNumViewports = 1;
	pDeviceContext->RSGetViewports(&iNumViewports, &m_tOrigin_ViewPort);

	_uint iNumViews = 0;
	ID3D11RenderTargetView*		pMRT[8] = { nullptr };

	for (auto& pRenderTarget : *pMRTList)
	{
		pRenderTarget->Clear_RenderTarget();
		pMRT[iNumViews++] = pRenderTarget->Get_RTV();
	}

	pDeviceContext->OMSetRenderTargets(iNumViews, pMRT, m_pDepthStencilView);

	return S_OK;
}

HRESULT CTarget_Manager::End_MRT(ID3D11DeviceContext* pDeviceContext, const _tchar * pMRTTag)
{
	pDeviceContext->OMSetRenderTargets(1, &m_pBackBufferRTV, m_pDepthStencilView);
	ID3D11ShaderResourceView* pSRV[128] = { nullptr };
	pDeviceContext->PSSetShaderResources(0, 128, pSRV);

	Safe_Release(m_pBackBufferRTV);
	Safe_Release(m_pDepthStencilView);

	pDeviceContext->RSSetViewports(1, &m_tOrigin_ViewPort);

	return S_OK;
}

HRESULT CTarget_Manager::Begin_LightDepth_MRT(ID3D11DeviceContext * pDeviceContext, const _tchar * pMRTTag)
{
	list<CRenderTarget*>*		pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	ID3D11ShaderResourceView* pSRV[128] = { nullptr };
	pDeviceContext->PSSetShaderResources(0, 128, pSRV);

	_uint iNumView = 1;
	pDeviceContext->OMGetRenderTargets(iNumView, &m_pBackBufferRTV, &m_pDepthStencilView);

	_uint iNumViewports = 1;
	pDeviceContext->RSGetViewports(&iNumViewports, &m_tOrigin_ViewPort);
	_uint iNumViews = 0;

	ID3D11RenderTargetView*		pMRT[8] = { nullptr };

	for (auto& pRenderTarget : *pMRTList)
	{
		pRenderTarget->Clear_RenderTarget();
		pMRT[iNumViews++] = pRenderTarget->Get_RTV();
	}
	pDeviceContext->OMSetRenderTargets(iNumViews, pMRT, m_pLightDepth_DSV);
	pDeviceContext->ClearDepthStencilView(m_pLightDepth_DSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	pDeviceContext->RSSetViewports(1, &m_tLightDepth_ViewPort);
	return S_OK;
}

HRESULT CTarget_Manager::Begin_NonClear_MRT(ID3D11DeviceContext * pDeviceContext, const _tchar * pMRTTag)
{
	list<CRenderTarget*>*		pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	_uint iNumView = 1;
	pDeviceContext->OMGetRenderTargets(iNumView, &m_pBackBufferRTV, &m_pDepthStencilView);

	_uint iNumViewports = 1;
	pDeviceContext->RSGetViewports(&iNumViewports, &m_tOrigin_ViewPort);

	_uint iNumViews = 0;
	ID3D11RenderTargetView*		pMRT[8] = { nullptr };

	for (auto& pRenderTarget : *pMRTList)
	{
		pMRT[iNumViews++] = pRenderTarget->Get_RTV();
	}

	pDeviceContext->OMSetRenderTargets(iNumViews, pMRT, m_pDepthStencilView);

	return S_OK;
}

HRESULT CTarget_Manager::Clear_MRT(ID3D11DeviceContext * pDeviceContext, const _tchar * pMRTTag)
{
	list<CRenderTarget*>*		pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	for (auto& pRenderTarget : *pMRTList)
	{
		pRenderTarget->Clear_RenderTarget();
	}
	return S_OK;
}

#ifdef _DEBUG
HRESULT CTarget_Manager::Ready_DebugBuffer(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	m_pShader = CShader::Create(pDevice, pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_Deferred.hlsl"), VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;

	m_pDebugBuffer = CVIBuffer_Rect::Create(pDevice, pDeviceContext);
	if (nullptr == m_pDebugBuffer)
		return E_FAIL;

	D3D11_VIEWPORT			Viewport;
	_uint		iNumViewports = 1;
	pDeviceContext->RSGetViewports(&iNumViewports, &Viewport);

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH(Viewport.Width, Viewport.Height, 0.f, 1.f)));
	
	return S_OK;
}
HRESULT CTarget_Manager::Ready_DebugDesc(const _tchar * pRenderTargetTag, _float fTopX, _float fTopY, _float fWidth, _float fHeight)
{
	CRenderTarget*		pRenderTarget = Find_RenderTarget(pRenderTargetTag);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	return pRenderTarget->Ready_DebugDesc(fTopX, fTopY, fWidth, fHeight);	
}
HRESULT CTarget_Manager::Render_Debug(const _tchar * pMRTTag)
{
	list<CRenderTarget*>*	pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	for (auto& pRenderTarget : *pMRTList)
	{
		if (FAILED(pRenderTarget->Bind_WorldMatrix(m_pShader)))
			return E_FAIL;
		if (FAILED(m_pShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
			return E_FAIL;

		m_pShader->Begin(0);

		m_pDebugBuffer->Render();
	}

	return S_OK;
}
#endif // _DEBUG

CRenderTarget * CTarget_Manager::Find_RenderTarget(const _tchar * pRenderTargetTag) const 
{
	auto	iter = find_if(m_RenderTargets.begin(), m_RenderTargets.end(), CTagFinder(pRenderTargetTag));

	if (iter == m_RenderTargets.end())
		return nullptr;

	return iter->second;	
}

list<class CRenderTarget*>* CTarget_Manager::Find_MRT(const _tchar * pMRTTag)
{
	auto	iter = find_if(m_MRTs.begin(), m_MRTs.end(), CTagFinder(pMRTTag));

	if (iter == m_MRTs.end())
		return nullptr;

	return &iter->second;	
}

void CTarget_Manager::Free()
{
	for (auto& Pair : m_MRTs)
	{
		for (auto& pRenderTarget : Pair.second)
			Safe_Release(pRenderTarget);

		Pair.second.clear();	
	}

	m_MRTs.clear();
		

	for (auto& Pair : m_RenderTargets)
		Safe_Release(Pair.second);

	m_RenderTargets.clear();

	Safe_Release(m_pLightDepth_DSV);

#ifdef _DEBUG
	Safe_Release(m_pShader);
	Safe_Release(m_pDebugBuffer);
#endif // _DEBUG
}
