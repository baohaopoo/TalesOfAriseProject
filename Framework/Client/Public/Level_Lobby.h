#pragma once
#include "Client_Defines.h"
#include "Level.h"
#include "Level_Loading.h"
#include "GameInstance.h"
#include "Camera_Default.h"

BEGIN(Client)

class CLevel_Lobby final : public CLevel
{
private:
	explicit CLevel_Lobby(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual ~CLevel_Lobby() = default;
public:
	virtual HRESULT NativeConstruct() override;
	virtual void Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;
public:
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_StartBtnEffect(const _tchar* pLayerTag);
	HRESULT Ready_Layer_ExitBtnEffect(const _tchar* pLayerTag);
public:
	static CLevel_Lobby* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual void Free() override;
private:
	bool mCol = false;
	bool mExitCol = false;

	CGameObject* StartBtnEffect;
	CGameObject* ExitBtnEffect;
private:
	_tchar UIMsg[MAX_PATH] = TEXT("");
	_tchar ExitMsg[MAX_PATH] = TEXT("");
};

END