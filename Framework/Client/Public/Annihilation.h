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

class CAnnihilation final : public CBullet
{
public:
	typedef struct tagAnnihilationDESC
	{
		class CModel*	pModelCom = nullptr;
		const char*		pBoneName = nullptr;	//ª¿¿Ã∏ß
		CTransform*		pTargetTransform = nullptr;

	}ANNIHILATIONDESC;
private:
	explicit CAnnihilation(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CAnnihilation(const CAnnihilation& rhs);
	virtual ~CAnnihilation() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:




private:
	ANNIHILATIONDESC			m_BulletDesc;
	_float4x4*			m_pSocketMatrix;
	_float4x4			m_PivotMatrix;


private:
	ID3D11ShaderResourceView*		m_pMaskSRV = nullptr;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();
	_double m_Time = 0.0;
	_double m_BackTime = 0.0;



public:
	static CAnnihilation* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END