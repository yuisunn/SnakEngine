#include "Common.fx"

struct VS_INPUT
{
	float3 Position : POSITION;
	float3 Normal   : NORMAL;
	float2 Texcoord : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 Position : POSITION;
	float2 Texcoord : TEXCOORD;
	float4 W_Pos : TEXCOORD1;
	float4 W_Normal : TEXCOORD2;
	float4 WVP_Pos : TEXCOORD3;
};

float4x4 wvp_matrix;
float4x4 wv_matrix;
float4x4 w_matrix;
float4x4 light_mat;
float4 eyePos;

float far;
float near;

texture diffuseTexture;
sampler2D diffuseSampler : register(s0) = sampler_state
{
	texture = <diffuseTexture>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;   
	AddressU  = Wrap;
	AddressV  = Wrap;
};

//
// Vertex Shader
//
VS_OUTPUT VS(VS_INPUT In)
{
	VS_OUTPUT Out;
	
	Out.Position = mul(float4(In.Position, 1.0f), wvp_matrix);
	Out.Texcoord = In.Texcoord;

	Out.W_Pos = mul(float4(In.Position, 1.0f), w_matrix);
	Out.W_Normal =mul(float4(In.Normal,1),w_matrix);
	Out.WVP_Pos = Out.Position;
	return Out;
}

struct PS_OUTPUT
{

	float4 GBuffer : COLOR0; 
	float4 PosMap : COLOR1;
	float4 tex : COLOR2;
	float4 NormalMap : COLOR3;
};

PS_OUTPUT PS(VS_OUTPUT In)
{
	PS_OUTPUT Out;

	//Ñ¹Ëõ·¨Ïß
	float2  n = normalize(In.W_Normal.xy) * sqrt(In.W_Normal.z * 0.5 + 0.5);
	
	float depth = In.WVP_Pos.z/In.WVP_Pos.w;//RescaleDistToLight(length(eyePos-In.W_Pos));
    Out.tex = tex2D(diffuseSampler,In.Texcoord);
    Out.PosMap = In.W_Pos;
    Out.GBuffer = 0.5f;
    Out.NormalMap = In.W_Normal;
		
	return Out;
}

technique GBuffer_Effect
{
	pass p0 
	{
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS();
		
	}
}
