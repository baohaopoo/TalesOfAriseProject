#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTexture final : public CComponent
{
private:
	CTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CTexture(const CTexture& rhs);
	virtual ~CTexture() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(const _tchar* pTextureFilePath, _uint iNumTextures);
	virtual HRESULT NativeConstruct(void* pArg);

public:
	HRESULT SetUp_ShaderResourceView(class CShader* pShader, const char* pConstantName, _uint iTextureIndex);

private:	
	vector<ID3D11ShaderResourceView*>				m_Textures;
	typedef vector<ID3D11ShaderResourceView*>		TEXTURES;

public:
	static CTexture* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pTextureFilePath, _uint iNumTextures = 1); 
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END