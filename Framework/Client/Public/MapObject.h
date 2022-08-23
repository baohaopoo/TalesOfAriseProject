#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
class CCollider;
END

BEGIN(Client)

class CMapObject : public CGameObject {

public:
	typedef struct _tagMapObjectDesc {
		const _tchar*	pPrototype_ObjectName = nullptr;	// 프로토타입 오브젝트 태그
		_float4x4	TransformMatrix;	// 이동 행렬
		_float3		fColliderSize;		// 충돌체의 사이즈
		_bool		m_bMap = false;
		LEVEL		eLevel;
	}MAPOBJDesc;

protected:
	explicit CMapObject(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CMapObject(const CMapObject& rhs);
	virtual ~CMapObject() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	CCollider* Get_Collider(void) { return m_pSphereCom; }

protected:
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CModel*				m_pModelCom = nullptr;
	CCollider*			m_pSphereCom = nullptr;

protected:
	HRESULT SetUp_Components(void* pArg);
	HRESULT SetUp_ConstantTable();

public:
	const _float3 Get_Rotation(void) { return m_fRotation; }
	void Set_Rotation(_float3 fRotation) { m_fRotation = fRotation; }
	const TCHAR* Get_ModelTag(void) { return m_pModelTag; }
	_bool isMap(void) { return m_bMap; }


private:
	TCHAR m_pModelTag[MAX_PATH];
	_float3 m_fRotation = _float3{ 0.f, 0.f, 0.f };
	_bool m_bMap = false;

public:
	static CMapObject* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

public:
	_uint Get_FeatureNumber(void) { return m_iFeatureMeshNumber; }

private:
	_uint m_iFeatureMeshNumber = 0;

private:
	static _uint iFeatureMeshNumber;	// 배치된 메쉬의 총 갯수
};

END