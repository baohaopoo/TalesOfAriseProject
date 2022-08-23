#pragma once
#include "Client_Defines.h"
#include "UI.h"
#include "GameObject.h"
#include "GameInstance.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
END



BEGIN(Client)

class AutoTargetUI final : public CGameObject
{
private:
	AutoTargetUI(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	AutoTargetUI(const AutoTargetUI& rhs);
	virtual ~AutoTargetUI() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	bool m_isBattleOn = false;
	_float4x4 m_ProjMatrix;
	_float m_fX, m_fY;


	CTexture*			m_pTextureCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;

	_float m_fWinCX = 0.f, m_fWinCY = 0.f;

public:
	void BattleOn(bool what) { m_isBattleOn = what; }
public:
	HRESULT SetUp_Components();

public:
	static AutoTargetUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);

	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END