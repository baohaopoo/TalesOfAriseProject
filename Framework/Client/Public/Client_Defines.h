#pragma once

namespace Client
{
	const unsigned int	g_iWinCX = 1920;//1280; // 1280;
	const unsigned int	g_iWinCY = 1080;//720; // 720;

	enum LEVEL { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_LOBBY, LEVEL_TUTORIAL, LEVEL_BOSS1, LEVEL_BOSS2, LEVEL_END };

	enum MAP_TYPE { MAP_GYT, MAP_BALSELPH, MAP_FIREAVATAR, MAP_BATTLE02, MAP_BATTLE04, MAP_END };

	enum DAMAGE_TYPE { DAMAGE_TYPE_DOWN, DAMAGE_TYPE_LARGE, DAMAGE_TYPE_SMALL, DAMAGE_TYPE_END };

	enum DAMAGE_DIR { DAMAGE_DIR_FRONT, DAMAGE_DIR_BACK, DAMAGE_DIR_LEFT, DAMAGE_DIR_RIGHT, DAMAGE_DIR_END };

}

#include <process.h>

#define  LAYER_MAPTOOL_CAMERA					TEXT("Layer_MapTool_Camera")
#define  LAYER_MAPTOOL_TERRAIN					TEXT("Layer_MapTool_Terrain")
#define  LAYER_MAPTOOL_BattleMap				TEXT("Layer_MapTool_BattleMap")
#define  LAYER_MAPTOOL_PlacedObjects			TEXT("Layer_MapTool_PlacedObjects")
#define  LAYER_MAPTOOL_WayPoints				TEXT("Layer_MapTool_WayPoints")
#define  LAYER_MAPTOOL_Picking_Rect				TEXT("Layer_MapTool_PickingB_Rect")
#define  LAYER_MAPTOOL_Dot						TEXT("Layer_MapTool_Dot")

#define  LAYER_MAP_BATTLE						TEXT("Layer_Map_Battle")

extern HINSTANCE g_hInst;
extern HWND g_hWnd;

using namespace Client;
