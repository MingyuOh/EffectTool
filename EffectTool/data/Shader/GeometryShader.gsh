
//--------------------------------------------------------------------------------------
// Geometry Shader
//--------------------------------------------------------------------------------------

struct VS_OUTPUT
{
	float4 p : SV_POSITION;
	float3 n : NORMAL;
	float4 c : COLOR0;
	float2 t : TEXCOORD0;
};


//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer cb0
{
	matrix	g_matWorld		: packoffset(c0);
	matrix	g_matView		: packoffset(c4);
	matrix	g_matProj		: packoffset(c8);
	float4	g_vColor		: packoffset(c12);
};

[maxvertexcount(4)]
void GS(point VS_OUTPUT input[1], inout TriangleStream<VS_OUTPUT> SpriteStream)
{
	VS_OUTPUT output;

	const float3 g_positions[4] =
	{
		float3(-1.0,  1.0, 0.0),
		float3(1.0,  1.0, 0.0),
		float3(-1.0, -1.0, 0.0),
		float3(1.0, -1.0, 0.0),
	};
	float4 g_color[4] =
	{
		float4(1.0f, 0.0f, 0.0f, 1.0f),
		float4(0.0f, 1.0f, 0.0f, 1.0f),
		float4(0.0f, 0.0f, 1.0f, 1.0f),
		float4(1.0f, 1.0f, 1.0f, 1.0f),
	};
	const float2 g_texcoords[4] =
	{
		float2(0.0, 0.0),
		float2(1.0, 0.0),
		float2(0.0, 1.0),
		float2(1.0, 1.0),
	};

	for (int i = 0; i<4; i++)
	{
		float4 position = float4(g_positions[i], 1.0f);
		position = mul(position, g_matWorld);// +input[0].p;
		position = mul(position, g_matView);
		position = mul(position, g_matProj);

		output.p = position;
		output.n = input[0].n;
		output.c = g_color[i];
		output.t = g_texcoords[i];

		SpriteStream.Append(output);
	}
	SpriteStream.RestartStrip();
}

//[maxvertexcount(9)]
//void GS(triangle VS_OUTPUT input[3], inout TriangleStream<VS_OUTPUT> TriStream)
//{
//	VS_OUTPUT output;
//	// 페이스의 중점을 구한다.
//	float3 centerPos = (input[0].p.xyz + input[1].p.xyz + input[2].p.xyz) / 3.0;
//	float4 centerColor = (input[0].c + input[1].c + input[2].c) / 4.0;
//	float3 centerNormal = (input[0].n + input[1].n + input[2].n) / 3.0;
//	float2 centerTexture = (input[0].t + input[1].t) / 2.0;
//
//	// 정점 당 계산된 중점을 사용하여 페이스를 추가한다.
//	for (int i = 0; i<3; i++)
//	{
//		output.p = input[i].p;
//		output.n = input[i].n;
//		output.c = input[i].c;
//		output.t = input[i].t;
//		TriStream.Append(output);
//
//		int iNext = (i + 1) % 3;
//		output.p = input[iNext].p;
//		output.n = input[iNext].n;
//		output.c = input[iNext].c;
//		output.t = input[iNext].t;
//		TriStream.Append(output);
//
//		output.p = float4(centerPos, 1);
//		output.n = centerNormal;
//		output.c = centerColor;
//		output.t = centerTexture;
//		TriStream.Append(output);
//
//		TriStream.RestartStrip();
//	}
//
//	TriStream.RestartStrip();
//}