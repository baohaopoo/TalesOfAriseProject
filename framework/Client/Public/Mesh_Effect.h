#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CTexture;
class CModel;
END

BEGIN(Client)

class CMesh_Effect  final : public CGameObject
{
protected:
	CMesh_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CMesh_Effect(const CMesh_Effect& rhs);
	virtual ~CMesh_Effect() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta);
	virtual void LateTick(_double TimeDelta);
	virtual HRESULT Render();
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();
	void	Reset();
	void	KeyFram_Reset();
	void	Reset_PassTime() { m_PassTime = m_EffectDesc_Mesh.fPassTime; }

	EFFECTDESC_MESH*	Get_EffectDesc_Mesh() { return &m_EffectDesc_Mesh; }
	_uint				Get_NumMeshContainers() { return m_iNumMeshContainers; }
	_float				Get_Time() { return m_Time; }
	_bool				Get_Finish() { return m_bFinish; }

	void				Set_ParentsMatrix(_matrix ParentsMatrix) { return XMStoreFloat4x4(&m_ParentsMatrix, ParentsMatrix); }
	void				Set_Time(_float Time) { m_Time = Time; }


private:
	CRenderer*		m_pRendererCom = nullptr;
	CShader*		m_pShaderCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;
	CModel*			m_pModel = nullptr;

	EFFECTDESC_MESH m_EffectDesc_Mesh;
	_uint			m_iNumMeshContainers = 1;
	_bool			m_bFinish = false;
	
	_int			m_i = 1;
	_float			m_Time = 0.f;
	_float			m_PassTime = 0;
	_float			Alpha = 1.f;
	_float3			m_Shader;
	_float4x4		m_LocalMatrix;
	_float4x4		m_ParentsMatrix;
public:
	static CMesh_Effect* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
