#pragma once

#pragma warning (disable : 4005)
#pragma warning (disable : 4251)

#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>

#include <d3dcompiler.h>
#include <d3dx11effect.h>

#include <DirectXTK/Effects.h>
#include <DirectXTK/PrimitiveBatch.h>
#include <DirectXTK/VertexTypes.h>

#include <DirectXTK/SpriteBatch.h>
#include <DirectXTK/SpriteFont.h>

#include <DirectXTK/DDSTextureLoader.h>
#include <DirectXTK/WICTextureLoader.h>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

/* 텍스쳐 파일 입출력을 위하여. */
#include <DirectXTK/ScreenGrab.h>
#include <wincodec.h>

using namespace DirectX;


#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <typeinfo.h>

#include "Engine_Macro.h"
#include "Engine_Struct.h"
#include "Engine_Function.h"
#include "Engine_Typedef.h"


#include <vector>
#include <map>
#include <list>
#include <unordered_map>
#include <algorithm>
#include <set>
#include <queue>

#include <tchar.h>
#include <iostream>


using namespace std;

/* 릭체크 */

#include <fstream>
#include <process.h>

#include <PhysX/PxPhysics.h>
#include <PhysX/PxPhysicsAPI.h>
#include <PhysX/PxConfig.h>

#define PX_RELEASE(x)	if(x)	{ x->release(); x = NULL;	}
#define PVD_HOST "127.0.0.1"

using namespace physx;

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW		
#endif 
#endif // _DEBUG

#ifdef _DEBUG

extern bool g_bWireFrame;
extern bool g_bDeferred;
extern bool g_bLightAcc;
extern bool g_bLightDepth;
extern bool g_bDistortion;

#endif // _DEBUG

namespace Engine
{
	const unsigned int g_iLightCX = 5000;
	const unsigned int g_iLightCY = 5000;
}

using namespace Engine;