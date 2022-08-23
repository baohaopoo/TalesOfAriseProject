#pragma once

#include "Graphic_Device.h"
#include "Input_Device.h"
#include "Level_Manager.h"
#include "Object_Manager.h"
#include "Component_Manager.h"
#include "Timer_Manager.h"
#include "PIpeLine.h"
#include "Light_Manager.h"
#include "Font_Manager.h"
#include "Frustum.h"
#include "Target_Manager.h"
#include "PhysX.h"
#include "Picking.h"
//#include "Sound_Manager.h"
#include "PostProcessing.h"

#ifdef _DEBUG
#include "Console_Debug.h"
#endif // _DEBUG

/* 클라이언트에 보여줘야할 인터페이스를 보관하고. 보여준다. */

BEGIN(Engine)

class ENGINE_DLL CGameInstance : public CBase {

	DECLARE_SINGLETON(CGameInstance)

private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public: /* For.GameInstance */
	HRESULT Initialize_Engine(HINSTANCE hInst, _uint iNumLevels, CGraphic_Device::GRAPHICDESC GraphicDesc, ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppDeviceContextOut);
	void Tick_Engine(_double TimeDelta);
	HRESULT Render_Engine();
	HRESULT Render_Begin(_float4 vClearColor);
	HRESULT Render_End();
	HRESULT Clear_LevelResource(_uint iLevelIndex);

public: /* For.Graphic_Device */


public: /* For.Input_Device */
	_char Get_DIKeyState(_uchar byKeyID);
	_char Get_DIMButtonState(CInput_Device::MOUSEBUTTONSTATE eDIMBState);
	_long Get_DIMMoveState(CInput_Device::MOUSEMOVESTATE eDIMMState);

public: /* For.Timer_Manager */
	HRESULT Add_Timers(const _tchar* pTimerTag);
	_double Compute_TimeDelta(const _tchar* pTimerTag);

//public://For Sound_Manager
//	HRESULT Init_Sound(const _tchar* pSoundFilePath);
//	void Play_Sound(TCHAR* pSoundKey, CSound_Manager::CHANNELID eID, _float _vol);

public: /* For.Level_Manager */
	HRESULT Open_Level(_uint iLevelIndex, class CLevel* pLevel);
	_uint Get_LevelIndex() { return m_iLevel; }

public: /* For.Object_Manager */
	class CComponent* Get_Component(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pComponentTag, _uint iIndex = 0);
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, class CGameObject* pPrototype);

	CGameObject* Add_GameObjectToLayer(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pPrototypeTag, void* pArg = nullptr);
	void Delete_Component(const _tchar* pComponentTag);
	CLayer* Find_Layer(_uint iLevelIndex, const _tchar* pLayerTag);
	CGameObject* Get_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, _uint iIndex = 0);
	CLayer* Add_Layer(_uint iLevelIndex, const _tchar * pLayerTag);

public: /* For.Component_Manager */
	HRESULT Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, CComponent* pPrototype);
	CComponent* Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg);

public: /* For.PipeLine */
	_matrix Get_TransformMatrix(CPipeLine::TRANSFORMSTATE eState);
	_float4x4 Get_TransformFloat4x4_TP(CPipeLine::TRANSFORMSTATE eState);
	_vector Get_CamPosition();
	_float4 Get_CamPositionFloat4();
	_float Get_CamFar();

	_matrix Get_LightTransformMatrix(CPipeLine::TRANSFORMSTATE eState);
	_float4x4 Get_LightTransformFloat4x4_TP(CPipeLine::TRANSFORMSTATE eState);
	_vector Get_LightPosition();
	_float4 Get_LightPositionFloat4();
	_float Get_LightFar();

public: /*For.Light_Manager */
	const LIGHTDESC* Get_LightDesc(_uint iIndex);
	HRESULT Add_Lights(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const LIGHTDESC& LightDesc);

public: /*For.Font_Manager */
	HRESULT Add_Fonts(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pFontTag, const _tchar* pFontFilePath);
	HRESULT Render_Fonts(const _tchar* pFontTag, const _tchar* pText, _float2 vPosition, _fvector vColor, _float fSize = 1.f);

public: /*For.Frustum */
	_bool isInFrustum_WorldSpace(_vector vWorldPos, _float fRange = 0.f);
	_bool isIn_LocalSpace(CTransform* pTransform, _float fRange = 0.f);

public: /* for.Target_Manager */
	ID3D11ShaderResourceView* Get_RenderTargetSRV(const _tchar* pTargetTag) const;

public: /*For. Input_Device -> Key_Manager*/
	_bool Key_Pressing(_uchar byKeyID);
	_bool Key_Down(_uchar byKeyID);
	_bool Key_Up(_uchar byKeyID);
	_bool Button_Pressing(CInput_Device::MOUSEBUTTONSTATE eDIMBState);
	_bool Button_Down(CInput_Device::MOUSEBUTTONSTATE eDIMBState);
	_bool Button_Up(CInput_Device::MOUSEBUTTONSTATE eDIMBState);

public: /*For.Picking_Manager */
	_bool Picking(_float3* pVerticesPos, _fmatrix WorldMatrix, _vector* pOut);
	_bool Picking(_fvector vPos1, _fvector vPos2, _fvector vPos3, _fmatrix WorldMatrix, _vector* pOut);
	_bool Picking(_fvector vPos1, _fvector vPos2, _fvector vPos3, _fmatrix WorldMatrix, _vector* pPos, _vector* pDistance);
	const CPicking_Manager::RAYDESC Get_vRayDesc(void);

public:/*For.PostProcessing */
	void Set_RadialBlur(_bool bRadialBlur);
	void Set_RadialBlurLength(_float fLength);
	void Set_RadialNlurStrength(_float fStrength);
#ifdef _DEBUG
public://For Console_Debug
	HRESULT Init_Console_Debug(_bool bInit);
	_bool Get_WireFrame();
#endif // _DEBUG

public://For Effect
	void LoadDataFile_Effect();
	EFFECTDESC* Get_InstanceEffect_Data(UNIT_TYPE type,_int index);
	EFFECTDESC_MESH* Get_MeshEffect_Data(UNIT_TYPE type, _int index);
private:
	CGraphic_Device*			m_pGraphic_Device = nullptr;
	CInput_Device*				m_pInput_Device = nullptr;
	CLevel_Manager*				m_pLevel_Manager = nullptr;
	CObject_Manager*			m_pObject_Manager = nullptr;
	CComponent_Manager*			m_pComponent_Manager = nullptr;
	CTimer_Manager*				m_pTimer_Manager = nullptr;
	CPipeLine*					m_pPipeLine = nullptr;
	CLight_Manager*				m_pLight_Manager = nullptr;
	CFont_Manager*				m_pFont_Manager = nullptr;
	CFrustum*					m_pFrustum = nullptr;
	CTarget_Manager*			m_pTarget_Manager = nullptr;
	CPhysX*						m_pPhysX = nullptr;
	CPicking_Manager*			m_pPicking_Manager = nullptr;
	CPostProcessing*			m_pPostProcessing = nullptr;
	_uint						m_iLevel = 0;

#ifdef _DEBUG
	CConsole_Debug* m_pConsole_Debug = nullptr;
#endif // _DEBUG

	vector<EFFECTDESC> vInstanceEffectDesc_Monster;
	vector<EFFECTDESC_MESH> vMeshEffectDesc_Monster;

	vector<EFFECTDESC> vInstanceEffectDesc_Boss;
	vector<EFFECTDESC_MESH> vMeshEffectDesc_Boss;

	vector<EFFECTDESC> vInstanceEffectDesc_Alphen;
	vector<EFFECTDESC_MESH> vMeshEffectDesc_Alphen;

	vector<EFFECTDESC> vInstanceEffectDesc_Shionne;
	vector<EFFECTDESC_MESH> vMeshEffectDesc_Shionne;

public:
	static void Release_Engine();
	virtual void Free() override;
};

END