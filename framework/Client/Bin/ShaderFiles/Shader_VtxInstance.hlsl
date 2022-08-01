#include "Shader_Defines.hlsli"

cbuffer Matrices
{
	float4x4		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
}

cbuffer Camera
{
	vector			g_vCamPosition;
};

texture2D			g_DiffuseTexture;

sampler DefaultSampler = sampler_state {
	filter = min_mag_mip_linear;
	AddressU = wrap;
	AddressV = wrap;
};

struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
	float4		vRight : TEXCOORD1;
	float4		vUp : TEXCOORD2;
	float4		vLook : TEXCOORD3;
	float4		vTranslation : TEXCOORD4;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD;
};

VS_OUT VS_MAIN_RECT(VS_IN In)
{
	VS_OUT		Out;

	float4x4	InstanceMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);

	float4x4	matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	vector		vPosition = mul(float4(In.vPosition, 1.f), InstanceMatrix);
	vPosition = mul(vPosition, matWVP);

	Out.vPosition = vPosition;
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct VS_OUT_POINT
{
	float4		vPosition : POSITION;
	float2		vPSize : PSIZE;
};

VS_OUT_POINT VS_MAIN_POINT(VS_IN In)
{
	VS_OUT_POINT		Out;

	float4x4	InstanceMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);

	vector		vPosition = mul(float4(In.vPosition, 1.f), InstanceMatrix);
	vPosition = mul(vPosition, g_WorldMatrix);

	Out.vPosition = vPosition;
	Out.vPSize = In.vTexUV;

	return Out;
}

struct GS_IN
{
	float4		vPosition : POSITION;
	float2		vPSize : PSIZE;
};

struct GS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

[maxvertexcount(6)]
void GS_MAIN_POINT(in point GS_IN In[1], inout TriangleStream<GS_OUT> OutStream)
{
	GS_OUT			Out[4];

	float3			vLook = (g_vCamPosition - In[0].vPosition).xyz;
	float3			vAxisY = float3(0.f, 1.f, 0.f);

	float3			vRight = normalize(cross(vAxisY, vLook)) * In[0].vPSize.x;
	float3			vUp = normalize(cross(vLook, vRight)) * In[0].vPSize.y;

	Out[0].vPosition = float4(In[0].vPosition.xyz + vRight + vUp, 1.f);
	Out[0].vTexUV = float2(0.f, 0.f);

	Out[1].vPosition = float4(In[0].vPosition.xyz - vRight + vUp, 1.f);
	Out[1].vTexUV = float2(1.f, 0.f);

	Out[2].vPosition = float4(In[0].vPosition.xyz - vRight - vUp, 1.f);
	Out[2].vTexUV = float2(1.f, 1.f);

	Out[3].vPosition = float4(In[0].vPosition.xyz + vRight - vUp, 1.f);
	Out[3].vTexUV = float2(0.f, 1.f);


	float4x4	matVP;
	matVP = mul(g_ViewMatrix, g_ProjMatrix);

	Out[0].vPosition = mul(Out[0].vPosition, matVP);
	Out[1].vPosition = mul(Out[1].vPosition, matVP);
	Out[2].vPosition = mul(Out[2].vPosition, matVP);
	Out[3].vPosition = mul(Out[3].vPosition, matVP);

	OutStream.Append(Out[0]);
	OutStream.Append(Out[1]);
	OutStream.Append(Out[2]);
	OutStream.RestartStrip();

	OutStream.Append(Out[0]);
	OutStream.Append(Out[2]);
	OutStream.Append(Out[3]);
	OutStream.RestartStrip();
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector			vColor : SV_TARGET0;
};

PS_OUT PS_MAIN_RECT(PS_IN In)
{
	PS_OUT			Out;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	if (Out.vColor.a == 0.0f)
		discard;

	return Out;
}

PS_OUT PS_MAIN_POINT(PS_IN In)
{
	PS_OUT			Out;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vColor.r = 1.f;
	Out.vColor.gb = 0.f;

	if (Out.vColor.a == 0.0f)
		discard;

	return Out;
}



technique11 DefaultTechnique
{
	pass Rect
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_NonBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_RECT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_RECT();
	}

	pass Point
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_NonBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_POINT();
		GeometryShader = compile gs_5_0 GS_MAIN_POINT();
		PixelShader = compile ps_5_0 PS_MAIN_POINT();
	}

}