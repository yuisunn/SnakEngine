


float2 EncodeViewNormalStereo( float3 n )
{

   float2 enc;
   enc=normalize(n.xy) * sqrt(n.z * 0.5 + 0.5); 
   return enc;
}

float3 DecodeViewNormalStereo(float2 n)
{
   float3 enc;
   enc.xy=n.xy*2-1;   
   enc.z=-sqrt(1-dot(enc.xy,enc.xy));
   return enc;
}



// Don't know why this isn't in the standard library...
float linstep(float min, float max, float v)
{
    return clamp((v - min) / (max - min), 0, 1);
}

// Rescale into [0, 1]
float RescaleDistToLight(float Distance)
{
    return linstep(0.1, 1000, Distance);
}


