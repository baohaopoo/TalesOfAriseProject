#include "..\Public\CustomFont.h"

CCustomFont::CCustomFont(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: m_pDevice(pDevice)
	, m_pDeviceContext(pDeviceContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

HRESULT CCustomFont::NativeConstruct(const _tchar * pFontFilePath)
{		
	m_pBatch = new SpriteBatch(m_pDeviceContext);
	m_pFont = new SpriteFont(m_pDevice, pFontFilePath);	

	return S_OK;
}

HRESULT CCustomFont::Render(const _tchar* pText, _float2 vPosition, _fvector vColor,_float fSize)
{
	m_pDeviceContext->GSSetShader(nullptr, nullptr, 0);

	m_pBatch->Begin();

	_float2 vOrigin = _float2(0.f, 0.f);
	//m_pFont->DrawString(m_pBatch, pText, vPosition, vColor);
	m_pFont->DrawString(m_pBatch, pText, vPosition, vColor, 0.f, vOrigin, fSize);
	m_pBatch->End();

	return S_OK;
}

CCustomFont * CCustomFont::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const _tchar * pFontFilePath)
{
	CCustomFont*	pInstance = new CCustomFont(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct(pFontFilePath)))
	{
		MSG_BOX(TEXT("Failed to Created CCustomFont"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCustomFont::Free()
{	
	Safe_Delete(m_pBatch);
	Safe_Delete(m_pFont);

	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
}
