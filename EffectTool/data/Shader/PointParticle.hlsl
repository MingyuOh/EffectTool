Texture2D g_txDiffuse : register(t0)
Texture1D g_txRandom  : register(t1);
SamplerState sample0  : register(s0);

#define PT_LAUNCHER		0
#define PT_SHELL		1
#define PT_EMBER1		2
#define PT_EMBER2		3
#define PT_EMBER3		4
#define P_SHELLIFE		3.0f
#define P_EMBER1LIFE	1.5f
#define P_EMBER2LIFE	1.5f
#define P_EMBER3LIFE	2.0f

//--------------------------------------------------------------------------------------
// Constant Buffer
//--------------------------------------------------------------------------------------
cbuffer cb0: register(b0)
{
	matrix	g_matWorld		: packoffset(c0);
	matrix  g_matView		: packoffset(c4);
	matrix  g_matProj		: packoffset(c8);
	float4  g_vColor		: packoffset(c12);
};

cbuffer cb1: register(b1)
{
	D3DXVECTOR4	m_vFrameGravity : packoffset(c0);		// 중력값(파티클이 떨어지는 역할)

	float m_fGlobalTime			: packoffset(c1.x);		// 응용프로그램이 시작된 이후 누적된 시간
	float m_fElapsedTime		: packoffset(c1.y);		// 이전 프레임과 현 프레임의 경과 시간
	float m_fSecondsPerFirework : packoffset(c1.z);		// 불꽃 총알에 더해지는 생명시간(값이 클수록 늦게 터진다.)
	float m_fNumEmber1s			: packoffset(c1.w);		// Ember1 타입의 파티클이 가동 될 때 생성되는 개수 
	float m_pMaxEmber2s			: packoffset(c2.x);		// Ember2 타입의 파티클이 가동 될 때 생성되는 개수 
	float m_fY					: packoffset(c2.y);
	float m_fZ					: packoffset(c2.z);
	float m_fW					: packoffset(c2.w);
};


struct VSPARTICLE_INPUT
{
	float3 vPos	  : POSITION;	// 파티클 위치
	float3 vVel   : NORMAL;		// 파티클 이동 속도
	float  fTimer : TIMER;		// 파티클 생명시간
	uint   Type	  : TYPE;		// 파티클 타입
};

struct VSPARTICLE_OUTPUT
{
	float3 vPos   : POSITION;
	float4 color  : COLOR0;
	float  radius : RADIUS;
};

struct PS_INPUT
{
	float4 vPos  : SV_POSITION;
	float2 tex	 : TEXTURE0;
	float4 color : COLOR0;
};

VSPARTICLE_OUTPUT VS(VSPARTICLE_INPUT input)
{
	VSPARTICLE_OUTPUT output = (VSPARTICLE_OUTPUT)0;

	output.vPos = input.vPos;
	output.radius = 1.0f;

	if (input.Type == PT_LAUNCHER)
	{
		output.color = float4(1, 0, 0, 1);
		output.radius = 1.0f;
	}
	else if (input.Type == PT_SHELL)
	{
		output.color = float4(1, 1, 1, 1);
		output.radius = 1.0f;
	}
	else if (input.Type == PT_EMBER1)
	{
		output.color = float4(1, 1, 0, 1);
		output.color *= (input.fTimer / P_EMBER1LIFE);
		output.radius = 1.0f;
	}
	else if (input.Type == PT_EMBER1)
	{
		output.color = float4(1, 0, 0, 1);
		output.color *= (input.fTimer / P_EMBER2LIFE);
		output.radius = 1.0f;
	}
	else if (input.Type == PT_EMBER1)
	{
		output.color = float4(0, 0, 1, 1);
		output.color *= (input.fTimer / P_EMBER3LIFE);
		output.radius = 1.0f;
	}
	
	return output;
}

float4 PS(PS_INPUT input) : SV_Target
{
	return g_txDiffuse.Sample(sample0, input.tex) * input.color;
}

[maxvertexcount(0)]
void GS(point VSPARTICLE_OUTPUT input[1], inout TriangleStream<PS_INPUT> SpriteStream)
{
	PS_INPUT output;
	const float3 g_positions[4] =
	{
		float3(-1.0f, 1.0f, 0.0f),
		float3(1.0f, 1.0f, 0.0f),
		float3(-1.0f, -1.0f, 0.0f),
		float3(1.0f, -1.0f, 0.0f)
	};

	float4 g_color[4] = 
	{
		float4(1.0f, 0.0f, 0.2f, 1.0f),
		float4(0.0f, 1.0f, 0.0f, 1.0f),
		float4(0.0f, 0.0f, 1.0f,1.0f),
		float4(1.0f, 1.0f,1.0f,  1.0f)
	};

	const float2 g_texcoords[4] =
	{
		float2(0.0f, 0.0f),
		float2(1.0f, 0.0f),
		float2(0.0f, 1.0f),
		float2(1.0f, 1.0f)
	};

	for (int iCnt = 0; iCnt < 4; iCnt++)
	{
		float3 position = g_positions[iCnt] * input[0].radius;
		position = mul(position, (float3x3)g_matWorld);
		output.vPos = mul(float4(position, 1.0f), g_matView);
		output.vPos = mul(position.vPos, g_matProj);
		output.color = input[0].color;
		output.tex = g_texcoords[iCnt];
		SpriteStream.Append(output);
	}
	SpriteStream.RestartStrip();
}

//--------------------------------------------------------------------------------------
// 랜덤 벡터
//--------------------------------------------------------------------------------------
float3 RandomDir(float fOffset)
{
	float tCoord = (g_fGlobalTime + fOffSet) / 300.0f;
	float4 vColor = g_txRandom.SampleLevel(sample0, tCoord, 0);
	return vColor.xyz;
}

[maxvertexcount(128)]
void GSAdvanceParticleMain(point VS_INPUT input[1], inout PointStream<VS_INPUT> ParticleOutputStream)
{
	if (input[0].Type == PT_LAUNCHER)
		GSLauncherHandler(input[0], ParticleOutputStream);
	else if (input[0].Type == PT_SHELL)
		GSShellHandler(input[0], ParticleOutputStream);
	else if (input[0].Type == PT_EMBER1 || input[0].Type == PT_EMBER3)
		GSEmber1Handler(input[0], ParticleOutputStream);
	else if (input[0].Type == PT_EMBER2)
		GSEmber2Handler(input[0], ParticleOutputStream);
}

//--------------------------------------------------------------------------------------
// 타입이 런처인 파티클 핸들러
//--------------------------------------------------------------------------------------
void GSLauncherHandler(VS_INPUT input, inout PointStream<VS_INPUT> ParticleOutputStream)
{
	if (input.fTimer <= 0)
	{
		float3 vRandom = normalize(RandomDir(input.Type));
		VS_INPUT output;
		output.vPos = input.vPos + input.vVel * g_fElapseTime;
		output.vVel = input.vVel + vRandom * 8.0f;
		output.fTimer = P_SHELLIFE + vRandom.y * 0.5f;
		output.Type = PT_SHELL;
		ParticleOutputStream.Append(output);

		// 타이머 리셋
		input.fTimer = g_fSecondsPerFirework + vRandom.x * 0.4f;
	}
	else
	{
		input.fTimer -= g_fElapsedTime;
	}

	ParticleOutputStream.Append(input);
}