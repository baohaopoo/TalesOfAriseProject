#include "..\Public\PostProcessing.h"
#include "Shader.h"
#include "Target_Manager.h"
#include "VIBuffer_Rect.h"

IMPLEMENT_SINGLETON(CPostProcessing)

CPostProcessing::CPostProcessing()
	:m_pTarget_Manager(CTarget_Manager::GetInstance())
{
	Safe_AddRef(m_pTarget_Manager);
}

_float CPostProcessing::Get_MiddleGray()
{
	return m_fMiddleGrey;
}

_float CPostProcessing::Get_White()
{
	return m_fWhite;
}

ID3D11ShaderResourceView* CPostProcessing::Get_DownScale4x4SRV(DOWNSCALE_TYPE eType)
{
	return m_pDownScale4x4SRV[eType];
}

ID3D11UnorderedAccessView* CPostProcessing::Get_DownScale4x4UAV(DOWNSCALE_TYPE eType)
{
	return m_pDownScale4x4UAV[eType];
}

void CPostProcessing::Set_MiddleGray(_float fMiddleGray)
{
	m_fMiddleGrey = fMiddleGray;
}

void CPostProcessing::Set_White(_float fWhite)
{
	m_fWhite = fWhite;
}

void CPostProcessing::Set_RadialBlur(_bool bRadialBlur)
{
	m_bRadialBlur = bRadialBlur;
}

void CPostProcessing::Set_RadialBlurLength(_float fLength)
{
	m_fRadialLength = fLength;
}

void CPostProcessing::Set_RadialNlurStrength(_float fStrength)
{
	m_fRadialStrength = fStrength;
}

HRESULT CPostProcessing::Init_PostProcessing(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	m_pDevice = pDevice;
	m_pDeviceContext = pDeviceContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);

	_uint iNumViewPorts = 1;
	D3D11_VIEWPORT tViewPort;
	m_pDeviceContext->RSGetViewports(&iNumViewPorts, &tViewPort);

	m_iWidth = (_uint)tViewPort.Width;
	m_iHeight = (_uint)tViewPort.Height;
	m_iDownScaleGroups = (UINT)ceil((_float)(m_iWidth * m_iHeight / 16) / 1024.f);

	if (FAILED(SetUp_Shader()))
	{
		MSG_BOX(L"Failed To CPostProcessing : Init_PostProcessing : SetUp_Shader");
		return E_FAIL;
	}

	if (FAILED(SetUp_Texture_Buffer()))
	{
		MSG_BOX(L"Failed To CPostProcessing : Init_PostProcessing : SetUp_Texture_Buffer");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CPostProcessing::Combine_Resources(CVIBuffer_Rect* pVIBuffer, _float4x4* pMatrix)
{
	_uint vRes[2] = { m_iWidth / 4, m_iHeight / 4 };
	if (FAILED(m_pShader_Blur_CS->Set_RawValue("Res", vRes, sizeof(_uint) * 2)))
		return E_FAIL;
	if (FAILED(m_pShader_DownScale_CS->Set_RawValue("Res", vRes, sizeof(_uint) * 2)))
		return E_FAIL;

	_uint iDomain = vRes[0] * vRes[1];
	if (FAILED(m_pShader_DownScale_CS->Set_RawValue("Domain", &iDomain, sizeof(_uint))))
		return E_FAIL;
	if (FAILED(m_pShader_DownScale_CS->Set_RawValue("GroupSize", &m_iDownScaleGroups, sizeof(_uint))))
		return E_FAIL;
	if (FAILED(m_pShader_DownScale_CS->Set_RawValue("Adaptation", &m_fAdaptation, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShader_DownScale_CS->Set_RawValue("fBloomThreshold", &m_fBloomThreshold, sizeof(_float))))
		return E_FAIL;

	ID3D11RenderTargetView* pRTV = nullptr;
	ID3D11DepthStencilView* pDSV = nullptr;

	m_pDeviceContext->OMGetRenderTargets(1, &pRTV, &pDSV);

	ID3D11RenderTargetView* pRTVNullptr = nullptr;
	m_pDeviceContext->OMSetRenderTargets(1, &pRTVNullptr, nullptr);

	if (FAILED(DownScale()))
		return E_FAIL;
	if (FAILED(Bloom()))
		return E_FAIL;
	if (FAILED(Blur(m_pTempSRV[0], m_pBloomUAV)))
		return E_FAIL;

	if (FAILED(DownScale4x4(m_pTarget_Manager->Get_SRV(L"Target_Blur"), m_pDownScale4x4UAV[BLUR])))
		return E_FAIL;
	if (FAILED(Blur_Object(m_pDownScale4x4SRV[BLUR], m_pDownScale4x4UAV[BLUR])))
		return E_FAIL;
	
	m_pDeviceContext->OMSetRenderTargets(1, &pRTV, pDSV);
	Safe_Release(pRTV);
	Safe_Release(pDSV);

	if (FAILED(FinalPass(pVIBuffer, pMatrix)))
		return E_FAIL;

	/*if (FAILED(TestPass(pVIBuffer, pMatrix, m_pDownScaleEmissiveSRV)))
		return E_FAIL;*/

	return S_OK;
}

HRESULT CPostProcessing::DownScale_Blur(ID3D11ShaderResourceView* pInput, DOWNSCALE_TYPE eType)
{
	if (FAILED(DownScale4x4(pInput, m_pDownScale4x4UAV[eType])))
		return E_FAIL;
	if (FAILED(Blur_Object(m_pDownScale4x4SRV[eType], m_pDownScale4x4UAV[eType])))
		return E_FAIL;

	return S_OK;
}

HRESULT CPostProcessing::DownScale()
{
	if (FAILED(m_pShader_DownScale_CS->Set_UnorderedAccessView("AverageLum", m_pDownScale1DUAV)))
		return E_FAIL;
	if (FAILED(m_pShader_DownScale_CS->Set_UnorderedAccessView("HDRDownScale", m_pDownScaleUAV)))
		return E_FAIL;

	if (FAILED(m_pShader_DownScale_CS->Set_ShaderResourceView("HDRTex", m_pTarget_Manager->Get_SRV(L"Target_HDR"))))
		return E_FAIL;

	m_pShader_DownScale_CS->Begin(0);
	m_pDeviceContext->Dispatch(m_iDownScaleGroups, 1, 1);

	if (FAILED(m_pShader_DownScale_CS->Set_UnorderedAccessView("AverageLum", m_pAvgLumUAV)))
		return E_FAIL;

	if (FAILED(m_pShader_DownScale_CS->Set_ShaderResourceView("AverageValues1D", m_pDownScale1DSRV)))
		return E_FAIL;
	if (FAILED(m_pShader_DownScale_CS->Set_ShaderResourceView("PrevAvgLum", m_pPrevAvgLumSRV)))
		return E_FAIL;

	m_pShader_DownScale_CS->Begin(1);
	m_pDeviceContext->Dispatch(1, 1, 1);

	m_pDeviceContext->CSSetShaderResources(0, 128, m_pClearSRV);
	m_pDeviceContext->CSSetUnorderedAccessViews(0, 2, m_pClearUAV, nullptr);
	return S_OK;
}

HRESULT CPostProcessing::Bloom()
{
	if (FAILED(m_pShader_DownScale_CS->Set_ShaderResourceView("HDRDownScaleTex", m_pDownScaleSRV)))
		return E_FAIL;
	if (FAILED(m_pShader_DownScale_CS->Set_ShaderResourceView("AvgLum", m_pAvgLumSRV)))
		return E_FAIL;

	if (FAILED(m_pShader_DownScale_CS->Set_UnorderedAccessView("Bloom", m_pTempUAV[0])))
		return E_FAIL;

	m_pShader_DownScale_CS->Begin(2);
	m_pDeviceContext->Dispatch(m_iDownScaleGroups, 1, 1);

	m_pDeviceContext->CSSetShaderResources(0, 128, m_pClearSRV);
	m_pDeviceContext->CSSetUnorderedAccessViews(0, 1, m_pClearUAV, nullptr);

	return S_OK;
}

HRESULT CPostProcessing::Blur(ID3D11ShaderResourceView* pInput, ID3D11UnorderedAccessView* pOutput)
{
	_uint vRes[2] = { m_iWidth / 4, m_iHeight / 4 };
	if (FAILED(m_pShader_Blur_CS->Set_RawValue("Res", vRes, sizeof(_uint) * 2)))
		return E_FAIL;

	if (FAILED(m_pShader_Blur_CS->Set_UnorderedAccessView("Output", m_pTempUAV[1])))
		return E_FAIL;

	if (FAILED(m_pShader_Blur_CS->Set_ShaderResourceView("Input", pInput)))
		return E_FAIL;

	m_pShader_Blur_CS->Begin(1);
	m_pDeviceContext->Dispatch((_uint)ceil((m_iWidth / 4.0f) / (128.0f - 12.0f)), (_uint)ceil(m_iHeight / 4.0f), 1);
	
	if (FAILED(m_pShader_Blur_CS->Set_UnorderedAccessView("Output", pOutput)))
		return E_FAIL;
	
	if (FAILED(m_pShader_Blur_CS->Set_ShaderResourceView("Input", m_pTempSRV[1])))
		return E_FAIL;

	m_pShader_Blur_CS->Begin(0);
	m_pDeviceContext->Dispatch((_uint)ceil(m_iWidth / 4.0f), (_uint)ceil((m_iHeight / 4.0f) / (128.0f - 12.0f)), 1);

	m_pDeviceContext->CSSetShaderResources(0, 128, m_pClearSRV);
	m_pDeviceContext->CSSetUnorderedAccessViews(0, 1, m_pClearUAV, nullptr);

	return S_OK;
}

HRESULT CPostProcessing::FinalPass(CVIBuffer_Rect* pVIBuffer, _float4x4* pMatrix)
{
	if (FAILED(m_pShader_PostProcessing_FX->Set_ShaderResourceView("HDRTex", m_pTarget_Manager->Get_SRV(L"Target_HDR"))))
		return E_FAIL;
	if (FAILED(m_pShader_PostProcessing_FX->Set_ShaderResourceView("AvgLum", m_pAvgLumSRV)))
		return E_FAIL;
	if (FAILED(m_pShader_PostProcessing_FX->Set_ShaderResourceView("BloomTex", m_pBloomSRV)))
		return E_FAIL;
	if (FAILED(m_pShader_PostProcessing_FX->Set_ShaderResourceView("BlurTexture", m_pDownScale4x4SRV[BLUR])))
		return E_FAIL;

	if (FAILED(m_pShader_PostProcessing_FX->Set_RawValue("g_WorldMatrix", &pMatrix[0], sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader_PostProcessing_FX->Set_RawValue("g_ViewMatrix", &pMatrix[1], sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader_PostProcessing_FX->Set_RawValue("g_ProjMatrix", &pMatrix[2], sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShader_PostProcessing_FX->Set_RawValue("MiddleGrey", &m_fMiddleGrey, sizeof(_float))))
		return E_FAIL;

	_float fWhite = m_fWhite;
	fWhite *= m_fMiddleGrey;
	fWhite *= fWhite;
	if (FAILED(m_pShader_PostProcessing_FX->Set_RawValue("LumWhiteSqr", &fWhite, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShader_PostProcessing_FX->Set_RawValue("BloomScale", &m_fBloomScale, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShader_PostProcessing_FX->Set_RawValue("g_bRadialBlur", &m_bRadialBlur, sizeof(_bool))))
		return E_FAIL;
	if (FAILED(m_pShader_PostProcessing_FX->Set_RawValue("g_fDistance", &m_fRadialLength, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShader_PostProcessing_FX->Set_RawValue("g_fStrength", &m_fRadialStrength, sizeof(_float))))
		return E_FAIL;
	
	m_pShader_PostProcessing_FX->Begin(0);
	pVIBuffer->Render();

	return S_OK;
}

#ifdef _DEBUG
HRESULT CPostProcessing::TestPass(CVIBuffer_Rect * pVIBuffer, _float4x4 * pMatrix, ID3D11ShaderResourceView* pSRV)
{
	if (FAILED(m_pShader_PostProcessing_FX->Set_RawValue("g_WorldMatrix", &pMatrix[0], sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader_PostProcessing_FX->Set_RawValue("g_ViewMatrix", &pMatrix[1], sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader_PostProcessing_FX->Set_RawValue("g_ProjMatrix", &pMatrix[2], sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader_PostProcessing_FX->Set_ShaderResourceView("HDRTex", pSRV)))
		return E_FAIL;

	m_pShader_PostProcessing_FX->Begin(1);
	pVIBuffer->Render();

	return S_OK;
}
#endif // _DEBUG

HRESULT CPostProcessing::DownScale4x4(ID3D11ShaderResourceView* pInput, ID3D11UnorderedAccessView* pOutput)
{
	_uint iScale = 4;
	_uint vRes[2] = { m_iWidth / iScale, m_iHeight / iScale };

	if (FAILED(m_pShader_Blur_CS->Set_UnorderedAccessView("Output", pOutput)))
		return E_FAIL;

	if (FAILED(m_pShader_Blur_CS->Set_ShaderResourceView("Input", pInput)))
		return E_FAIL;

	m_pShader_Blur_CS->Begin(2);
	m_pDeviceContext->Dispatch(m_iDownScaleGroups, 1, 1);

	m_pDeviceContext->CSSetShaderResources(0, 128, m_pClearSRV);
	m_pDeviceContext->CSSetUnorderedAccessViews(0, 1, m_pClearUAV, nullptr);

	return S_OK;
}

HRESULT CPostProcessing::Blur_Object(ID3D11ShaderResourceView* pInput, ID3D11UnorderedAccessView* pOutput)
{
	_uint iScale = 4.f;
	_uint vRes[2] = { m_iWidth / iScale, m_iHeight / iScale };
	if (FAILED(m_pShader_Blur_CS->Set_RawValue("Res", vRes, sizeof(_uint) * 2)))
		return E_FAIL;

	if (FAILED(m_pShader_Blur_CS->Set_UnorderedAccessView("Output", m_pDownScale4x4UAV[TEMP])))
		return E_FAIL;

	if (FAILED(m_pShader_Blur_CS->Set_ShaderResourceView("Input", pInput)))
		return E_FAIL;

	m_pShader_Blur_CS->Begin(1);
	m_pDeviceContext->Dispatch((_uint)ceil((m_iWidth / iScale) / (128.f - 12.f)), (_uint)ceil(m_iHeight / iScale), 1);

	if (FAILED(m_pShader_Blur_CS->Set_UnorderedAccessView("Output", pOutput)))
		return E_FAIL;

	if (FAILED(m_pShader_Blur_CS->Set_ShaderResourceView("Input", m_pDownScale4x4SRV[TEMP])))
		return E_FAIL;

	m_pShader_Blur_CS->Begin(0);
	m_pDeviceContext->Dispatch((_uint)ceil(m_iWidth / iScale), (_uint)ceil((m_iHeight / iScale) / (128.f - 12.f)), 1);

	m_pDeviceContext->CSSetShaderResources(0, 128, m_pClearSRV);
	m_pDeviceContext->CSSetUnorderedAccessViews(0, 1, m_pClearUAV, nullptr);

	return S_OK;
}

HRESULT CPostProcessing::SetUp_Shader()
{
	m_pShader_Blur_CS = CShader::Create(m_pDevice, m_pDeviceContext, L"../Bin/ShaderFiles/Shader_Blur.hlsl", nullptr, 0, CShader::CS);
	m_pShader_DownScale_CS = CShader::Create(m_pDevice, m_pDeviceContext, L"../Bin/ShaderFiles/Shader_DownScale.hlsl", nullptr, 0, CShader::CS);
	m_pShader_PostProcessing_FX = CShader::Create(m_pDevice, m_pDeviceContext, L"../Bin/ShaderFiles/Shader_PostProcessing.hlsl", VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements);

	return S_OK;
}

HRESULT CPostProcessing::SetUp_Texture_Buffer()
{
	D3D11_TEXTURE2D_DESC tTextureDesc;
	ZeroMemory(&tTextureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	tTextureDesc.Width = m_iWidth / 4;
	tTextureDesc.Height = m_iHeight / 4;
	tTextureDesc.MipLevels = 1;
	tTextureDesc.ArraySize = 1;
	tTextureDesc.Format = DXGI_FORMAT_R16G16B16A16_TYPELESS;
	tTextureDesc.SampleDesc.Count = 1;
	tTextureDesc.SampleDesc.Quality = 0;
	tTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	tTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	tTextureDesc.CPUAccessFlags = 0;
	tTextureDesc.MiscFlags = 0;
	
	if (FAILED(m_pDevice->CreateTexture2D(&tTextureDesc, nullptr, &m_pDownScaleRTV)))
		return E_FAIL;

	D3D11_SHADER_RESOURCE_VIEW_DESC tSRVDesc;
	ZeroMemory(&tSRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	tSRVDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	tSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	tSRVDesc.Texture2D.MipLevels = 1;

	if (FAILED(m_pDevice->CreateShaderResourceView(m_pDownScaleRTV, &tSRVDesc, &m_pDownScaleSRV)))
		return E_FAIL;

	D3D11_UNORDERED_ACCESS_VIEW_DESC tUAVDesc;
	ZeroMemory(&tUAVDesc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));
	tUAVDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	tUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	tUAVDesc.Buffer.FirstElement = 0;
	tUAVDesc.Buffer.NumElements = m_iWidth * m_iHeight / 16;

	if (FAILED(m_pDevice->CreateUnorderedAccessView(m_pDownScaleRTV, &tUAVDesc, &m_pDownScaleUAV)))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateTexture2D(&tTextureDesc, nullptr, &m_pTempRTV[0])))
		return E_FAIL;
	if (FAILED(m_pDevice->CreateShaderResourceView(m_pTempRTV[0], &tSRVDesc, &m_pTempSRV[0])))
		return E_FAIL;
	if(FAILED(m_pDevice->CreateUnorderedAccessView(m_pTempRTV[0], &tUAVDesc, &m_pTempUAV[0])))
		return E_FAIL;
	
	if (FAILED(m_pDevice->CreateTexture2D(&tTextureDesc, nullptr, &m_pTempRTV[1])))
		return E_FAIL;
	if (FAILED(m_pDevice->CreateShaderResourceView(m_pTempRTV[1], &tSRVDesc, &m_pTempSRV[1])))
		return E_FAIL;
	if (FAILED(m_pDevice->CreateUnorderedAccessView(m_pTempRTV[1], &tUAVDesc, &m_pTempUAV[1])))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateTexture2D(&tTextureDesc, nullptr, &m_pBloomRTV)))
		return E_FAIL;
	if (FAILED(m_pDevice->CreateShaderResourceView(m_pBloomRTV, &tSRVDesc, &m_pBloomSRV)))
		return E_FAIL;
	if (FAILED(m_pDevice->CreateUnorderedAccessView(m_pBloomRTV, &tUAVDesc, &m_pBloomUAV)))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateTexture2D(&tTextureDesc, nullptr, &m_pDownScale4x4RTV[TEMP])))
		return E_FAIL;
	if (FAILED(m_pDevice->CreateShaderResourceView(m_pDownScale4x4RTV[TEMP], &tSRVDesc, &m_pDownScale4x4SRV[TEMP])))
		return E_FAIL;
	if (FAILED(m_pDevice->CreateUnorderedAccessView(m_pDownScale4x4RTV[TEMP], &tUAVDesc, &m_pDownScale4x4UAV[TEMP])))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateTexture2D(&tTextureDesc, nullptr, &m_pDownScale4x4RTV[BLUR])))
		return E_FAIL;
	if (FAILED(m_pDevice->CreateShaderResourceView(m_pDownScale4x4RTV[BLUR], &tSRVDesc, &m_pDownScale4x4SRV[BLUR])))
		return E_FAIL;
	if (FAILED(m_pDevice->CreateUnorderedAccessView(m_pDownScale4x4RTV[BLUR], &tUAVDesc, &m_pDownScale4x4UAV[BLUR])))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateTexture2D(&tTextureDesc, nullptr, &m_pDownScale4x4RTV[EMISSIVE])))
		return E_FAIL;
	if (FAILED(m_pDevice->CreateShaderResourceView(m_pDownScale4x4RTV[EMISSIVE], &tSRVDesc, &m_pDownScale4x4SRV[EMISSIVE])))
		return E_FAIL;
	if (FAILED(m_pDevice->CreateUnorderedAccessView(m_pDownScale4x4RTV[EMISSIVE], &tUAVDesc, &m_pDownScale4x4UAV[EMISSIVE])))
		return E_FAIL;
	
	D3D11_BUFFER_DESC tBufferDesc;
	ZeroMemory(&tBufferDesc, sizeof(D3D11_BUFFER_DESC));
	tBufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	tBufferDesc.StructureByteStride = sizeof(_float);
	tBufferDesc.ByteWidth = m_iDownScaleGroups * tBufferDesc.StructureByteStride;
	tBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	tBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	if (FAILED(m_pDevice->CreateBuffer(&tBufferDesc, nullptr, &m_pDownScale1DBuffer)))
		return E_FAIL;

	ZeroMemory(&tUAVDesc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));
	tUAVDesc.Format = DXGI_FORMAT_UNKNOWN;
	tUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	tUAVDesc.Buffer.FirstElement = 0;
	tUAVDesc.Buffer.NumElements = m_iDownScaleGroups;

	if (FAILED(m_pDevice->CreateUnorderedAccessView(m_pDownScale1DBuffer, &tUAVDesc, &m_pDownScale1DUAV)))
		return E_FAIL;

	tSRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	tSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	tSRVDesc.Buffer.FirstElement = 0;
	tSRVDesc.Buffer.NumElements = m_iDownScaleGroups;

	if (FAILED(m_pDevice->CreateShaderResourceView(m_pDownScale1DBuffer, &tSRVDesc, &m_pDownScale1DSRV)))
		return E_FAIL;

	tBufferDesc.ByteWidth = sizeof(_float);
	if (FAILED(m_pDevice->CreateBuffer(&tBufferDesc, nullptr, &m_pAvgLumBuffer)))
		return E_FAIL;

	tUAVDesc.Buffer.NumElements = 1;
	if (FAILED(m_pDevice->CreateUnorderedAccessView(m_pAvgLumBuffer, &tUAVDesc, &m_pAvgLumUAV)))
		return E_FAIL;

	tSRVDesc.Buffer.NumElements = 1;
	if (FAILED(m_pDevice->CreateShaderResourceView(m_pAvgLumBuffer, &tSRVDesc, &m_pAvgLumSRV)))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateBuffer(&tBufferDesc, nullptr, &m_pPrevAvgLumBuffer)))
		return E_FAIL;
	if (FAILED(m_pDevice->CreateUnorderedAccessView(m_pPrevAvgLumBuffer, &tUAVDesc, &m_pPrevAvgLumUAV)))
		return E_FAIL;
	if (FAILED(m_pDevice->CreateShaderResourceView(m_pPrevAvgLumBuffer, &tSRVDesc, &m_pPrevAvgLumSRV)))
		return E_FAIL;
	
	return S_OK;
}

void CPostProcessing::Free()
{
	Safe_Release(m_pTarget_Manager);

	Safe_Release(m_pShader_PostProcessing_FX);
	Safe_Release(m_pShader_DownScale_CS);

	Safe_Release(m_pBloomRTV);
	Safe_Release(m_pBloomSRV);
	Safe_Release(m_pBloomUAV);

	Safe_Release(m_pDownScaleRTV);
	Safe_Release(m_pDownScaleSRV);
	Safe_Release(m_pDownScaleUAV);

	Safe_Release(m_pDownScale1DBuffer);
	Safe_Release(m_pDownScale1DSRV);
	Safe_Release(m_pDownScale1DUAV);

	Safe_Release(m_pAvgLumBuffer);
	Safe_Release(m_pAvgLumSRV);
	Safe_Release(m_pAvgLumUAV);

	Safe_Release(m_pPrevAvgLumBuffer);
	Safe_Release(m_pPrevAvgLumSRV);
	Safe_Release(m_pPrevAvgLumUAV);

	for (auto& pBuffer : m_pTempRTV)
	{
		Safe_Release(pBuffer);
	}
	for (auto& pSRV : m_pTempSRV)
	{
		Safe_Release(pSRV);
	}
	for (auto& pUAV : m_pTempUAV)
	{
		Safe_Release(pUAV);
	}

	for (auto& pRTV : m_pDownScale4x4RTV)
	{
		Safe_Release(pRTV);
	}
	for (auto& pSRV : m_pDownScale4x4SRV)
	{
		Safe_Release(pSRV);
	}
	for (auto& pUAV : m_pDownScale4x4UAV)
	{
		Safe_Release(pUAV);
	}

	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
}