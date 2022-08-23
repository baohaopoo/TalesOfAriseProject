#pragma once
#include "Base.h"

BEGIN(Engine)

class CPostProcessing final : public CBase
{
	DECLARE_SINGLETON(CPostProcessing)
public:
	enum DOWNSCALE_TYPE { TEMP, BLUR, EMISSIVE, TYPE_END };
private:
	CPostProcessing();
	virtual ~CPostProcessing() = default;
public:
	_float Get_MiddleGray();
	_float Get_White();
	ID3D11ShaderResourceView* Get_DownScale4x4SRV(DOWNSCALE_TYPE eType);
	ID3D11UnorderedAccessView* Get_DownScale4x4UAV(DOWNSCALE_TYPE eType);
public:
	void Set_MiddleGray(_float fMiddleGray);
	void Set_White(_float fWhite);
	void Set_RadialBlur(_bool bRadialBlur);
	void Set_RadialBlurLength(_float fLength);
	void Set_RadialNlurStrength(_float fStrength);
public:
	HRESULT Init_PostProcessing(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	HRESULT Combine_Resources(class CVIBuffer_Rect* pVIBuffer, _float4x4* pMatrix);
	HRESULT DownScale_Blur(ID3D11ShaderResourceView* pInput, DOWNSCALE_TYPE eType);
private:
	HRESULT DownScale();
	HRESULT Bloom();
	HRESULT Blur(ID3D11ShaderResourceView* pInput, ID3D11UnorderedAccessView* pOutput);
	HRESULT FinalPass(class CVIBuffer_Rect* pVIBuffer, _float4x4* pMatrix);
#ifdef _DEBUG
	HRESULT TestPass(class CVIBuffer_Rect* pVIBuffer, _float4x4* pMatrix, ID3D11ShaderResourceView* pSRV);
#endif // _DEBUG
public:
	HRESULT DownScale4x4(ID3D11ShaderResourceView* pInput, ID3D11UnorderedAccessView* pOutput);
	HRESULT Blur_Object(ID3D11ShaderResourceView* pInput, ID3D11UnorderedAccessView* pOutput);
private:
	HRESULT SetUp_Shader();
	HRESULT SetUp_Texture_Buffer();
private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;
private:
	class CShader* m_pShader_PostProcessing_FX = nullptr;
	class CShader* m_pShader_DownScale_CS = nullptr;
	class CShader* m_pShader_Blur_CS = nullptr;
private:
	class CTarget_Manager* m_pTarget_Manager = nullptr;
private:
	ID3D11ShaderResourceView* m_pClearSRV[128] = { nullptr };
	ID3D11UnorderedAccessView* m_pClearUAV[128] = { nullptr };
private:
	ID3D11Texture2D* m_pDownScaleRTV = nullptr;
	ID3D11ShaderResourceView* m_pDownScaleSRV = nullptr;
	ID3D11UnorderedAccessView* m_pDownScaleUAV = nullptr;
private:
	ID3D11Texture2D* m_pTempRTV[2] = { nullptr };
	ID3D11ShaderResourceView* m_pTempSRV[2] = { nullptr };
	ID3D11UnorderedAccessView* m_pTempUAV[2] = { nullptr };
private:
	ID3D11Texture2D* m_pBloomRTV = nullptr;
	ID3D11ShaderResourceView* m_pBloomSRV = nullptr;
	ID3D11UnorderedAccessView* m_pBloomUAV = nullptr;
private:
	ID3D11Buffer* m_pDownScale1DBuffer = nullptr;
	ID3D11UnorderedAccessView* m_pDownScale1DUAV = nullptr;
	ID3D11ShaderResourceView* m_pDownScale1DSRV = nullptr;
private:
	ID3D11Buffer* m_pAvgLumBuffer = nullptr;
	ID3D11UnorderedAccessView* m_pAvgLumUAV = nullptr;
	ID3D11ShaderResourceView* m_pAvgLumSRV = nullptr;
private:
	ID3D11Buffer* m_pPrevAvgLumBuffer = nullptr;
	ID3D11UnorderedAccessView* m_pPrevAvgLumUAV = nullptr;
	ID3D11ShaderResourceView* m_pPrevAvgLumSRV = nullptr;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
private:
	ID3D11Texture2D* m_pDownScale4x4RTV[TYPE_END] = { nullptr };
	ID3D11ShaderResourceView* m_pDownScale4x4SRV[TYPE_END] = { nullptr };
	ID3D11UnorderedAccessView* m_pDownScale4x4UAV[TYPE_END] = { nullptr };
private:
	_uint m_iWidth = 0, m_iHeight = 0, m_iDownScaleGroups = 0;
	_float m_fMiddleGrey = 0.263f, m_fWhite = 1.53f;
	_float m_fAdaptation = 1.f;
	_float m_fBloomThreshold = 1.1f;
	_float m_fBloomScale = 0.74f;
private:
	_bool m_bRadialBlur = false;
	_float m_fRadialLength = 0.4f;
	_float m_fRadialStrength = 7.f;
public:
	virtual void Free() override;
};

END