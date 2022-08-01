#pragma once
#include "Client_Defines.h"
#include "Player.h"

BEGIN(Client)

class CAlphen final : public CPlayer
{
private:
	explicit CAlphen(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CAlphen(const CAlphen& rhs);
	virtual ~CAlphen() = default;

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
	static CAlphen* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

private:
	_uint m_NumAnim = 0;	// 애니메이션 총 갯수 저장하기 위한 변수

};

END