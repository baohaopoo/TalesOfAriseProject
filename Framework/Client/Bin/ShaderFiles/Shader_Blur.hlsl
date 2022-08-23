Texture2D<float4> Input : register(t0);
RWTexture2D<float4> Output : register(u0);

cbuffer cb0
{
	int2 Res : packoffset(c0);
}

void DownScale4x4(uint2 CurPixel, uint groupThreadId)
{
	if (CurPixel.y < Res.y)
	{
		int3 nFullResPos = int3(CurPixel * 4, 0);
		float4 downScaled = float4(0.0, 0.0, 0.0, 0.0);
		[unroll]
		for (int i = 0; i < 4; i++)
		{
			[unroll]
			for (int j = 0; j < 4; j++)
			{
				downScaled += Input.Load(nFullResPos, int2(j, i));
			}
		}
		downScaled /= 16.0;
		Output[CurPixel.xy] = downScaled;
	}
}

[numthreads(1024, 1, 1)]
void CS_MAIN_DOWNSCALE4X4(uint3 groupID : SV_GroupID, uint3 dispatchThreadID : SV_DispatchThreadID, uint3 groupThreadID : SV_GroupThreadID)
{
	uint2 CurPixel = uint2(dispatchThreadID.x % Res.x, dispatchThreadID.x / Res.x);
	DownScale4x4(CurPixel, groupThreadID.x);
}

static const float SampleWeights[13] = {
	0.002216,
	0.008764,
	0.026995,
	0.064759,
	0.120985,
	0.176033,
	0.199471,
	0.176033,
	0.120985,
	0.064759,
	0.026995,
	0.008764,
	0.002216,
};

#define kernelhalf 6
#define groupthreads 128
groupshared float4 SharedInput[groupthreads];

[numthreads(groupthreads, 1, 1)]
void CS_MAIN_VERTICALBLUR(uint3 Gid : SV_GroupID, uint GI : SV_GroupIndex)
{
	int2 coord = int2(Gid.x, GI - kernelhalf + (groupthreads - kernelhalf * 2) * Gid.y);
	coord = clamp(coord, int2(0, 0), int2(Res.x - 1, Res.y - 1));
	SharedInput[GI] = Input.Load(int3(coord, 0));

	GroupMemoryBarrierWithGroupSync();

	if (GI >= kernelhalf && GI < (groupthreads - kernelhalf) &&
		((GI - kernelhalf + (groupthreads - kernelhalf * 2) * Gid.y) < Res.y))
	{
		float4 vOut = 0;

		[unroll]
		for (int i = -kernelhalf; i <= kernelhalf; ++i)
		{
			vOut += SharedInput[GI + i] * SampleWeights[i + kernelhalf];
		}

		Output[coord] = float4(vOut.rgb, 1.0f);
	}
}

[numthreads(groupthreads, 1, 1)]
void CS_MAIN_HORIZONBLUR(uint3 Gid : SV_GroupID, uint GI : SV_GroupIndex)
{
	int2 coord = int2(GI - kernelhalf + (groupthreads - kernelhalf * 2) * Gid.x, Gid.y);
	coord = clamp(coord, int2(0, 0), int2(Res.x - 1, Res.y - 1));
	SharedInput[GI] = Input.Load(int3(coord, 0));

	GroupMemoryBarrierWithGroupSync();

	if (GI >= kernelhalf && GI < (groupthreads - kernelhalf) &&
		((Gid.x * (groupthreads - 2 * kernelhalf) + GI - kernelhalf) < Res.x))
	{
		float4 vOut = 0;

		[unroll]
		for (int i = -kernelhalf; i <= kernelhalf; ++i)
			vOut += SharedInput[GI + i] * SampleWeights[i + kernelhalf];

		Output[coord] = float4(vOut.rgb, 1.0f);
	}
}

technique11 DefaultTechnique
{
	pass Vertical//0
	{
		VertexShader = NULL;
		GeometryShader = NULL;
		PixelShader = NULL;
		ComputeShader = compile cs_5_0 CS_MAIN_VERTICALBLUR();
	}

	pass Horizon//1
	{
		VertexShader = NULL;
		GeometryShader = NULL;
		PixelShader = NULL;
		ComputeShader = compile cs_5_0 CS_MAIN_HORIZONBLUR();
	}

	pass DownScale//2
	{
		VertexShader = NULL;
		GeometryShader = NULL;
		PixelShader = NULL;
		ComputeShader = compile cs_5_0 CS_MAIN_DOWNSCALE4X4();
	}
}