
cbuffer Matrices
{
	float4x4		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
}

cbuffer CellColor
{
	float4		g_vColor;
};

struct VS_IN
{
	float3		vPosition : POSITION;
	float4		vColor : COLOR0;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float4		vColor : COLOR0;	
};



VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	float3		vPosition = In.vPosition;

	

	if (all(g_vColor == float4(1.f, 0.f, 0.f, 1.f)))
		vPosition.y += 0.1f;

	Out.vPosition = mul(vector(vPosition, 1.f), matWVP);
	Out.vColor = In.vColor * g_vColor;	

	return Out;
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float4		vColor : COLOR0;
};

struct PS_OUT
{
	vector			vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out;

	Out.vColor = In.vColor;	

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
};

DepthStencilState DSS_Default
{
	DepthEnable = true;
	DepthWriteMask = all;
	DepthFunc = less_equal;
};

technique11 DefaultTechnique
{
	pass DefaultRendering
	{		
		SetRasterizerState(RS_Default);		
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_NonBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

}


