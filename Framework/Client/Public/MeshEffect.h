#pragma once
#include "Client_Defines.h"
#include "BlendObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CTexture;
class CModel;
END

BEGIN(Client)

class CMeshEffect  final : public CBlendObject
{
protected:
	CMeshEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CMeshEffect(const CMeshEffect& rhs);
	virtual ~CMeshEffect() = default;

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


	EFFECTDESC_MESH*	Get_EffectDesc_Mesh() { return &m_EffectDesc_Mesh; }
	_uint				Get_NumMeshContainers() { return m_iNumMeshContainers; }
	_float				Get_Time() { return m_Time; }
	_bool				Get_Finish() { return m_bFinish; }
	_float				Get_TimeSpeed() { return m_MultipleTime; }

	void				Set_ParentsMatrix(_matrix ParentsMatrix) { return XMStoreFloat4x4(&m_ParentsMatrix, ParentsMatrix); }
	void				Set_Time(_float Time) { m_Time = Time; }
	void				Set_TimeSpeed(_float TimeSpeed) { m_MultipleTime = TimeSpeed; }

private:
	CRenderer*		m_pRendererCom = nullptr;
	CShader*		m_pShaderCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;
	CModel*			m_pModel = nullptr;
	
	EFFECTDESC_MESH m_EffectDesc_Mesh;
	_uint			m_iNumMeshContainers = 1;
	_bool			m_bFinish = false;


	_float			m_Time = 0.f;
	_float			m_PassTime = 0;
	_float			m_MultipleTime = 1.f;
	_float			Alpha = 1.f;
	_float3			m_Shader;
	_float4x4		m_LocalMatrix;
	_float4x4		m_ParentsMatrix;
public:
	static CMeshEffect* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CBlendObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

//bool			bBillboard = false;
//unsigned int	iMesh = 0;
//unsigned int	iTexture = 0;
//unsigned int	iShader = 0;
//
//float			fMaxTime = 10.f;
//float			fMaxTimRend = 10.f;
//
//float			KeyFram_0_TimeEnd = 0.f;
//float			KeyFram_1_TimeEnd = 2.5f;
//float			KeyFram_2_TimeEnd = 5.f;
//float			KeyFram_3_TimeEnd = 7.5f;
//float			KeyFram_4_TimeEnd = 10.f;
//
//float			KeyFram_0_Alpha = 1.f;
//float			KeyFram_1_Alpha = 1.f;
//float			KeyFram_2_Alpha = 1.f;
//float			KeyFram_3_Alpha = 1.f;
//float			KeyFram_4_Alpha = 1.f;
//
//float			KeyFram_0_PositionRand = 1.f;
//float			KeyFram_1_PositionRand = 1.f;
//float			KeyFram_2_PositionRand = 1.f;
//float			KeyFram_3_PositionRand = 1.f;
//float			KeyFram_4_PositionRand = 1.f;
//
//float			KeyFram_0_RotationRand = 1.f;
//float			KeyFram_1_RotationRand = 1.f;
//float			KeyFram_2_RotationRand = 1.f;
//float			KeyFram_3_RotationRand = 1.f;
//float			KeyFram_4_RotationRand = 1.f;
//
//XMFLOAT3		KeyFram_0_Scale = XMFLOAT3(0.f, 0.f, 0.f);
//XMFLOAT3		KeyFram_1_Scale = XMFLOAT3(0.f, 0.f, 0.f);
//XMFLOAT3		KeyFram_2_Scale = XMFLOAT3(0.f, 0.f, 0.f);
//XMFLOAT3		KeyFram_3_Scale = XMFLOAT3(0.f, 0.f, 0.f);
//XMFLOAT3		KeyFram_4_Scale = XMFLOAT3(0.f, 0.f, 0.f);
//
//XMFLOAT3		KeyFram_0_Rotation = XMFLOAT3(0.f, 0.f, 0.f);
//XMFLOAT3		KeyFram_1_Rotation = XMFLOAT3(0.f, 0.f, 0.f);
//XMFLOAT3		KeyFram_2_Rotation = XMFLOAT3(0.f, 0.f, 0.f);
//XMFLOAT3		KeyFram_3_Rotation = XMFLOAT3(0.f, 0.f, 0.f);
//XMFLOAT3		KeyFram_4_Rotation = XMFLOAT3(0.f, 0.f, 0.f);
//
//XMFLOAT3		KeyFram_0_Position = XMFLOAT3(0.f, 0.f, 0.f);
//XMFLOAT3		KeyFram_1_Position = XMFLOAT3(0.f, 0.f, 0.f);
//XMFLOAT3		KeyFram_2_Position = XMFLOAT3(0.f, 0.f, 0.f);
//XMFLOAT3		KeyFram_3_Position = XMFLOAT3(0.f, 0.f, 0.f);
//XMFLOAT3		KeyFram_4_Position = XMFLOAT3(0.f, 0.f, 0.f);
//
//XMFLOAT4		vColor1 = XMFLOAT4(0.f, 0.f, 0.f, 0.f);
//XMFLOAT4		vColor2 = XMFLOAT4(0.f, 0.f, 0.f, 0.f);