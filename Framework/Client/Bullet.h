#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CCollider;
class CRenderer;
class CModel;
END

class CBullet abstract : public CGameObject
{


protected:
	CBullet(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	CBullet(const CBullet& rhs);
	virtual ~CBullet() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg, CTransform::TRANSFORMDESC* pTransformDesc);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;


protected:


protected:
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CModel*				m_pModelCom = nullptr;
	CTexture*			m_pTexureCom = nullptr;
	CCollider*			m_pAttackOBBCom = nullptr;
	CCollider*			m_pAttackSphereCom = nullptr;

public:
	virtual CGameObject* Clone(void* pArg) override = 0;
	virtual void Free() override;


protected:
	_double 	m_DeadTime = 0.0;
	_double 	m_LiveTime = 0.0;


};

