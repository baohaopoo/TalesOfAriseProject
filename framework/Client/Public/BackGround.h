#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CBackGround final : public CGameObject
{
private:
	explicit CBackGround(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CBackGround(const CBackGround& rhs);
	virtual ~CBackGround() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	CTexture*			m_pTextureCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;


public:
	typedef struct tagBackground
	{
		_float				m_fX, m_fY, m_fSizeX, m_fSizeY;
		int kind;

	}BACKDESC;

	BACKDESC m_tUIInfo;

public:
	_float UIKind = 0;
private:
	_float4x4			m_ProjMatrix;

	//_float				m_fX, m_fY, m_fSizeX, m_fSizeY;

public:
	void ChangeCut(bool what) { m_isChangeCut = what; }

private:
	//fade in out 용
	_float controlValv = 0;
	float rgb = 0;
	//한번만 fade in out 하도록 하려고 함.
	bool m_isFirst = false;
	bool m_isChangeCut = false;
	_uint m_iSprite = 0, m_iShaderPass = 0;

public:
	HRESULT SetUp_Components();

public:
	static CBackGround* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END