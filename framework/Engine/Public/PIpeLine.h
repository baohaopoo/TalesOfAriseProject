#pragma once

#include "Base.h"

BEGIN(Engine)

/* �亯ȯ���, ������ȯ����� �����Ѵ�. */
/* Tick�ϳ�������, �亯ȯ����� ������� ���Ͽ� ī�޶��� ��ġ�� ������ �����Ѵ�. */

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

public:
	HRESULT Set_Transform(TRANSFORMSTATE eState, _fmatrix TransformMatrix) {
		XMStoreFloat4x4(&m_TransformMatrix[eState], TransformMatrix);
		return S_OK;
	}

public:
	void Tick();

private:
	_float4x4			m_TransformMatrix[D3DTS_END];
	_float4				m_vCamPosition;
public:
	virtual void Free() override;
};

END