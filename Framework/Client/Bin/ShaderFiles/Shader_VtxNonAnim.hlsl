#include "Shader_Defines.hlsli"


cbuffer Matrices
{
	float4x4		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
	float4x4		g_SocketMatrix;
}

cbuffer PreviewColor
{
	vector g_PreviewDiffuse = (vector)0.f;
};

cbuffer Blur
{
	bool g_bBlur = false;
};

texture2D			g_DiffuseTexture;
texture2D			g_NormalTexture;
texture2D			g_EmissiveTexture;
texture2D			g_TerrainDiffuseTexture;

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
	float3		vTangent : TANGENT;
};

struct VS_OUT_MODEL
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;

	float3		vTangent : TANGENT;
	float3		vBinormal : BINORMAL;
};

VS_OUT_MODEL VS_MAIN_MODEL(VS_IN In)
{
	VS_OUT_MODEL		Out;

	float4		vPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
	vPosition = mul(vPosition, g_ViewMatrix);
	vPosition = mul(vPosition, g_ProjMatrix);

	Out.vPosition = vPosition;
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));
	Out.vTangent = normalize(mul(vector(In.vTangent, 0.f), g_WorldMatrix)).xyz;
	Out.vBinormal = cross(Out.vNormal.xyz, Out.vTangent);
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = vPosition;

	return Out;
}


VS_OUT_MODEL VS_MAIN_MODEL_SOCKET(VS_IN In)
{
	VS_OUT_MODEL		Out;

	matrix			matWS, matWSV, matWSVP;

	matWS = mul(g_WorldMatrix, g_SocketMatrix);
	matWSV = mul(matWS, g_ViewMatrix);
	matWSVP = mul(matWSV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWSVP);
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));
	Out.vTangent = normalize(mul(vector(In.vTangent, 0.f), g_WorldMatrix)).xyz;
	Out.vBinormal = cross(Out.vNormal.xyz, Out.vTangent);
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;

	return Out;
}


struct PS_IN_MODEL
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;

	float3		vTangent : TANGENT;
	float3		vBinormal : BINORMAL;
};

struct PS_OUT
{
	vector			vDiffuse : SV_TARGET0;
	vector			vNormal : SV_TARGET1;
	vector			vDepth : SV_TARGET2;
	vector			vEmissive : SV_TARGET3;
	vector			vBlur : SV_TARGET4;
};

PS_OUT PS_MAIN_MODEL(PS_IN_MODEL In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	vector		vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);

	float3		vNormal = vNormalDesc.xyz * 2.f - 1.f;

	float3x3	WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal.xyz);

	vNormal = normalize(mul(vNormal, WorldMatrix));

	Out.vDiffuse = vMtrlDiffuse;
	Out.vNormal = vector(vNormal * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 300.0f, 0.f, 0.f);
	Out.vEmissive = g_EmissiveTexture.Sample(DefaultSampler, In.vTexUV);

	if (true == g_bBlur)
	{
		Out.vBlur = vMtrlDiffuse;
	}

	if (vMtrlDiffuse.a < 0.1f)
		discard;

	return Out;
}

PS_OUT PS_MAIN_MODEL_PREVIEW(PS_IN_MODEL In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	vector		vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);

	float3		vNormal = vNormalDesc.xyz * 2.f - 1.f;

	float3x3	WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal.xyz);

	vNormal = normalize(mul(vNormal, WorldMatrix));

	Out.vDiffuse = saturate(vMtrlDiffuse + g_PreviewDiffuse);
	Out.vNormal = vector(vNormal * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 300.0f, 0.f, 0.f);

	if (vMtrlDiffuse.a < 0.1f)
		discard;

	return Out;
}

PS_OUT PS_MAIN_MODEL_Multiply_Texture(PS_IN_MODEL In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vMtrlDiffuse = g_TerrainDiffuseTexture.Sample(DefaultSampler, In.vTexUV * 300.f);
	vector		vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV * 300.f);

	float3		vNormal = -1.f;// vNormalDesc.xyz * 2.f - 1.f;

	float3x3	WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal.xyz);

	vNormal = normalize(mul(vNormal, WorldMatrix));

	Out.vDiffuse = vMtrlDiffuse;
	Out.vNormal = vector(vNormal * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 300.0f, 0.f, 0.f);

	if (vMtrlDiffuse.a < 0.1f)
		discard;

	return Out;
}

technique11 DefaultTechnique
{
	pass Model
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_NonBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_MODEL();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_MODEL();
	}

	pass Model_Socket
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_NonBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_MODEL_SOCKET();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_MODEL();
	}
	
	pass Model_Preview
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_NonBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_MODEL();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_MODEL_PREVIEW();
	}

	pass Model_Multiply_Texture
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_NonBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_MODEL();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_MODEL_Multiply_Texture();
	}
}


