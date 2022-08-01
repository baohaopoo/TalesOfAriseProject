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

HRESULT CTarget_Manager::Begin_MRT(ID3D11DeviceContext* pDeviceContext, const _tchar * pMRTTag)
{
	list<CRenderTarget*>*		pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	_uint		iNumView = 1;

	pDeviceContext->OMGetRenderTargets(iNumView, &m_pBackBufferRTV, &m_pDepthStencilView);

	_uint			iNumViews = 0;

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

	ID3D11ShaderResourceView*		pSRV[128] = { nullptr };

	pDeviceContext->PSSetShaderResources(0, 128, pSRV);

	Safe_Release(m_pBackBufferRTV);
	Safe_Release(m_pDepthStencilView);

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

#ifdef _DEBUG
	Safe_Release(m_pShader);
	Safe_Release(m_pDebugBuffer);
#endif // _DEBUG
}
