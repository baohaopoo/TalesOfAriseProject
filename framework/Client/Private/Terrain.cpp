#include "stdafx.h"
#include "..\Public\Terrain.h"
#include "GameInstance.h"

CTerrain::CTerrain(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{

}

CTerrain::CTerrain(const CTerrain & rhs)
	: CGameObject(rhs)
{
}

HRESULT CTerrain::NativeConstruct_Prototype()
{

	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrain::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);
	
	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;	

	if (FAILED(Create_MaskTexture()))
		return E_FAIL;


	return S_OK;
}

void CTerrain::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

}

void CTerrain::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);

	m_pVIBufferCom->Culling(m_pTransformCom);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
}

HRESULT CTerrain::Render()
{
	if (nullptr == m_pShaderCom || 
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;
	
	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;


#ifdef _DEBUG
	//CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	//	
	//pGameInstance->Render_Navigation();	

	//RELEASE_INSTANCE(CGameInstance);
#endif // _DEBUG

	return S_OK;
}

HRESULT CTerrain::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNorTex"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_TUTORIAL, TEXT("Prototype_Component_VIBuffer_Terrain"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Diffuse */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Diffuse"), LEVEL_TUTORIAL, TEXT("Prototype_Component_Texture_Terrain"), (CComponent**)&m_pTextureCom[TYPE_DIFFUSE])))
		return E_FAIL;

	/* For.Com_Mask */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Mask"), LEVEL_TUTORIAL, TEXT("Prototype_Component_Texture_TerrainMask"), (CComponent**)&m_pTextureCom[TYPE_MASK])))
		return E_FAIL;

	/* For.Com_Brush */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Brush"), LEVEL_TUTORIAL, TEXT("Prototype_Component_Texture_Brush"), (CComponent**)&m_pTextureCom[TYPE_BRUSH])))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrain::SetUp_ConstantTable()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransformCom->Bind_WorldMatrixOnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;	

	if (FAILED(m_pTextureCom[TYPE_DIFFUSE]->SetUp_ShaderResourceView(m_pShaderCom, "g_SourDiffuseTexture", 0)))
		return E_FAIL;
	if (FAILED(m_pTextureCom[TYPE_DIFFUSE]->SetUp_ShaderResourceView(m_pShaderCom, "g_DestDiffuseTexture", 1)))
		return E_FAIL;
	if (FAILED(m_pTextureCom[TYPE_MASK]->SetUp_ShaderResourceView(m_pShaderCom, "g_MaskTexture", 0)))
		return E_FAIL;
	if (FAILED(m_pTextureCom[TYPE_BRUSH]->SetUp_ShaderResourceView(m_pShaderCom, "g_BrushTexture", 0)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CTerrain::Create_MaskTexture()
{
	ID3D11Texture2D*			pTexture2D;

	D3D11_TEXTURE2D_DESC		TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	/*
	UINT Width;
	UINT Height;
	UINT MipLevels;
	UINT ArraySize;
	DXGI_FORMAT Format;
	DXGI_SAMPLE_DESC SampleDesc;
	D3D11_USAGE Usage;
	UINT BindFlags;
	UINT CPUAccessFlags;
	UINT MiscFlags;
	*/

	TextureDesc.Width = 129;
	TextureDesc.Height = 129;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;
	TextureDesc.Usage = D3D11_USAGE_DYNAMIC;
	TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	TextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	TextureDesc.MiscFlags = 0;

	_ulong*		pPixel = new _ulong[TextureDesc.Width * TextureDesc.Height];
	ZeroMemory(pPixel, sizeof(_ulong) * TextureDesc.Width * TextureDesc.Height);

	pPixel[0] = D3DCOLOR_ARGB(255, 255, 0, 0);

	/*for (_uint i = 0; i < TextureDesc.Height; ++i)
	{
		for (_uint j = 0; j < TextureDesc.Width; ++j)
		{			
			_uint	iIndex = i * TextureDesc.Width + j;

			pPixel[iIndex] = D3DCOLOR_ARGB(255, 255, 0, 0);
		}
	}
*/
	D3D11_SUBRESOURCE_DATA		SubResourceData;
	ZeroMemory(&SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));

	SubResourceData.pSysMem = pPixel;
	SubResourceData.SysMemPitch = TextureDesc.Width * 4;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, &SubResourceData, &pTexture2D)))
		return E_FAIL;	

	D3D11_MAPPED_SUBRESOURCE			SubResource;
	ZeroMemory(&SubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	m_pDeviceContext->Map(pTexture2D, 0, D3D11_MAP_WRITE_DISCARD/*D3D11_MAP_WRITE_NO_OVERWRITE*/, 0, &SubResource);

	for (_uint i = 0; i < TextureDesc.Height; ++i)
	{
		for (_uint j = 0; j < SubResource.RowPitch / 4; ++j)
		{
			_uint	iIndex = i * SubResource.RowPitch / 4 + j;

			if(j > 64)
				((_ulong*)SubResource.pData)[iIndex] = D3DCOLOR_ARGB(255, 0, 0, 0);
			else
				((_ulong*)SubResource.pData)[iIndex] = D3DCOLOR_ARGB(255, 255, 255, 255);

		}
	}

	m_pDeviceContext->Unmap(pTexture2D, 0);

	m_pDevice->CreateShaderResourceView(pTexture2D, nullptr, &m_pMaskSRV);

	if (FAILED(DirectX::SaveWICTextureToFile(m_pDeviceContext, pTexture2D, GUID_ContainerFormatPng, TEXT("../Bin/Mask.png"), &GUID_WICPixelFormat32bppBGRA)))
		return E_FAIL;

	Safe_Delete_Array(pPixel);
	Safe_Release(pTexture2D);

	return S_OK;
}

CTerrain * CTerrain::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CTerrain*	pInstance = new CTerrain(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CTerrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTerrain::Clone(void * pArg)
{
	CTerrain*	pInstance = new CTerrain(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CTerrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrain::Free()
{
	__super::Free();


	for (_uint i = 0; i < TYPE_END; ++i)
		Safe_Release(m_pTextureCom[i]);	
	
	Safe_Release(m_pMaskSRV);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
