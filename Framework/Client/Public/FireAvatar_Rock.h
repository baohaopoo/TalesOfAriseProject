#pragma once
#include "Client_Defines.h"
#include "Equipment.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
END

BEGIN(Client)

class CFireAvatar_Rock final : public CEquipment
{
private:
	explicit CFireAvatar_Rock(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CFireAvatar_Rock(const CFireAvatar_Rock& rhs);
	virtual ~CFireAvatar_Rock() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();

public:
	_vector Get_Position() { return vPos; }

private:
	_float4x4			m_RockRightMatrix;
	_float4x4			m_RockLookMatrix;

private:
	_float4x4* m_pSocketMatrix;
	_vector vPos = { 0.f, 0.f, 0.f, 1.f };

public:
	_float4x4 Get_RealPosMtrix() { return RealPosMatrix; }

public:
	static CFireAvatar_Rock* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

private:
	_float4x4 RealPosMatrix;
};

END