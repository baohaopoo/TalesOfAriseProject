#pragma once

namespace Engine
{
	enum EFFECT_TYPE { EFFECT_TYPE_MESH, EFFECT_TYPE_INSTANCE, EFFECT_TYPE_END };
	typedef struct EffectDesc
	{
		bool			bBillboard = false;
		unsigned int	iNum = 100;
		unsigned int	iTexture = 0;
		unsigned int	iShader = 0;
		float			fLoop = 0.f;
		float			fRotationSpe = 0.f;
		float			fMaxTime = 10.f;
		float			fPassTime = 0.f;
		float			fLastTime = 0;
		float			fScale = 0.05f;
		float			fScale_Speed = 5.f;
		float			fSpeed = 1.f;
		float			fSpeedRand = 1.f;

		float			KeyFram_1_TimeEnd = 2.5f;
		float			KeyFram_1_RelaxSpeed = 1.f;
		float			KeyFram_1_Speed = 0.f;
		float			KeyFram_1_fRotation = 1.f;
		float			KeyFram_1_fRelaxRotation = 0.f;

		float			KeyFram_2_TimeEnd = 5.f;
		float			KeyFram_2_RelaxSpeed = 1.f;
		float			KeyFram_2_Speed = 0.f;
		float			KeyFram_2_fRotation = 1.f;
		float			KeyFram_2_fRelaxRotation = 0.f;

		float			KeyFram_3_TimeEnd = 7.5f;
		float			KeyFram_3_RelaxSpeed = 1.f;
		float			KeyFram_3_Speed = 0.f;
		float			KeyFram_3_fRotation = 1.f;
		float			KeyFram_3_fRelaxRotation = 0.f;

		float			KeyFram_4_TimeEnd = 10.f;
		float			KeyFram_4_RelaxSpeed = 1.f;
		float			KeyFram_4_Speed = 0.f;
		float			KeyFram_4_fRotation = 1.f;
		float			KeyFram_4_fRelaxRotation = 0.f;

		XMFLOAT4		KeyFram_1_Movement = XMFLOAT4(0.f, 0.f, 0.f, 0.f);
		XMFLOAT4		KeyFram_2_Movement = XMFLOAT4(0.f, 0.f, 0.f, 0.f);
		XMFLOAT4		KeyFram_3_Movement = XMFLOAT4(0.f, 0.f, 0.f, 0.f);
		XMFLOAT4		KeyFram_4_Movement = XMFLOAT4(0.f, 0.f, 0.f, 0.f);

		XMFLOAT4		vRotation = XMFLOAT4(-0.5f, -0.5f, -0.5f, 0.f);
		XMFLOAT4		vMovement = XMFLOAT4(-0.5f, -0.5f, -0.5f, 0.f);
		XMFLOAT4		vMovementRand = XMFLOAT4(1.f, 1.f, 1.f, 0.f);
		XMFLOAT4		vPostion = XMFLOAT4(0.f, 0.f, 0.f, 0.f);
		XMFLOAT4		vPostionRand = XMFLOAT4(0.f, 0.f, 0.f, 0.f);
		XMFLOAT4		vColor1 = XMFLOAT4(0.f, 0.f, 0.f, 0.f);
		XMFLOAT4		vColor2 = XMFLOAT4(0.f, 0.f, 0.f, 0.f);
	}EFFECTDESC;

	typedef struct EffectDesc_Mesh
	{
		bool			bBillboard = false;
		unsigned int	iMesh = 0;
		unsigned int	iTexture = 0;
		unsigned int	iTexture1 = 0;
		unsigned int	iTexture2 = 0;
		unsigned int	iShader = 0;
		float			MainAlpha = 1.f;

		float			fMaxTime = 10.f;
		float			fPassTime = 0.f;

		float			KeyFram_0_TimeEnd = 0.f;
		float			KeyFram_1_TimeEnd = 2.5f;
		float			KeyFram_2_TimeEnd = 5.f;
		float			KeyFram_3_TimeEnd = 7.5f;
		float			KeyFram_4_TimeEnd = 10.f;

		float			KeyFram_0_Alpha = 0.f;
		float			KeyFram_1_Alpha = 0.f;
		float			KeyFram_2_Alpha = 0.f;
		float			KeyFram_3_Alpha = 0.f;
		float			KeyFram_4_Alpha = 0.f;

		XMFLOAT3		KeyFram_0_Scale = XMFLOAT3(1.f, 1.f, 1.f);
		XMFLOAT3		KeyFram_1_Scale = XMFLOAT3(1.f, 1.f, 1.f);
		XMFLOAT3		KeyFram_2_Scale = XMFLOAT3(1.f, 1.f, 1.f);
		XMFLOAT3		KeyFram_3_Scale = XMFLOAT3(1.f, 1.f, 1.f);
		XMFLOAT3		KeyFram_4_Scale = XMFLOAT3(1.f, 1.f, 1.f);

		XMFLOAT3		KeyFram_0_Shader = XMFLOAT3(1.f, 1.f, 1.f);
		XMFLOAT3		KeyFram_1_Shader = XMFLOAT3(1.f, 1.f, 1.f);
		XMFLOAT3		KeyFram_2_Shader = XMFLOAT3(1.f, 1.f, 1.f);
		XMFLOAT3		KeyFram_3_Shader = XMFLOAT3(1.f, 1.f, 1.f);
		XMFLOAT3		KeyFram_4_Shader = XMFLOAT3(1.f, 1.f, 1.f);

		XMFLOAT3		KeyFram_0_Rotation = XMFLOAT3(0.f, 0.f, 0.f);
		XMFLOAT3		KeyFram_1_Rotation = XMFLOAT3(0.f, 0.f, 0.f);
		XMFLOAT3		KeyFram_2_Rotation = XMFLOAT3(0.f, 0.f, 0.f);
		XMFLOAT3		KeyFram_3_Rotation = XMFLOAT3(0.f, 0.f, 0.f);
		XMFLOAT3		KeyFram_4_Rotation = XMFLOAT3(0.f, 0.f, 0.f);

		XMFLOAT3		KeyFram_0_Position = XMFLOAT3(0.f, 0.f, 0.f);
		XMFLOAT3		KeyFram_1_Position = XMFLOAT3(0.f, 0.f, 0.f);
		XMFLOAT3		KeyFram_2_Position = XMFLOAT3(0.f, 0.f, 0.f);
		XMFLOAT3		KeyFram_3_Position = XMFLOAT3(0.f, 0.f, 0.f);
		XMFLOAT3		KeyFram_4_Position = XMFLOAT3(0.f, 0.f, 0.f);

		XMFLOAT4		vColor1 = XMFLOAT4(0.f, 0.f, 0.f, 0.f);
		XMFLOAT4		vColor2 = XMFLOAT4(0.f, 0.f, 0.f, 0.f);
	}EFFECTDESC_MESH;


	typedef struct tagKeyFrame
	{
		XMFLOAT3			vScale;
		XMFLOAT4			vRotation;
		XMFLOAT3			vTranslation;
		double				Time;
	}KEYFRAME;

	typedef struct tagModelMaterial
	{
		class CTexture*		pMaterials[AI_TEXTURE_TYPE_MAX];
	}MODELMATERIAL;

	typedef struct tagLightDesc
	{
		enum TYPE { TYPE_DIRECTIONAL, TYPE_POINT, TYPE_END };
		TYPE			eType;

		// 점광원에 필요한 정보
		XMFLOAT4		vPosition;
		float			fRange;

		// 방향광에 필요한 정보
		XMFLOAT4		vDirection;

		// 공통적으로 필요한 정보
		XMFLOAT4		vDiffuse;
		XMFLOAT4		vAmbient;// = (1.f, 1.f, 1.f, 1.f);
		XMFLOAT4		vSpecular;

	}LIGHTDESC;

	typedef struct tagMaterial
	{
		XMFLOAT4		vDiffuse;
		XMFLOAT4		vAmbient;// = (0.4f, 0.4f, 0.4f, 1.f);
		XMFLOAT4		vSpecular;
		float			fPower;

	}MATERIALDESC;

	typedef struct tagVertex_Non_Anim_Model
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexUV;
		XMFLOAT3		vTangent;
	}VTXNONANIMMODEL;


	typedef struct ENGINE_DLL tagVertex_Non_Anim_Model_Declaration
	{
		static const unsigned int iNumElements;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[4];
	}VTXNONANIMMODEL_DECLARATION;


	typedef struct tagVertex_Normal_Texture
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexUV;
	}VTXNORTEX;

	typedef struct ENGINE_DLL tagVertex_Normal_Texture_Declaration
	{
		static const unsigned int iNumElements;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[3];
	}VTXNORTEX_DECLARATION;


	typedef struct tagVertex_Matrix
	{
		XMFLOAT4		vRight, vUp, vLook, vTranslation;
	}VTXMATRIX;

	typedef struct ENGINE_DLL tagVertex_Instance_Declaration
	{
		static const unsigned int iNumElements;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[6];
	}VTXINSTANCE_DECLARATION;

	typedef struct tagVertex_Point
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vPSize;
	}VTXPOINT;

	typedef struct tagVertex_Anim_Model
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexUV;
		XMUINT4			vBlendIndex; /* 이 점정에 영향을 주는 뼈의 인덱스들(최대 네개)*/
		XMFLOAT4		vBlendWeight; /* 각 뼈(최대 네개)가 정점에게 주는 영향(0.0 ~ 1.0) */
		XMFLOAT3		vTangent;
	}VTXANIMMODEL;

	typedef struct ENGINE_DLL tagVertex_Anim_Model_Declaration
	{
		static const unsigned int iNumElements;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[6];
	}VTXANIMMODEL_DECLARATION;


	typedef struct tagVertex_Cube
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vTexUV;
	}VTXCUBE;

	typedef struct ENGINE_DLL tagVertex_Cube_Declaration
	{
		static const unsigned int iNumElements;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[2];
	}VTXCUBE_DECLARATION;


	typedef struct tagVertex_Color
	{
		XMFLOAT3		vPosition;
		XMFLOAT4		vColor;
	}VTXCOL;

	typedef struct ENGINE_DLL tagVertex_Color_Declaration
	{
		static const unsigned int iNumElements;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[2];
	}VTXCOL_DECLARATION;


	typedef struct tagVertex_Texture
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vTexUV;
	}VTXTEX;

	typedef struct ENGINE_DLL tagVertex_Texture_Declaration
	{
		static const unsigned int iNumElements;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[2];
	}VTXTEX_DECLARATION;

	typedef struct tagVertex_Cube_Texture
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vTexUV;
	}VTXCUBETEX;

	typedef struct tagFaceListIndices32
	{
		unsigned long		_0, _1, _2;
	}FACELISTINDICES32;


	typedef struct tagFaceListIndices16
	{
		unsigned short		_0, _1, _2;
	}FACELISTINDICES16;
}
