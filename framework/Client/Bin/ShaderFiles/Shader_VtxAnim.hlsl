#include "Shader_Defines.hlsli"

struct BoneMatrix
{
	float4x4 BoneMatrices[256];
};

texture2D g_DiffuseTexture;
texture2D g_NormalTexture;
texture2D g_DissolveTexture;

sampler DefaultSampler = sampler_state {
	filter = min_mag_mip_linear;
	AddressU = wrap;
	AddressV = wrap;
};

cbuffer Matrices
{
	float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
	BoneMatrix g_BoneMatrices;
}

cbuffer RimLight
{
	vector g_vCamPosition;		// 림라이트 사용할 때 필요한 전역변수
}

cbuffer Alpha				// 디졸브시 사용하는 알파값으로 선언
{
	float g_fAlpha;
};

struct VS_IN
{
	float3 vPosition : POSITION;
	float3 vNormal : NORMAL;
	float2 vTexUV : TEXCOORD;
	uint4 vBlendIndex : BLENDINDEX;
	float4 vBlendWeight : BLENDWEIGHT;
	float3		vTangent : TANGENT;				// 추가
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;

	float3		vTangent : TANGENT;			// 추가
	float3		vBinormal : BINORMAL;		// 추가
	float4		vWorldPos : TEXCOORD2;
};


VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out;

	float		fWeightW = 1.f - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);

	float4x4	BoneMatrix =
		g_BoneMatrices.BoneMatrices[In.vBlendIndex.x] * In.vBlendWeight.x +
		g_BoneMatrices.BoneMatrices[In.vBlendIndex.y] * In.vBlendWeight.y +
		g_BoneMatrices.BoneMatrices[In.vBlendIndex.z] * In.vBlendWeight.z +
		g_BoneMatrices.BoneMatrices[In.vBlendIndex.w] * fWeightW;


	float4x4	matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	float4		vPosition = mul(float4(In.vPosition, 1.f), BoneMatrix);
	Out.vPosition = mul(vPosition, matWVP);

	float4		vNormal = mul(float4(In.vNormal, 0.f), BoneMatrix);
	Out.vNormal = normalize(mul(vNormal, g_WorldMatrix));

	float3		vTangent = mul(float4(In.vTangent, 0.f), BoneMatrix);
	Out.vTangent = normalize(mul(vector(vTangent, 0.f), g_WorldMatrix)).xyz;

	Out.vBinormal = cross(Out.vNormal.xyz, Out.vTangent);

	Out.vTexUV = In.vTexUV;

	Out.vProjPos = Out.vPosition;

	Out.vWorldPos = mul(vector(In.vPosition, 0.f), g_WorldMatrix);

	return Out;
}

struct PS_IN
{
	vector			vPosition : SV_POSITION;
	float4			vNormal : NORMAL;
	float2			vTexUV : TEXCOORD0;
	float4			vProjPos : TEXCOORD1;

	float3		vTangent : TANGENT;		// 추가
	float3		vBinormal : BINORMAL;	// 추가
	float4		vWorldPos : TEXCOORD2;
};

struct PS_OUT
{
	vector			vDiffuse : SV_TARGET0;
	vector			vNormal : SV_TARGET1;
	vector			vDepth : SV_TARGET2;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	vector		vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);

	float3		vNormal = vNormalDesc.xyz * 2.f - 1.f;

	float3x3	WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal.xyz);

	vNormal = normalize(mul(vNormal, WorldMatrix));

	if (vMtrlDiffuse.a < 0.3f)
		discard;

	Out.vDiffuse = vMtrlDiffuse;
	Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 600.0f, 0.f, 0.f); //600은 카메라의 far값

	return Out;
}


PS_OUT PS_RIMLIGHT(PS_IN In)		// 림라이트 사용할 경우에 적용하는 픽셀셰이더
{
	PS_OUT         Out = (PS_OUT)0;

	vector      vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	vector      vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);

	float3      vNormal = vNormalDesc.xyz * 2.f - 1.f;

	float3x3   WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal.xyz);

	vNormal = normalize(mul(vNormal, WorldMatrix));

	Out.vDiffuse = vMtrlDiffuse;

	Out.vNormal = vector(vNormal.xyz* 0.5f + 0.5f, 0.f);

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 600.f, 0.0f, 0.f);		// 600은 카메라의 far값

	if (vMtrlDiffuse.a < 0.1f)
		discard;

	vector Look = normalize(In.vWorldPos - g_vCamPosition);
	float Rim = smoothstep(0.2f, 1.f, 1.f - max(0.f, dot(vNormal, Look))); //smoothstep 은 색상 선형보간을 한다

	Out.vDiffuse += Rim*vector(1.f, 1.f, 1.f, 0.f);

	return Out;
}


PS_OUT PS_MAIN_DISSOLVE(PS_IN In)				// 디졸브 사용시 적용하는 픽셀 셰이더
{
	PS_OUT         Out;

	float4 vDissolveColor = g_DissolveTexture.Sample(DefaultSampler, In.vTexUV);


	float4 vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	vDiffuse.a = g_fAlpha;

	if (vDissolveColor.r <= vDiffuse.a)
	{
		vDiffuse.a = 1.f;
	}

	else {
		vDiffuse.a = 0.f;
	}

	if (vDiffuse.a == 0.f)
		discard;

	if (vDissolveColor.r - 0.02 <= vDiffuse.a &&vDissolveColor.r + 0.02 >vDiffuse.a)
		vDiffuse = float4(1.f, 0.f, 0.f, 1.f);

	if (vDiffuse.a<0.04&&vDiffuse.a > 0.01f)
		vDiffuse = float4(1.f, 0.f, 0.f, 1.f);

	Out.vDiffuse = vDiffuse;
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 600.f, 0.0f, 0.f);		// 600은 카메라의 far값


	return Out;
}

technique11 DefaultTechnique
{
	pass DefaultRendering//0
	{
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}
}