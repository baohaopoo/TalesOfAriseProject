#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CCollider;
class CRenderer;
class CModel;
END

BEGIN(Client)

class CPlayer abstract : public CGameObject
{
protected:
	CPlayer(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg, CTransform::TRANSFORMDESC* pTransformDesc);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

protected:
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CModel*				m_pModelCom = nullptr;
	CCollider*			m_pAABBCom = nullptr;
	CCollider*			m_pSphereCom = nullptr;

public:
	virtual CGameObject* Clone(void* pArg) override = 0;
	virtual void Free() override;

protected:
	_bool m_bStopAnimation = false;
	_bool m_bLoop = true;
	_double m_fAttackSpeed = 1.0;
	_double m_iLinearTime = 5.0;		// 선형보간 Term식
	_uint m_iCurrentAnimationIndex = 0;
	_uint m_iNextAnimationIndex = 0;
};

END