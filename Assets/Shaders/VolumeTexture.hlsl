RWTexture3D<float> Volume : register(u0);

float rand_1_05(in float2 uv)
{
    float2 noise = (frac(sin(dot(uv, float2(12.9898, 78.233) * 2.0)) * 43758.5453));
    return abs(noise.x + noise.y) * 0.5;
}

[numthreads(4, 4, 4)]
void VolumeTextureCS( uint3 DTid : SV_DispatchThreadID, uint3 Gid : SV_GroupID, uint3 GTid : SV_GroupThreadID, uint Gi : SV_GroupIndex)
{
    //float4 Color = float4(1.0f, 0.0f, 0.0f, 0.0f);
   // float2 uv = float2(DTid.xy + 0.5) / float(128,128);
    Volume[DTid.xyz] = 1.0f;
    
    GroupMemoryBarrierWithGroupSync();
    
    return;   
}