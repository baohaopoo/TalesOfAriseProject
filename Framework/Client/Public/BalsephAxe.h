#pragma once

#include "Client_Defines.h"
#include "Equipment.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
END

BEGIN(Client)

class CBalsephAxe final : public CEquipment
{
public:
	typedef struct tagSocketDesc
	{
		class CModel*	pModelCom = nullptr;
		const char*		pBoneName = nullptr;
		CTransform* pTargetTransform = nullptr;
	}AXEDESC;

private:
	explicit CBalsephAxe(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CBalsephAxe(const CBalsephAxe& rhs);
	virtual ~CBalsephAxe() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();


private:
	AXEDESC m_pAxeDesc;

public:
	static CBalsephAxe* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END