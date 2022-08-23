#pragma once

#include "Component.h"

BEGIN(Engine)

class CNavigation;

END

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
	HRESULT Go_Straight(_double TimeDelta, CNavigation* pNavigation = nullptr);
	HRESULT Go_Left(_double TimeDelta);
	HRESULT Go_Right(_double TimeDelta);
	HRESULT Go_Down(_double TimeDelta);
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
	HRESULT Look(_fvector vLookk);				// 몬스터가 플레이어를 쳐다보도록 Look방향을 바꾸는 함수
	HRESULT LookAtReverse(_fvector vTargetPos);
	HRESULT Scaled(const _float3& vScale);
	HRESULT Scaling(_float fTimeDelta);
	HRESULT Turn(_fvector vAxis, _double TimeDelta);
	HRESULT Rotation(_fvector vAxis, _double Radian);
	HRESULT RotationXYZ(_double RadianX, _double RadianY, _double RadianZ);
	HRESULT TurnAxis(_fvector vAxis);
	HRESULT TurnReverseAxis(_fvector vAxis);
	HRESULT Chase(_fvector vTargetPos, _double TimeDelta);
	HRESULT Set_WorldMatrix(_fmatrix WorldMatrix);
	HRESULT Check_Right_Left(_fvector vTargetLook, _double TimeDelta);
	_bool FrontPlayer(_fvector vTargetPos);				// 적 기준으로 플레이어가 앞에서 때리는지 뒤에서 때리는지 판별하기 위한 함수
	_bool LeftPlayer(_fvector vTargetPos);				// 적 기준으로 플레이어가 왼쪽에 있는지 오른쪽에 있는지 판별하기 위한 함수


	_float Get_Height(CNavigation* pNavigation);
	void Set_Height(_float fHeight);
	HRESULT Move(_float3 fPos);									// 인자로 받은 위치로 이동하는 함수		
	HRESULT Move(_float fPosX, _float fPosY, _float fPosZ);



public:
	HRESULT Go_Straight_PlaneXZ(_double TimeDelta);
	HRESULT Go_BackWard_PlaneXZ(_double TimeDelta);
	HRESULT TurnByRadian(_fvector vAxis, _double Radian);

public:
	_double Get_Speed(void) { return m_TransformDesc.SpeedPerSec; }
	void Set_Speed(_double dSpeed) { m_TransformDesc.SpeedPerSec = dSpeed; };

public:
	const TRANSFORMDESC& Get_TransformDesc(void) { return m_TransformDesc; }
	void Set_TransformDesc(TRANSFORMDESC TransformDesc) { m_TransformDesc = TransformDesc; }

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