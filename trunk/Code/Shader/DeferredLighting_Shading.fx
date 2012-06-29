float4x4 wvp_mat;
float4x4 wv_mat;
float3 eyePos;
float4 lightColor;
float3 lightPos;

texture SSAO;
sampler2D SSAOSampler = sampler_state
{
	texture = <SSAO>;
	MinFilter = Point;
	MagFilter = Point;
	MipFilter = Point;   
	AddressU  = Clamp;
	AddressV  = Clamp;
};

texture GBuffer;
sampler2D GBufferSampler = sampler_state
{
	texture = <GBuffer>;
	MinFilter = Point;
	MagFilter = Point;
	MipFilter = Point;   
	AddressU  = Clamp;
	AddressV  = Clamp;
};

texture PosMap;
sampler2D PosMapSampler = sampler_state
{
	texture = <PosMap>;
	MinFilter = Point;
	MagFilter = Point;
	MipFilter = Point;   
	AddressU  = Clamp;
	AddressV  = Clamp;
};

texture ShadowMap;
sampler2D ShadowMapSampler = sampler_state
{
	texture = <ShadowMap>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;   
	AddressU  = Clamp;
	AddressV  = Clamp;
};

texture Tex;
sampler2D TexSampler = sampler_state
{
	Texture = <Tex>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;   
	AddressU  = Clamp;
	AddressV  = Clamp;
};

texture NormalMap;
sampler2D NormalMapSampler = sampler_state
{
	Texture = <NormalMap>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;   
	AddressU  = Clamp;
	AddressV  = Clamp;
};


struct VS_INPUT
{
	float3 Position : POSITION;
};

struct VS_OUTPUT
{
	float4 Position : POSITION;
	float2 Texcoord : Texcoord;
};

VS_OUTPUT vs_main(VS_INPUT In)
{
	VS_OUTPUT Out;
   Out.Position           = float4(sign(In.Position.xy),0.0,1.0);
   Out.Texcoord.x    = (Out.Position.x+1.0) * 0.5;
   Out.Texcoord.y    = 1.0 - ((Out.Position.y+1.0) * 0.5);
	return Out;
}


struct PS_OUTPUT
{
	float4 Image : COLOR0;
};

float ks = 1;
float kd = 0.2;
PS_OUTPUT ps_main(VS_OUTPUT In)
{
    PS_OUTPUT Out;
    
    float4 shadow = tex2D(ShadowMapSampler,In.Texcoord);
    float4 tex= tex2D(TexSampler,In.Texcoord);
    if(shadow.w != 0.5)
    {
     Out.Image= tex;
    }
    else
    {
    float3 pos = tex2D(PosMapSampler,In.Texcoord).xyz;
    
    float3 N = tex2D(NormalMapSampler,In.Texcoord).xyz;
    
    float4 ao = tex2D(SSAOSampler,In.Texcoord);
    
    N.xy=N.xy*2-1;   
    N.z=-sqrt(1-dot(N.xy,N.xy));
    
    float3 L = normalize(lightPos - pos).xyz;
    float3 V = normalize(eyePos - pos);
    float3 H = normalize(L + V);
    
    float4 diffuse =lightColor.y * saturate(dot(N,L));
    float specular = lightColor.w * pow(max(dot(N,H),0),8);
    float brdf = (8 + 8) / 8  * specular;
    Out.Image = tex * (diffuse+brdf)*ao.x * shadow.x;
    }
    return Out;
}


technique Shading_Effect
{
	pass p0 
	{
		VertexShader = compile vs_2_0 vs_main();
		PixelShader = compile ps_2_0 ps_main();

		ALPHABLENDENABLE = FALSE;
		ALPHATESTENABLE = FALSE;
		ZENABLE = TRUE;
		ZWRITEENABLE = TRUE;
	}
}
