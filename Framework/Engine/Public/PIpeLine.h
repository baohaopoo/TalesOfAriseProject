#pragma once

#include "Base.h"

BEGIN(Engine)

/* 뷰변환행렬, 투영변환행렬을 보관한다. */
/* Tick하나더만들어서, 뷰변환행렬의 역행렬을 구하여 카메라의 위치르 정보도 보관한다. */

class CPipeLine final : public CBase
{
	DECLARE_SINGLETON(CPipeLine)
public:
	enum TRANSFORMSTATE { D3DTS_VIEW, D3DTS_PROJ, D3DTS_END };
public:
	CPipeLine();
	virtual ~CPipeLine() = default;

public:
	_matrix Get_TransformMatrix(TRANSFORMSTATE eState) {
		return XMLoadFloat4x4(&m_TransformMatrix[eState]);
	}

	_float4x4 Get_TransformFloat4x4_TP(TRANSFORMSTATE eState);

	_vector Get_CamPosition() {
		return XMLoadFloat4(&m_vCamPosition);
	}

	_float4 Get_CamPositionFloat4() {
		return m_vCamPosition;
	}

	_float Get_CamFar()
	{
		return m_fCamFar;
	}

public://for.light
	_matrix Get_LightTransformMatrix(TRANSFORMSTATE eState) {
		return XMLoadFloat4x4(&m_LightTransformMatrix[eState]);
	}

	_float4x4 Get_LightTransformFloat4x4_TP(TRANSFORMSTATE eState);

	_vector Get_LightPosition() {
		return XMLoadFloat4(&m_vLightPosition);
	}

	_float4 Get_LightPositionFloat4() {
		return m_vLightPosition;
	}
	_float Get_LightFar()
	{
		return m_fLightFar;
	}

public:
	HRESULT Set_Transform(TRANSFORMSTATE eState, _fmatrix TransformMatrix) {
		XMStoreFloat4x4(&m_TransformMatrix[eState], TransformMatrix);
		return S_OK;
	}
	HRESULT Set_CamFar(_float fFar)
	{
		m_fCamFar = fFar;
		return S_OK;
	}

	HRESULT Set_LightTransform(TRANSFORMSTATE eState, _fmatrix TransformMatrix) {
		XMStoreFloat4x4(&m_LightTransformMatrix[eState], TransformMatrix);
		return S_OK;
	}
	HRESULT Set_LightFar(_float fFar)
	{
		m_fLightFar = fFar;
		return S_OK;
	}

public:
	void Tick();

private:
	_float4x4			m_TransformMatrix[D3DTS_END];
	_float4				m_vCamPosition;
	_float				m_fCamFar = 0.f;

	_float4x4			m_LightTransformMatrix[D3DTS_END];
	_float4				m_vLightPosition;
	_float				m_fLightFar = 0.f;

public:
	virtual void Free() override;
};

END