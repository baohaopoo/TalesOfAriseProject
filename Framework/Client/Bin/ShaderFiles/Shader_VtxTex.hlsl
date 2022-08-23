#include "Shader_Defines.hlsli"

cbuffer Matrices
{
	float4x4		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
}
cbuffer BackgroundUI
{
	float backuirgb;
};

//로비화면 시작/Exit버튼용 effect
cbuffer glowbtnUI
{
	float glowbtnrgb;
};

cbuffer emblemRgb
{
	float emblemrgb;
};

cbuffer smasheffectui
{
	float smasheffectrgb;
}
cbuffer TutoLineMsgRgb
{
	float tutolinemsgrgb;
}
cbuffer AlluirgbDa
{
	float alluirgb;
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
	vector			vBlur : SV_TARGET4;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out;

	Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexUV);


	return Out;
}
PS_OUT PS_MAIN2(PS_IN In)
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


//SmashEffect를 위한 쉐이더
PS_OUT PS_SMASHEFFECTUI(PS_IN In) {
	PS_OUT			Out;
	Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexUV);


	////알파블렌드를 지워준다.
	Out.vColor.a = smasheffectrgb;
	if (Out.vColor.a < 0.2f)
		discard;
	return Out;
}

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

PS_OUT PS_BackUI(PS_IN In)
{
	PS_OUT			Out;

	Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexUV);

	//알파블렌드를 지워준다.
	Out.vColor.a = backuirgb;
	return Out;
}
cbuffer Percentage
{
	float g_fPercentage;
	bool m_isRed;
}
PS_OUT PS_CPPOINT(PS_IN In)
{
	PS_OUT			Out;

	Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexUV);

	if (-0.5f + g_fPercentage <= In.vLocalPos.y)
	{
		discard;
	}

	if (m_isRed)
	{
		Out.vColor.r = 1;
		Out.vColor.g = 0;
		Out.vColor.b = 0;
	}
	else
	{

		Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexUV);

	}

	return Out;
}

//emblem을 위한 쉐이더
PS_OUT PS_MAINSEMBLEM(PS_IN In) {
	PS_OUT Out;

	Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexUV);

	if (Out.vColor.r <= 0.1f && Out.vColor.g <= 0.1 && Out.vColor.b <= 0.1)
	{
		discard;
	}

	//알파블렌드를 지워준다.
	Out.vColor.a = emblemrgb;
	return Out;

}
//exUI 뒷배경을 위한 쉐이더
PS_OUT PS_EXUI(PS_IN In) {
	PS_OUT Out;

	Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexUV);

	if (Out.vColor.r < 0.05)
	{
		discard;
	}

	return Out;

}

//Line을 위한 쉐이더
PS_OUT PS_MAINLINEUI(PS_IN In) {
	PS_OUT			Out;

	Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexUV);

	if (Out.vColor.r < 0.35f && Out.vColor.g < 0.35f&& Out.vColor.b < 0.35f)
	{
		discard;
	}

	return Out;
}
//HPBar 를 위한 쉐이더
PS_OUT PS_MAINHPBar(PS_IN In) {
	PS_OUT Out;

	Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexUV);

	//Out.vColor.a = invenbtnrgb;

	if (Out.vColor.r>0.01  /* && Out.vColor.g >= 0.5&& Out.vColor.b >= 0.5*/)
	{
		discard;
	}
	return Out;
}
//InfoFadeUI를 위하.ㄴ
PS_OUT PS_MAINInfoFadeUI(PS_IN In) {
	PS_OUT Out;

	Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexUV);

	//Out.vColor.a = invenbtnrgb;

	if (Out.vColor.r<0.01  /* && Out.vColor.g >= 0.5&& Out.vColor.b >= 0.5*/)
	{
		discard;
	}
	return Out;
}
PS_OUT PS_MAINHPUI(PS_IN In) {
	PS_OUT			Out;

	Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexUV);

	if (Out.vColor.r >= 1 && Out.vColor.g >= 0.5&& Out.vColor.b >= 0.5)
	{
		discard;
	}

	////알파블렌드를 지워준다.
	//Out.vColor.a = exuirgb;
	return Out;
}
PS_OUT PAALLUIRGB(PS_IN In) {
	PS_OUT			Out;

	Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexUV);


	////알파블렌드를 지워준다.
	Out.vColor.a = alluirgb;
	return Out;
}

cbuffer linemsgrgb
{
	float linemsgrgb;
};
//LineMsgUI를 위한.
PS_OUT PS_LINEMSGUI(PS_IN In) {
	PS_OUT			Out;

	Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexUV);

	Out.vColor.a = linemsgrgb;


	////알파블렌드를 지워준다.
	//Out.vColor.a = exuirgb;
	return Out;
}
//디졸브
PS_OUT PSDISSOLVE(PS_IN In) {
	PS_OUT			Out;

	Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexUV);

	if (Out.vColor.r < Out.vColor.r - 0.4)
		discard;



	return Out;
}
PS_OUT TUTOLINEMSG(PS_IN In) {
	PS_OUT			Out;

	Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexUV);


	Out.vColor.a = tutolinemsgrgb;


	return Out;
}

PS_OUT LINEMSG2(PS_IN In) {
	PS_OUT			Out;

	Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexUV);


	Out.vColor.a = 0.7;

	return Out;
}

//LineMsgUI를 위한.
PS_OUT PS_STATUSUI(PS_IN In) {
	PS_OUT			Out;

	Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexUV);


	if (Out.vColor.r <= 0.002f)
	{
		discard;
	}


	////알파블렌드를 지워준다.
	//Out.vColor.a = exuirgb;
	return Out;
}

PS_OUT PS_MAIN_Picking_Rect(PS_IN In)
{
	PS_OUT			Out;

	Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexUV);

	return Out;
}
cbuffer HORIZONTALPERCENT
{
	float horizontalpercent;
	float g_Redbar;
	bool time;
}

PS_OUT PS_MAIN_PERCENTAGE_HORRIZON(PS_IN In) {
	PS_OUT Out;

	Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexUV);


	if (Out.vColor.r == 0)
	{
		if (horizontalpercent > In.vTexUV.x)
		{
			Out.vColor.r = 0.888;
			Out.vColor.g = 1;
			Out.vColor.b = 0.3;
		}
		else if (g_Redbar > In.vTexUV.x)
		{
			Out.vColor.r = 1;
			Out.vColor.g = 0;
			Out.vColor.b = 0;
		}
	}


	return Out;

}
cbuffer againHP
{
	float lefthpPercent;
	float g_leftRedBar;

}

PS_OUT LEFTHPPERCENT(PS_IN In) {
	PS_OUT Out;

	Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexUV);


	if (Out.vColor.r == 0)
	{
		if (lefthpPercent > In.vTexUV.x)
		{
			Out.vColor.r = 0.888;
			Out.vColor.g = 1;
			Out.vColor.b = 0.3;
		}
		else if (g_leftRedBar > In.vTexUV.x)
		{
			Out.vColor.r = 1;
			Out.vColor.g = 0;
			Out.vColor.b = 0;
		}
	}


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
	pass DefaultRendering  //0
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

	pass SoftEffect	//1
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_SOFT_EFFECT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_SOFT_EFFECT();
	}

	pass BackUI	//2
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_SOFT_EFFECT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_BackUI();
	}
	pass DefaultAlphaBlending //3
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass MouseAlphaBlending //4
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass EMBLEM //5
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		PixelShader = compile ps_5_0 PS_MAINSEMBLEM();

	}
	pass EXUI //6
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		PixelShader = compile ps_5_0 PS_EXUI();

	}

	pass UISmasheffect //7
	{

		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		PixelShader = compile ps_5_0 PS_SMASHEFFECTUI();

	}

	pass Line //8
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_NonBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		PixelShader = compile ps_5_0 PS_MAINLINEUI();

	}

	pass HPBar //9
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		PixelShader = compile ps_5_0 PS_MAINHPBar();

	}

	pass HPPUI //10
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		PixelShader = compile ps_5_0 PS_MAINHPUI();
	}
	pass INFOFADEUI //11
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_NonBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		PixelShader = compile ps_5_0 PS_MAINInfoFadeUI();
	}
	pass LineMSGUI //12
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		PixelShader = compile ps_5_0 PS_LINEMSGUI();
	}

	pass Dissolve //13
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		PixelShader = compile ps_5_0 PSDISSOLVE();
	}



	pass NonAlpha //14
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_NonBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		PixelShader = compile ps_5_0 PS_MAIN();

	}

	pass TutoLIneMsg //15
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		PixelShader = compile ps_5_0 TUTOLINEMSG();

	}
	pass AllUIDA //16
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		PixelShader = compile ps_5_0 PAALLUIRGB();

	}

	pass LINEMSG2 //17
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		PixelShader = compile ps_5_0 LINEMSG2();

	}


	pass statusUI //18
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		PixelShader = compile ps_5_0 PS_STATUSUI();

	}

	pass CPPoint //19
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		PixelShader = compile ps_5_0 PS_CPPOINT();
	}



	pass Navi_Picking_Rect  //20
	{
		SetRasterizerState(RS_WireFrame);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_NonBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_Picking_Rect();
	}

	pass HORIZONTALPASS  //21
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		PixelShader = compile ps_5_0 PS_MAIN_PERCENTAGE_HORRIZON();
	}

	pass LEFTHPPERCENTPASS  //22
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		PixelShader = compile ps_5_0 LEFTHPPERCENT();
	}


}