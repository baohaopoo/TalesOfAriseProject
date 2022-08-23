#pragma once

#include "Client_Defines.h"
#include "BlendObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CTexture;
class CVIBuffer_Rect_Instance;
END

BEGIN(Client)

class CRect_Effect final : public CBlendObject
{
private:
	explicit CRect_Effect(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CRect_Effect(const CRect_Effect& rhs);
	virtual ~CRect_Effect() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;


	HRESULT BufferSet();
	void	Reset_PassTime() { m_PassTime = m_EffectDesc->fPassTime; }

	EFFECTDESC*	Get_CreateDesc() { return m_EffectDesc; }
	_bool		Get_Finish() { return m_bFinish; }

	void Set_Pos(_float4 Pos) { m_EffectDesc->vPostion = Pos; }
	void				Set_TimeSpeed(_float TimeSpeed) { m_fmultipleTime = TimeSpeed; }
private:
	CRenderer*					m_pRendererCom = nullptr;
	CShader*					m_pShaderCom = nullptr;
	CTexture*					m_pTextureCom = nullptr;
	CVIBuffer_Rect_Instance*	m_pVIBufferCom = nullptr;

	_bool						m_bFinish = false;

	_float						m_fMaxTime = 10.f;
	_float						m_PassTime = 0;
	_float						m_fmultipleTime = 1.f;
	EFFECTDESC*					m_EffectDesc = nullptr;
private:
	HRESULT SetUp_Components(void* pArg);
	HRESULT SetUp_ConstantTable();

public:
	static CRect_Effect* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CBlendObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END