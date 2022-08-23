#pragma once

#include "Client_Defines.h"
#include "Bullet.h"

BEGIN(Engine)
class CShader;
class CCollider;
class CRenderer;
class CModel;
END

BEGIN(Client)

class CDisplode final : public CBullet
{
public:
	typedef struct tagBulletDESC
	{
		class CModel*	pModelCom = nullptr;
		const char*		pBoneName = nullptr;	//뼈이름
		_vector			vTargetPos;				//몬스터위치

	}BULLETDESC;

private:
	explicit CDisplode(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CDisplode(const CDisplode& rhs);
	virtual ~CDisplode() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;


private:
	BULLETDESC			m_BulletDesc;
	_float4x4*			m_pSocketMatrix;
	_float4x4			m_PivotMatrix;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();
	_double m_Time = 0.0;
	_double m_BackTime = 0.0;



public:
	static CDisplode* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END