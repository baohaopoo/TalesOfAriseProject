#pragma once
#include "Client_Defines.h"
#include "Monster.h"



BEGIN(Client)

class CFire_Avatar final : public CMonster
{
private:
	explicit CFire_Avatar(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CFire_Avatar(const CFire_Avatar& rhs);
	virtual ~CFire_Avatar() = default;

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
	static CFire_Avatar* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;


};

END