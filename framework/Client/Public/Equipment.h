#pragma once

#include "Client_Defines.h"
#include "GameObject.h"


BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
END

BEGIN(Client)
class CEquipment abstract : public CGameObject
{
public:		// protected로 하면 Level_Tutorial 등의 클래스에서 소켓 추가할 때 SocketDesc을 받을 수가 없다.
	typedef struct tagSocketDesc
	{
		class CModel*	pModelCom = nullptr;
		const char*		pBoneName = nullptr;
	}SOCKETDESC;

protected:
	CEquipment(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	CEquipment(const CEquipment& rhs);
	virtual ~CEquipment() = default;
	
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

protected:
	SOCKETDESC			m_SocketDesc;
	_float4x4*			m_pSocketMatrix;
	_float4x4			m_PivotMatrix;

public:
	virtual CGameObject* Clone(void* pArg) override = 0;
	virtual void Free() override;

};

END