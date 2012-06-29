#include "Common.fx"
float4x4 light_mat;
float4x4 wvp_mat;
float4 LightPos;
#define num_samples 9 
float4 vTexOffset[num_samples];


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

texture DepthMap;
sampler2D DepthMapSampler = sampler_state
{
	texture = <DepthMap>;
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
	float4 LightPos : TEXCOORD1;
};



VS_OUTPUT vs_main(VS_INPUT In)
{
	VS_OUTPUT Out;
	
	Out.Position      = mul(float4(In.Position,1.0),light_mat);	
    Out.LightPos      = Out.Position;
   
	return Out;

}

struct PS_OUTPUT
{
   float4 VSM : COLOR0;

};


PS_OUTPUT ps_main(VS_OUTPUT In)
{
    PS_OUTPUT  Out; 
    Out.VSM =In.LightPos.z/In.LightPos.w;
    return Out;
  
}



VS_OUTPUT vs_shadow(VS_INPUT In)
{
    VS_OUTPUT  Out;
	Out.Position      = mul(float4(In.Position,1.0),wvp_mat);
    Out.LightPos      = mul(float4(In.Position,1.0),light_mat);
    return Out;
}

PS_OUTPUT ps_shadow(VS_OUTPUT In)
{
    PS_OUTPUT  Out;
    
    float4 lightSpacePos = (In.LightPos/In.LightPos.w);
    float4 ViewSpacePos = tex2D(DepthMapSampler,lightSpacePos.xy);
   
    float  fLit = 0.0f; 
    
    for ( int i=0; i<num_samples; i++ )
	{
		float4 shadowmap = tex2D(DepthMapSampler, lightSpacePos.xy + vTexOffset[i].xy);
		fLit += (shadowmap.r < lightSpacePos.z) ? 0.05f : 1.0f;
	}
	fLit =saturate(fLit/num_samples);
    
    Out.VSM =float4(fLit,fLit,fLit,0.5);
    return Out;
  
}





technique DepthMap_Effect
{
  pass p0
  {
		VertexShader = compile vs_3_0 vs_main();
		PixelShader = compile ps_3_0 ps_main();
		
		ALPHABLENDENABLE = FALSE;
		ALPHATESTENABLE = FALSE;
		ZENABLE = TRUE;
		ZWRITEENABLE = TRUE;
  }
}


technique VSM_Effect
{
  pass p0
  {
		VertexShader = compile vs_3_0 vs_shadow();
		PixelShader = compile ps_3_0 ps_shadow();
		
		ALPHABLENDENABLE = FALSE;
		ALPHATESTENABLE = FALSE;
		ZENABLE = TRUE;
		ZWRITEENABLE = TRUE;
  }
}