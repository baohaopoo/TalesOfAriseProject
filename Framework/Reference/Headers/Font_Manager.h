#pragma once

#include "Base.h"

BEGIN(Engine)

class CFont_Manager final : public CBase
{
	DECLARE_SINGLETON(CFont_Manager)
private:
	CFont_Manager();
	virtual ~CFont_Manager() = default;

public:
	HRESULT Add_Fonts(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pFontTag, const _tchar* pFontFilePath);
	HRESULT Render_Fonts(const _tchar* pFontTag, const _tchar* pText, _float2 vPosition, _fvector vColor,_float fSize);

private:
	map<const _tchar*, class CCustomFont*>				m_Fonts;
	typedef map<const _tchar*, class CCustomFont*>		FONTS;

public:
	class CCustomFont* Find_Fonts(const _tchar* pFontTag);
public:
	virtual void Free() override;
};

END