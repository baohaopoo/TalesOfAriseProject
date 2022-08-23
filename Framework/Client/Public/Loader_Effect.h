#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CComponent;
END

BEGIN(Client)

class CLoader_Effect final : public CBase
{
private:
	explicit CLoader_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CLoader_Effect() = default;
public:
	CRITICAL_SECTION Get_CriticalSection();
	LEVEL Get_LevelIndex();
	_bool Get_Finished() const;
	const _tchar* Get_LoadingText() const;
public:
	HRESULT NativeConstruct(LEVEL eLevel);
public:
	HRESULT Loading_ForLobbyLevel();
private:
	HRESULT Loading_Lobby_Model();
	HRESULT Loading_Lobby_Texture();
	HRESULT Loading_Lobby_Object();
public:
	HRESULT Loading_ForTutorialLevel();
private:
	HRESULT Loading_Tutorial_Model();
	HRESULT Loading_Tutorial_Texture();
	HRESULT Loading_Tutorial_Object();
public:
	HRESULT Loading_ForBoss1Level();
private:
	HRESULT Loading_Boss1_Model();
	HRESULT Loading_Boss1_Texture();
	HRESULT Loading_Boss1_Object();
public:
	HRESULT Loading_ForBoss2Level();
private:
	HRESULT Loading_Boss2_Model();
	HRESULT Loading_Boss2_Texture();
	HRESULT Loading_Boss2_Object();
private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;
private:
	HANDLE m_hThread = 0;
	CRITICAL_SECTION m_CriticalSection;
private:
	LEVEL m_eLevel = LEVEL_END;
	_bool m_isFinished = false;
	_tchar m_szLoading[MAX_PATH] = L"";
public:
	static CLoader_Effect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, LEVEL eLevel);
	virtual void Free() override;
};

END