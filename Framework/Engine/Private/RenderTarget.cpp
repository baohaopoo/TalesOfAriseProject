#include "..\Public\RenderTarget.h"
#include "Shader.h"

CRenderTarget::CRenderTarget(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: m_pDevice(pDevice)
	, m_pDeviceContext(pDeviceContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

HRESULT CRenderTarget::NativeConstruct(_uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vColor)
{	
	if (nullptr == m_pDevice)
		return E_FAIL;

	

	m_vClearColor = vColor;
	
	ID3D11Texture2D*				pTexture2D = nullptr;

	D3D11_TEXTURE2D_DESC			TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = iWidth;
	TextureDesc.Height = iHeight;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = eFormat;
	TextureDesc.SampleDesc.Count = 1;
	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &pTexture2D)))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateRenderTargetView(pTexture2D, nullptr, &m_pRTV)))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateShaderResourceView(pTexture2D, nullptr, &m_pSRV)))
		return E_FAIL;

	Safe_Release(pTexture2D);
		
	return S_OK;
}

HRESULT CRenderTarget::Ready_DebugDesc(_float fTopX, _float fTopY, _float fWidth, _float fHeight)
{
	m_fX = fTopX + fWidth * 0.5f;
	m_fY = fTopY + fHeight * 0.5f;
	m_fSizeX = fWidth;
	m_fSizeY = fHeight;

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

	m_WorldMatrix._11 = m_fSizeX;
	m_WorldMatrix._22 = m_fSizeY;

	D3D11_VIEWPORT			Viewport;
	_uint					iNumViewport = 1;
	m_pDeviceContext->RSGetViewports(&iNumViewport, &Viewport);

	m_WorldMatrix._41 = m_fX - Viewport.Width * 0.5f;
	m_WorldMatrix._42 = -m_fY + Viewport.Height* 0.5f;

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_WorldMatrix)));

	return S_OK;
}

HRESULT CRenderTarget::Bind_WorldMatrix(CShader * pShader)
{
	pShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	pShader->Set_ShaderResourceView("g_Texture", m_pSRV);

	return S_OK;
}


HRESULT CRenderTarget::Clear_RenderTarget()
{
	if (nullptr == m_pDeviceContext)
		return E_FAIL;

	m_pDeviceContext->ClearRenderTargetView(m_pRTV, (_float*)&m_vClearColor);

	return S_OK;
}

CRenderTarget * CRenderTarget::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vColor)
{
	CRenderTarget*	pInstance = new CRenderTarget(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(iWidth, iHeight, eFormat, vColor)))
	{
		MSG_BOX(TEXT("Failed To Creating CRenderTarget"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CRenderTarget::Free()
{
	Safe_Release(m_pSRV);
	Safe_Release(m_pRTV);

	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
}
