#include "Common.fx"

struct VS_INPUT
{
	float3 Position : POSITION;
};

struct VS_OUTPUT
{
	float4 Position : POSITION;
	float3 Texcoord : TEXCOORD;
};

float4x4 p_matrix;
float4x4 v_matrix;

samplerCUBE diffuseSampler : register(s0);


//
// Vertex Shader
//
VS_OUTPUT VS(VS_INPUT In)
{
	VS_OUTPUT Out;
	
	Out.Position = mul(float4(In.Position, 1.0f), p_matrix);//����View������4�������λ�����Ǳ��ֺ�����������λ�ò���
	Out.Texcoord = mul(float4(In.Position, 1.0f),v_matrix).xyz;//��matView�����In.ObjPos���б任������ת��պе�Ч��

	return Out;
}

struct PS_OUTPUT
{

	float4 color : COLOR0; 

};

PS_OUTPUT PS(VS_OUTPUT In)
{
	PS_OUTPUT Out;

    Out.color = texCUBE(diffuseSampler, In.Texcoord);
		
	return Out;
}

technique Sky_Effect
{
	pass p0 
	{
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS();
		
	}
}
