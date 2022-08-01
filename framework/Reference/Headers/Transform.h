#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };

public:
	typedef struct tagTransformDesc
	{
		_double			SpeedPerSec;
		_double			RotationPerSec;
		_vector			fScalePerSec;
	}TRANSFORMDESC;

private:
	CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CTransform() = default;

public: // Getter 
	_vector Get_State(STATE eState) {
		return XMLoadFloat4((_float4*)&m_WorldMatrix.m[eState][0]);
	}

	_float3 Get_Scale();

	_matrix Get_WorldMatrix();
	_matrix Get_WorldMatrixInverse();
	_float4x4 Get_WorldFloat4x4();
	_float4x4 Get_WorldFloat4x4_TP();


public: // Setter 
	void Set_State(STATE eState, _fvector vState) {
		XMStoreFloat4((_float4*)&m_WorldMatrix.m[eState][0], vState);
	}

public:
	virtual HRESULT NativeConstruct_Prototype(void* pArg = nullptr);

public:
	HRESULT Bind_WorldMatrixOnShader(class CShader* pShader, const char* pConstantName);

public:
	HRESULT Go_Straight(_double TimeDelta);
	HRESULT Go_Left(_double TimeDelta);
	HRESULT Go_Right(_double TimeDelta);
	HRESULT Go_CircleLeft(_double TimeDelta, _float radius);
	HRESULT Go_TurnYAxis(_double TimeDelta);
	HRESULT Go_CircleNtropyLeft(_double TimeDelta, _float radius);
	_vector CirclePos(_double TimeDelta, _float radius);
	HRESULT Go_CircleRight(_double TimeDelta, _float radius);
	HRESULT Go_CircleCameraLeft(_double TimeDelta, _float radius);
	HRESULT Go_BackWard(_double TimeDelta);
	_bool Distance(_fvector vTargetPos, _float fLimitDistance);
	_bool FollowingDistance(_fvector vTargetPos, _float fLimitDistance);
	HRESULT TurnBack(_fvector vAxis);
	HRESULT LookAt(_fvector vTargetPos);
	HRESULT LookAtReverse(_fvector vTargetPos);
	HRESULT Scaled(const _float3& vScale);
	HRESULT Scaling(_float fTimeDelta);
	HRESULT Turn(_fvector vAxis, _double TimeDelta);
	HRESULT Rotation(_fvector vAxis, _double Radian);
	HRESULT TurnAxis(_fvector vAxis);
	HRESULT TurnReverseAxis(_fvector vAxis);
	HRESULT Chase(_fvector vTargetPos, _double TimeDelta);
	HRESULT Set_WorldMatrix(_fmatrix WorldMatrix);
private:
	_float4x4				m_WorldMatrix;
	TRANSFORMDESC			m_TransformDesc;

private:
	class CPipeLine* m_pPipeLine = nullptr;

public:
	static CTransform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END