#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "GameInstance.h"

BEGIN(Engine)
class CShader;
class CCollider;
class CRenderer;
class CModel;
END

BEGIN(Client)

class CPlayer;

// ĳ���Ͱ� ��������Ʈ �̿��, �ش� ��ü�� ���� ��ġ�� �̵��ϰ�, ĳ������ ī�޶�� m_vCameraLook������ �ٶ󺸰� �Ѵ�.

class CWayPoint final : public CGameObject
{
public:
	typedef struct _tagWayPoint_DESC {
		_float3		fPos;			// ���� ����Ʈ�� ��ġ
		_float3		fDir;			// ī�޶��� ����
		_uint		iPointNumber;	// ���� ����Ʈ�� ��ȣ, �ش� ��ȣ�� ���� ����Ʈ�� ã�´�.
	}WayPoint_DESC;

private:
	CWayPoint(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CWayPoint(const CWayPoint& rhs);
	virtual ~CWayPoint() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta);
	virtual void LateTick(_double TimeDelta);
	virtual HRESULT Render();

private:
	HRESULT SetUp_Components(void* pArg);
	HRESULT SetUp_ConstantTable();

private:
	CTexture*			m_pTextureCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CModel*				m_pModelCom = nullptr;
	CCollider*			m_pSphereCom = nullptr;

public:
	HRESULT Activate_WayPoint(CPlayer* pPlayer);

public:
	void Set_Destination(CWayPoint* pDestWayPoint) { m_pDestWayPoint = pDestWayPoint; }
	CWayPoint* Get_Destination(void) { return m_pDestWayPoint; }
	_float3 Get_CameraLook(void) { return m_fCameraLook; }
	_uint	Get_WayPointNumber(void) { return m_iWayPointNumber; }

private:
	CWayPoint*	m_pDestWayPoint = nullptr;		// �̵��� ���� ����Ʈ ����
	_float3		m_fCameraLook;					// �̵��� ī�޶��� ����
	_uint		m_iWayPointNumber = 0;			// ���� ��ȣ, ���° ���� ����Ʈ���� �˼� �ִ�.

public:
	static CWayPoint* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END