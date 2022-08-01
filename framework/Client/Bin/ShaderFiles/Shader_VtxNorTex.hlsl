
#include "Shader_Defines.hpp"


cbuffer Matrices
{
	float4x4		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
}

cbuffer BrushDesc
{
	vector		g_vBrushPosition = vector(15.f, 0.f, 15.f, 1.f);
	float		g_fBrushRange = 5.f;
};

texture2D			g_SourDiffuseTexture;
texture2D			g_DestDiffuseTexture;
texture2D			g_MaskTexture;
texture2D			g_BrushTexture;
texture2D			g_DiffuseTexture;

sampler DefaultSampler = sampler_state {
	filter = min_mag_mip_linear;
	AddressU = wrap;
	AddressV = wrap;
};

sampler PointSampler = sampler_state {
	filter = min_mag_mip_point;
	AddressU = wrap;
	AddressV = wrap;
};

struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD;

};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
};

VS_OUT VS_MAIN_TERRAIN(VS_IN In)
{
	VS_OUT		Out;

	float4		vPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
	vPosition = mul(vPosition, g_ViewMatrix);
	vPosition = mul(vPosition, g_ProjMatrix);

	Out.vPosition = vPosition;
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));
	Out.vTexUV = In.vTexUV;
	Out.vWorldPos = mul(vector(In.vPosition, 0.f), g_WorldMatrix);
	Out.vProjPos = vPosition;

	return Out;
}



struct PS_IN
{
	vector			vPosition : SV_POSITION;
	float4			vNormal : NORMAL;
	float2			vTexUV : TEXCOORD0;
	float4			vWorldPos : TEXCOORD1;
	float4			vProjPos : TEXCOORD2;
};

struct PS_OUT
{
	vector			vDiffuse : SV_TARGET0;
	vector			vNormal : SV_TARGET1;
	vector			vDepth : SV_TARGET2;

};

PS_OUT PS_MAIN_TERRAIN(PS_IN In)
{
	PS_OUT			Out;

	vector		vSourDiffuse = g_SourDiffuseTexture.Sample(DefaultSampler, In.vTexUV * 10.f);
	vector		vDestDiffuse = g_DestDiffuseTexture.Sample(DefaultSampler, In.vTexUV * 10.f);
	vector		vMask = g_MaskTexture.Sample(DefaultSampler, In.vTexUV);
	vector		vBrush = vector(0.f, 0.f, 0.f, 0.f);

	if (g_vBrushPosition.x - g_fBrushRange < In.vWorldPos.x && In.vWorldPos.x <= g_vBrushPosition.x + g_fBrushRange &&
		g_vBrushPosition.z - g_fBrushRange < In.vWorldPos.z && In.vWorldPos.z <= g_vBrushPosition.z + g_fBrushRange)
	{
		float2		vTexUV;
		vTexUV.x = (In.vWorldPos.x - (g_vBrushPosition.x - g_fBrushRange)) / (2.f * g_fBrushRange);
		vTexUV.y = ((g_vBrushPosition.z + g_fBrushRange) - In.vWorldPos.z) / (2.f * g_fBrushRange);

		vBrush = g_BrushTexture.Sample(DefaultSampler, vTexUV);
	}

	vector		vMtrlDiffuse = vDestDiffuse * vMask + vSourDiffuse * (1.f - vMask) + vBrush;

	Out.vDiffuse = vector(vMtrlDiffuse.rgb, 1.f);
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 300.0f, 0.f, 0.f);

	return Out;
}



technique11 DefaultTechnique
{
	pass Terrain
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_NonBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_TERRAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_TERRAIN();
	}
}


