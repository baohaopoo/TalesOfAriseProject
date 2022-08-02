#pragma once

namespace Client
{
	const unsigned int	g_iWinCX = 1920; // 1280;
	const unsigned int	g_iWinCY = 1080; // 720;

	enum LEVEL { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_LOBBY, LEVEL_TUTORIAL, LEVEL_BOSS1, LEVEL_BOSS2, LEVEL_END };
}

#include <process.h>

extern HINSTANCE g_hInst;
extern HWND g_hWnd;

using namespace Client;
