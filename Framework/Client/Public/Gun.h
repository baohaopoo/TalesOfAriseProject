#pragma once

#include "Client_Defines.h"
#include "Equipment.h"
BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
END

BEGIN(Client)

class CGun final : public CEquipment
{

private:
	explicit CGun(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CGun(const CGun& rhs);
	virtual ~CGun() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;



public:
	void Set_SocketMatrix(const char* pBoneName);

private:
	ID3D11ShaderResourceView*		m_pMaskSRV = nullptr;


private:
	_uint	m_iIndex = 0;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();


public:
	static CGun* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END