#pragma once

#include "Component.h"
#include "DebugDraw.h"

BEGIN(Engine)

class ENGINE_DLL CCollider final : public CComponent
{
public:
	enum TYPE { TYPE_AABB, TYPE_OBB, TYPE_SPHERE, TYPE_END };

public:
	typedef struct tagColliderDesc
	{
		_float3			vPosition;
		_float3			vSize;
		_float			fRadius;
	}COLLIDERDESC;

	typedef struct tagObbDesc
	{
		_float3			vCenter;
		_float3			vAlignAxis[3];
		_float3			vCenterAxis[3];
	}OBBDESC;


public:
	CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CCollider(const CCollider& rhs);
	virtual ~CCollider() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(TYPE eType);
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	_bool Collsion_AABB(CCollider* pTargetCollider);
	_bool Collsion_OBB(CCollider* pTargetCollider);
	_bool Collsion_Sphere(CCollider * pTargetCollider, _float * pCollisionDistance = nullptr, _vector * vDir = nullptr);

	_bool Collision_Lay(_fvector vRayPos, _fvector vRayDir, _float& fDist);
	_bool Collision(CCollider* pTargetCollider);

	void Update(_fmatrix WorldMatrix);

	void Set(_fvector vPosition, _float fRadius)
	{
		XMStoreFloat3(&m_pSPHERE->Center, vPosition);
		m_pSPHERE->Radius = fRadius;
	}

	void Set_OBB(_fvector vPosition, _float3 vExtents)
	{
		XMStoreFloat3(&m_pOBB->Center, vPosition);
		m_pOBB->Extents = vExtents;
	}

	void Set_On(_bool bOn)
	{
		m_bOn = bOn;
	}
	_bool Get_On()
	{
		return m_bOn;
	}
	void Set_New(_bool bNew)
	{
		m_bNew = bNew;
	}
	_bool Get_New()
	{
		return m_bNew;
	}
	void Set_HitInfo(_uint iHitCount, _bool bStrongHit)
	{
		m_iHitCount = iHitCount;
		m_bStrongHit = bStrongHit;
	}

	_uint Get_HitCount()
	{
		return m_iHitCount;
	}
	_bool Get_StrongHit()
	{
		return m_bStrongHit;
	}

	BoundingSphere* Get_SPHEREInWolrd()
	{
		return m_pSPHEREInWorld;
	}



#ifdef _DEBUG
public:
	HRESULT Render();
#endif // _DEBUG

private:
	TYPE						m_eType = TYPE_END;
	_bool						m_isCollision = false;

	BoundingBox*				m_pAABB = nullptr;
	BoundingBox*				m_pAABBInWorld = nullptr;
	BoundingOrientedBox*		m_pOBB = nullptr;
	BoundingOrientedBox*		m_pOBBInWorld = nullptr;
	BoundingSphere*				m_pSPHERE = nullptr;
	BoundingSphere*				m_pSPHEREInWorld = nullptr;


private:
	BasicEffect*								m_pBasicEffect = nullptr;
	PrimitiveBatch<VertexPositionColor>*		m_pBatch = nullptr;
	ID3D11InputLayout*							m_pInputLayout = nullptr;


private:
	_matrix Remove_Rotation(_fmatrix TransformMatrix);
	_vector Compute_Min();
	_vector Compute_Max();
	OBBDESC Compute_OBB();


public:





private:
	_bool m_bOn = false;
	_bool m_bNew = false;
	_uint m_iHitCount = 0;
	_bool m_bStrongHit = false;

public:
	static CCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, TYPE eType);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END