#include "Shader_Defines.hlsli"

cbuffer Matrices
{
	float4x4		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
}

struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD;
	float4		vLocalPos : TEXCOORD1;
};

texture2D			g_Texture;
texture2D			g_DepthTexture;

sampler DefaultSampler = sampler_state {
	filter = min_mag_mip_linear;
	AddressU = wrap;
	AddressV = wrap;
};


VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out;

	float4		vPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
	vPosition = mul(vPosition, g_ViewMatrix);
	vPosition = mul(vPosition, g_ProjMatrix);

	Out.vPosition = vPosition;
	Out.vTexUV = In.vTexUV;
	Out.vLocalPos = vector(In.vPosition, 1.f);

	return Out;
}

struct VS_OUT_SOFT_EFFECT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD;
	float4		vProjPos : TEXCOORD1;
};


VS_OUT_SOFT_EFFECT VS_MAIN_SOFT_EFFECT(VS_IN In)
{
	VS_OUT_SOFT_EFFECT		Out;

	float4		vPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
	vPosition = mul(vPosition, g_ViewMatrix);
	vPosition = mul(vPosition, g_ProjMatrix);

	Out.vPosition = vPosition;
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = vPosition;

	return Out;
}




struct PS_IN
{
	vector			vPosition : SV_POSITION;
	float2			vTexUV : TEXCOORD0;
	float4			vLocalPos : TEXCOORD1;
};

struct PS_OUT
{
	vector			vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out;

	Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexUV);


	return Out;
}

struct PS_IN_SOFT_EFFECT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD;
	float4		vProjPos : TEXCOORD1;
};



PS_OUT PS_MAIN_SOFT_EFFECT(PS_IN_SOFT_EFFECT In)
{
	PS_OUT			Out;

	Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexUV);

	float		fViewZ = In.vProjPos.w;

	float2		vUV;

	vUV.x = (In.vProjPos.x / In.vProjPos.w) * 0.5f + 0.5f;
	vUV.y = (In.vProjPos.y / In.vProjPos.w) * -0.5f + 0.5f;

	float4		vDepthDesc = g_DepthTexture.Sample(DefaultSampler, vUV);
	float		fTargetViewZ = vDepthDesc.y * 300.f;

	Out.vColor.a = max(min(fTargetViewZ - fViewZ, Out.vColor.a), 0.f);


	return Out;
}



//BOOL BlendEnable;
//D3D11_BLEND SrcBlend;
//D3D11_BLEND DestBlend;
//D3D11_BLEND_OP BlendOp;
//D3D11_BLEND SrcBlendAlpha;
//D3D11_BLEND DestBlendAlpha;
//D3D11_BLEND_OP BlendOpAlpha;
//UINT8 RenderTargetWriteMask;


technique11 DefaultTechnique
{
	/* Pass를 나누는 기준 == 그릴려고하는 리소스에게 어떤 셰이더 세트 효과를 줄것인지?! */

	/* 명암 + 그림자 + 노멀 */
	pass DefaultRendering
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_NonBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
		/*	SetDepthStencilState();
		*/

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass SoftEffect
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_SOFT_EFFECT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_SOFT_EFFECT();
	}

}


