#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CShader final : public CComponent
{
public:
	enum SHADER_TYPE { FX, CS, TYPE_END };
public:
	typedef struct tagPassDesc
	{
		ID3D11InputLayout*		pInputLayout = nullptr;
		ID3DX11EffectPass*		pEffectPass = nullptr;
	} PASSDESC;
private:
	CShader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CShader(const CShader& rhs);
	virtual ~CShader() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(const _tchar* pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements, SHADER_TYPE eType);
	virtual HRESULT NativeConstruct(void* pArg);

public:
	HRESULT Set_RawValue(const char* pConstantName, const void* pData, _uint iDataByteLength);
	HRESULT Set_ShaderResourceView(const char* pConstantName, ID3D11ShaderResourceView* pSRV);
	HRESULT Set_UnorderedAccessView(const char* pConstantName, ID3D11UnorderedAccessView* pUAV);
	HRESULT Begin(_uint iPassIndex);

private:
	ID3DX11Effect*				m_pEffect = nullptr;
	vector<PASSDESC*>			m_Passes;


public:
	static CShader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements, SHADER_TYPE eType = FX);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END