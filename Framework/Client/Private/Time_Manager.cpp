#include "stdafx.h"
#include "..\Public\Time_Manager.h"
#include "GameInstance.h"
#include "Player.h"
#include"Camera_Default.h"
#include "Layer.h"




IMPLEMENT_SINGLETON(CTime_Manager)


CTime_Manager::CTime_Manager()
{
}


void CTime_Manager::Set_LayerTime(LAYER_TYPE eType, _double dSpeed)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();


	CLayer *pPlayerLayer = pGameInstance->Find_Layer(LEVEL_STATIC, TEXT("Layer_Player"));
	if (pPlayerLayer == nullptr)
		return;
	for (auto Layer : pPlayerLayer->Get_ObjectList())
	{
		Layer->Set_Time(dSpeed);
	}

}

void CTime_Manager::Set_AllLayerTime(_double dSpeed)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	CLayer *pPlayerLayer = pGameInstance->Find_Layer(LEVEL_STATIC, TEXT("Layer_Player"));
	if (pPlayerLayer == nullptr)
		return;
	for (auto Layer : pPlayerLayer->Get_ObjectList())
	{
		Layer->Set_Time(dSpeed);
	}

	CLayer *pMonsterLayer = pGameInstance->Find_Layer(LEVEL_STATIC, TEXT("Monster_Battle_Layer"));
	if (pMonsterLayer == nullptr)
		return;
	for (auto Layer : pMonsterLayer->Get_ObjectList())
	{
		Layer->Set_Time(dSpeed);
	}
}



void CTime_Manager::Free()
{

}