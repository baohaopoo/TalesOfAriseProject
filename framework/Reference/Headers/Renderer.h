#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CRenderer final : public CComponent
{
public:
	enum RENDERGROUP { GROUP_PRIORITY, GROUP_TERRAIN, GROUP_NONBLEND, GROUP_NONLIGHT, GROUP_ALPHABLEND, GROUP_UI, GROUP_END };
private:
	explicit CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CRenderer() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
public:
	HRESULT Add_RenderGroup(RENDERGROUP eGroup, class CGameObject* pGameObject);
	HRESULT Render_GameObjects();
#ifdef _DEBUG
public:
	HRESULT Add_DebugComponent(class CComponent* pComponent);
#endif // _DEBUG
private:
	list<class CGameObject*> m_RenderList[GROUP_END];
	typedef list<class CGameObject*> RENDERLIST;
private:
	list<class CComponent*> m_DebugComponents;
	typedef list<class CComponent*> DEBUGCOMPONENTS;
private:
	class CTarget_Manager* m_pTarget_Manager = nullptr;
	class CLight_Manager* m_pLight_Manager = nullptr;
private:
	class CShader* m_pShader = nullptr;
	class CVIBuffer_Rect* m_pVIBuffer = nullptr;
	_float4x4 m_WorldMatrix, m_ViewMatrix, m_ProjMatrix;
#ifdef _DEBUG
private:
	HRESULT SetUp_RenderState();
#else
private:
	HRESULT SetUp_RenderState();
#endif // _DEBUG

private:
	HRESULT Render_Priority();
	HRESULT Render_Terrain();
	HRESULT Render_NonBlend();
	HRESULT Render_LightAcc();
	HRESULT Render_Deferred();
	HRESULT Render_NonLight();
	HRESULT Render_Blend();
	HRESULT Render_UI();
#ifdef _DEBUG
private:
	HRESULT Render_DebugComponents();
#endif // _DEBUG

#ifdef _DEBUG
private:
	ID3D11RasterizerState* m_pWireFrame = nullptr;
#endif // _DEBUG
private:
	ID3D11RasterizerState* m_pRasterDefault = nullptr;
public:
	static CRenderer* Create(ID3D11Device* ppDeviceOut, ID3D11DeviceContext* ppDeviceContextOut);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END

