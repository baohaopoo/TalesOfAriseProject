#pragma once

#include "Base.h"
#include "Model.h"
#include "Renderer.h"
#include "Transform.h"
#include "Shader.h"
#include "VIBuffer_Rect.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
public:
	enum OBJTYPE { OBJTYPE_PLAYER, OBJTYPE_MONSTER, OBJTYPE_END };
protected:
	explicit CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;

public:
	class CComponent* Get_Component(const _tchar* pComponentTag) {
		return Find_Component(pComponentTag);
	}

	void Delete_Component(const _tchar* pComponentTag);

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg, CTransform::TRANSFORMDESC* pTransformDesc = nullptr);
	virtual void Tick(_double TimeDelta);
	virtual void LateTick(_double TimeDelta);
	virtual HRESULT Render() = 0;
	virtual HRESULT Render_LightDepth();

public:
	bool	Get_Dead(void) const { return m_bDead; }
	void	Set_Dead(bool _bDead) { m_bDead = _bDead; }
	void	Set_Time(_double dTimeSpeed) { m_dTimeSpeed = dTimeSpeed; }
	enum OBJTYPE Get_PhysXType() { return PhysXType; }
	_float Get_DistanceToCamera();
protected:
	OBJTYPE PhysXType = OBJTYPE_END;
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;

protected:
	static const _tchar*	m_pTransformTag;
	class CTransform*		m_pTransformCom = nullptr;
	_float m_fDistanceToCamera = 0.f;
protected:
	map<const _tchar*, class CComponent*>			m_Components;
	typedef map<const _tchar*, class CComponent*>	COMPONENTS;

protected:
	HRESULT SetUp_Components(const _tchar* pComponentTag, _uint iLevelIndex, const _tchar* pPrototypeTag, CComponent** ppOut, void* pArg = nullptr);

private:
	CComponent* Find_Component(const _tchar* pComponentTag);


public:	
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;

protected:
	bool		m_bDead = false;
	_double		m_dTimeSpeed = 1.0;
};

END