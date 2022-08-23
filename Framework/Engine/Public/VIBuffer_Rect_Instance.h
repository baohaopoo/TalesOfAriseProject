#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Rect_Instance final : public CVIBuffer
{
protected:
	CVIBuffer_Rect_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CVIBuffer_Rect_Instance(const CVIBuffer_Rect_Instance& rhs);
	virtual ~CVIBuffer_Rect_Instance() = default;
public:
	typedef struct InstanceDesc
	{
		_float			fSpeed = 1.f;
		_float			fTime = 0.f;
		_float4			vMovement = _float4(0.f, 0.f, 1.f, 0.f);
	}INSTANCEDESC;

	typedef struct KeyFramDesc
	{
		_float			KeyFram_TimeEnd = 0.f;
		_float			KeyFram_RelaxSpeed = 1.f;
		_float			KeyFram_Speed = 0.f;
		_float			KeyFram_fRotation = 0.f;
		_float			KeyFram_fRelaxRotation = 0.f;
		_float4			KeyFram_Movement = _float4(0.f, 0.f, 0.f, 0.f);
	}KEYFRAMDESC;
	/*
	typedef struct EffectDesc
	{
	_bool			bBillboard = false;
	_uint			iTexture = 0;
	_uint			iShader = 0;
	_float			fLoop = 0.f;
	_float			fRotation = 0.f;
	_float			fMaxTime = 10.f;
	_float			fLastTime = 0;
	_float			fScale = 0.05f;
	_float			fScale_Speed = 5.f;
	_float			fSpeed = 1.f;
	_float			fSpeedRand = 1.f;

	_float			KeyFram_1_TimeEnd = 0.f;
	_float			KeyFram_1_RelaxSpeed = 1.f;
	_float			KeyFram_1_Speed = 0.f;
	_float			KeyFram_1_fRotation = 1.f;
	_float			KeyFram_1_fRelaxRotation = 0.f;

	_float			KeyFram_2_TimeEnd = 0.f;
	_float			KeyFram_2_RelaxSpeed = 1.f;
	_float			KeyFram_2_Speed = 0.f;
	_float			KeyFram_2_fRotation = 1.f;
	_float			KeyFram_2_fRelaxRotation = 0.f;

	_float			KeyFram_3_TimeEnd = 0.f;
	_float			KeyFram_3_RelaxSpeed = 1.f;
	_float			KeyFram_3_Speed = 0.f;
	_float			KeyFram_3_fRotation = 1.f;
	_float			KeyFram_3_fRelaxRotation = 0.f;

	_float			KeyFram_4_TimeEnd = 0.f;
	_float			KeyFram_4_RelaxSpeed = 1.f;
	_float			KeyFram_4_Speed = 0.f;
	_float			KeyFram_4_fRotation = 1.f;
	_float			KeyFram_4_fRelaxRotation = 0.f;

	_float4			KeyFram_1_Movement = _float4(0.f, 0.f, 0.f, 0.f);
	_float4			KeyFram_2_Movement = _float4(0.f, 0.f, 0.f, 0.f);
	_float4			KeyFram_3_Movement = _float4(0.f, 0.f, 0.f, 0.f);
	_float4			KeyFram_4_Movement = _float4(0.f, 0.f, 0.f, 0.f);

	_float4			vMovement = _float4(-0.5f, -0.5f, -0.5f, 0.f);
	_float4			vMovementRand = _float4(1.f, 1.f, 1.f, 0.f);
	_float4			vPostion = _float4(0.f, 0.f, 0.f, 0.f);
	_float4			vPostionRand = _float4(0.f, 0.f, 0.f, 0.f);
	_float4			vColor1 = _float4(0.f, 0.f, 0.f, 0.f);
	_float4			vColor2 = _float4(0.f, 0.f, 0.f, 0.f);
	}EFFECTDESC;
	*/
	//enum INSTANCETYPE { INSTANCETYPE_LOOP, INSTANCETYPE_DELETE, INSTANCETYPE_SPEED, DIRECTION_END };
	virtual HRESULT NativeConstruct_Prototype(_uint iNumInstance);
	virtual HRESULT NativeConstruct(void* pArg);

public:
	_bool Update(_double TimeDelta);
	_bool Update(_double TimeDelta,_matrix CamMatrix);
	HRESULT Reset();
	HRESULT KeyFramReset();
	HRESULT Restart();
public:
	virtual HRESULT Render() override;

	INSTANCEDESC*		Get_InstanceDesc() { return m_InstanceDesc; }
	EFFECTDESC*			Get_EffectDesc() { return &m_EffectDesc; }
	_bool	Get_Billboard() { return m_bBillboard; }
	_uint	Get_KeyFrameNum() { return m_iKeyFrame; }
	_float	Get_MaxTime() { return m_fMaxTime; }
	_float	Get_Time() { return m_fTime; }
private:
	ID3D11Buffer*			m_pVBInstance = nullptr;
	D3D11_BUFFER_DESC		m_VBInstDesc;
	D3D11_SUBRESOURCE_DATA	m_VBInstSubResourceData;

	_bool					m_finish = false;
	_bool					m_bBillboard = false;
	_uint					m_iInstanceStride = 0;
	_uint					m_iNumInstance = 0;
	_uint					m_iKeyFrame = 0;

	_float					m_fTime = 0.f;
	_float					m_fMaxTime = 10.f;
private:
	INSTANCEDESC*			m_InstanceDesc = nullptr;
	EFFECTDESC				m_EffectDesc;
	KEYFRAMDESC				m_KeyFramDesc;
public:
	static CVIBuffer_Rect_Instance* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iNumInstance = 1);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END

