#pragma once

#include "Client_Defines.h"
#include "Level.h"

/* �ε����� �����ش�. */
/* . �δ��� �ε��Ҽ��ֵ���. �δ���ü���� */
/* �δ����� ������ �� ����. */
BEGIN(Client)

class CLevel_Loading final : public CLevel
{
private:
	explicit CLevel_Loading(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual ~CLevel_Loading() = default;
public:
	virtual HRESULT NativeConstruct(LEVEL eNextLevel);
	virtual void Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;
public:
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);

private:
	LEVEL m_eNextLevel = LEVEL_END;
	class CLoader_etc*	m_pLoader_etc = nullptr;
	class CLoader_Map* m_pLoader_Map = nullptr;
	class CLoader_Effect* m_pLoader_Effect = nullptr;
	class CLoader_Player* m_pLoader_Player = nullptr;
	class CLoader_Monster* m_pLoader_Monster = nullptr;
	class CLoader_MapObject* m_pLoader_MapObject = nullptr;

	int num = 0;
	_float numtime = 0;
public:
	static CLevel_Loading* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut, LEVEL eNextLevel);
	virtual void Free() override;

private:
	_tchar LoadingMsg[MAX_PATH] = TEXT("");
	//string LoadingMsg = "";


};

END