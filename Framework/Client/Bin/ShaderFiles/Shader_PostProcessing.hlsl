Texture2D<float4> HDRTex : register(t0);
StructuredBuffer<float> AvgLum : register(t1);
Texture2D<float4> BloomTex : register(t2);
Texture2D<float4> BlurTexture : register(t3);

sampler LinearSampler = sampler_state {
	filter = min_mag_mip_linear;
	AddressU = wrap;
	AddressV = wrap;
};

sampler PointSampler = sampler_state {
	filter = min_mag_mip_point;
	AddressU = wrap;
	AddressV = wrap;
};

cbuffer Matrix
{
	float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
	float4x4 g_ViewMatrixInv, g_ProjMatrixInv;
}

struct VS_IN
{
	float3 vPosition : POSITION;
	float2 vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4 vPosition : SV_Position;
	float2 vTexUV : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT Out;

	float4 vPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
	vPosition = mul(vPosition, g_ViewMatrix);
	vPosition = mul(vPosition, g_ProjMatrix);

	Out.vPosition = vPosition;
	Out.vTexUV = In.vTexUV;

	return Out;
}

cbuffer FinalPassConstants : register(b0)
{
	float MiddleGrey : packoffset(c0);
	float LumWhiteSqr : packoffset(c0.y);
	float BloomScale : packoffset(c0.z);
}

static const float3 LUM_FACTOR = float3(0.299, 0.587, 0.114);

float3 ToneMapping(float3 HDRColor)
{
	float LScale = dot(HDRColor, LUM_FACTOR);
	LScale *= MiddleGrey / AvgLum[0];
	LScale = (LScale + LScale * LScale / LumWhiteSqr) / (1.0 + LScale);
	return HDRColor * LScale;
}

cbuffer Radial
{
	bool g_bRadialBlur;
	float g_fDistance;
	float g_fStrength;
}

float samples[5] = {
	0.01f, 0.02f, 0.03f, 0.04f, 0.05f
};

float3 RadialBlur(float3 HDRColor, float2 vTexUV)
{
	float2 vDirection = 0.5f - vTexUV;
	float fDistance = length(vDirection);

	vDirection /= fDistance;

	float4 vColor = HDRTex.Sample(PointSampler, vTexUV);
	float4 vSum = vColor;

	for (int i = 0; i < 5; ++i)
	{
		vSum += HDRTex.Sample(PointSampler, vTexUV + vDirection * samples[i] * g_fDistance);
	}
	vSum /= 6.f;
	
	float f = saturate(fDistance * g_fStrength);

	return lerp(vColor, vSum, f).rgb;
}

struct PS_IN
{
	float4 vPosition : SV_Position;
	float2 vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	float4 vColor : SV_Target0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out;
	float3 color = HDRTex.Sample(PointSampler, In.vTexUV.xy).xyz;
	float3 vBlurColor = BlurTexture.Sample(LinearSampler, In.vTexUV).xyz;

	float fBlurRatio = (vBlurColor.r + vBlurColor.g + vBlurColor.b) / 3.f;

	color += vBlurColor * fBlurRatio;

	if (true == g_bRadialBlur)
	{
		color = (color + RadialBlur(color, In.vTexUV)) * 0.5f;
	}
	
	color = ToneMapping(color);
	Out.vColor = float4(color, 1.f);

	return Out;
}

PS_OUT PS_MAIN_TEST(PS_IN In)
{
	PS_OUT Out;
	float3 color = HDRTex.Sample(PointSampler, In.vTexUV.xy).xyz;
	Out.vColor = float4(color, 1.f);

	return Out;
}

RasterizerState	RS_Default
{
	FillMode = Solid;
	FrontCounterClockwise = false;
	CullMode = back;
};

BlendState BS_NonBlend
{
	BlendEnable[0] = false;
	BlendEnable[1] = false;
};

BlendState BS_AlphaBlend
{
	BlendEnable[0] = true;
	SrcBlend[0] = Src_Alpha;
	DestBlend[0] = Inv_Src_Alpha;
	BlendOp[0] = Add;
};

BlendState BS_LightBlend
{
	BlendEnable[0] = true;
	BlendEnable[1] = true;
	SrcBlend = one;
	DestBlend = one;
	Blendop = add;
};

DepthStencilState DSS_Default
{
	DepthEnable = true;
	DepthWriteMask = all;
	DepthFunc = less_equal;
};

DepthStencilState DSS_NonZTestAndWrite
{
	DepthEnable = false;
	DepthWriteMask = zero;
};

technique11 DefaultTechnique
{
	pass DefaultRender//0
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_NonZTestAndWrite, 0);
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass TestRender//0
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_NonZTestAndWrite, 0);
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_TEST();
	}
}