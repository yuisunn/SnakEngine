
struct VS_INPUT
{
	float3 Position : POSITION;
};

struct VS_OUTPUT
{
	float4 Position : POSITION;
	float2 TexCoord : TEXCOORD;
};

float4x4 wvp_matrix;
float4x4 p_matrix;
float3 eyepos_vec;
float4x4 proj_inver;

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
texture RandomNormal;
sampler2D RandomNormalSampler = sampler_state
{
	texture = <RandomNormal>;
	MinFilter = Point;
	MagFilter = Point;
	MipFilter = Point;   
	AddressU  = Clamp;
	AddressV  = Clamp;
};

texture Normal;
sampler2D NormalSampler = sampler_state
{
	texture = <Normal>;
	MinFilter = Point;
	MagFilter = Point;
	MipFilter = Point;   
	AddressU  = Clamp;
	AddressV  = Clamp;
};

//
// Vertex Shader
//
VS_OUTPUT VS_main(VS_INPUT In)
{
	VS_OUTPUT Out;
   Out.Position           = float4(sign(In.Position.xy),0.0,1.0);
   Out.TexCoord.x    = (Out.Position.x+1.0) * 0.5;
   Out.TexCoord.y    = 1.0 - ((Out.Position.y+1.0) * 0.5);
	return Out;
}

float3 getPos(float2 uv)
{
   return (tex2D(PosMapSampler,uv).xyz);
}

float3 getNormal(float2 uv)
{
  float3 n = tex2D(GBufferSampler, uv).xyz;
  n.xy=n.xy*2-1;   
  n.z=-sqrt(1-dot(n.xy,n.xy));
  return n;
  
}

float screen_size = 512;
float random_size = 64;


struct PS_OUTPUT 
{
   float4 AO : COLOR0;
};

float g_sample_rad  = 3.0f;
float g_intensity = 4.8f;
float g_bias = 0.1f;
float g_scale = 0.2f;

float doAmbientOcclusion(in float2 tcoord,in float2 uv, in float3 p, in float3 cnorm)   
{ 
 float3 diff = getPos(tcoord + uv)-p;
 const float3 v = normalize(diff);   
 const float d = length(diff)*g_scale;   
 return max(0.0,dot(cnorm,v)-g_bias)*(1.0/(1.0+d))*g_intensity;   
} 

PS_OUTPUT ps_main(VS_OUTPUT i)   
{   
 PS_OUTPUT o;   
    
 const float2 vec[4] = {float2(1,0),float2(-1,0),   
            float2(0,1),float2(0,-1)};   
 float3 p = getPos(i.TexCoord);   
 //吧 压缩的 法线数据还原
 //float3 n = getNormal(i.TexCoord);
 float3 n = tex2D(NormalSampler,i.TexCoord).xyz;
 // wvp后的深度值用 w 裁剪变换到 0-1 空间后 的值 变换到 view space 的 pos
 float2 rand = (tex2D(RandomNormalSampler,screen_size*i.TexCoord/random_size).xy)*2-1;   
 float ao = 0.0f;   
 float rad = g_sample_rad/p.z;   
 int iterations = 4;   
 for (int j = 0; j < iterations; ++j)   
 {   
  float2 coord1 = reflect(vec[j],rand)*rad;   
  float2 coord2 = float2(coord1.x*0.707 - coord1.y*0.707, coord1.x*0.707 + coord1.y*0.707);   
     
  ao += doAmbientOcclusion(i.TexCoord,coord1*0.25, p, n);   
  ao += doAmbientOcclusion(i.TexCoord,coord2*0.5, p, n);   
  ao += doAmbientOcclusion(i.TexCoord,coord1*0.75, p, n);   
  ao += doAmbientOcclusion(i.TexCoord,coord2, p, n);   
 }    
 ao/=(float)iterations*4.0; 
 float3 final =1-ao;
 o.AO = float4(final,1.0f);
 return o;

 }
 
technique SSAO_Effect
{
	pass p0 
	{
		VertexShader = compile vs_3_0 VS_main();
		PixelShader = compile ps_3_0 ps_main();
		
		ALPHABLENDENABLE = FALSE;
		ALPHATESTENABLE = FALSE;
		ZENABLE = TRUE;
		ZWRITEENABLE = TRUE;
	}
}
texture AO;
sampler2D AOSampler = sampler_state
{
	texture = <AO>;
	MinFilter = Point;
	MagFilter = Point;
	MipFilter = Point;   
	AddressU  = Clamp;
	AddressV  = Clamp;
};
float blurSize = 1.0/512.0;

 







/*
technique blur_Effect
{
        VertexShader = compile vs_3_0 VS_main();
		PixelShader = compile ps_3_0 BlurPS_main();
		
		ALPHABLENDENABLE = FALSE;
		ALPHATESTENABLE = FALSE;
		ZENABLE = TRUE;
		ZWRITEENABLE = TRUE;
	}


}
*/



