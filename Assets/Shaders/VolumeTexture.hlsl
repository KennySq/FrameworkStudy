RWTexture3D<uint> Volume : register(u0);

[numthreads(4, 4, 4)]
void VolumeTextureCS( uint3 DTid : SV_DispatchThreadID, uint3 Gid : SV_GroupID, uint3 GTid : SV_GroupThreadID, uint Gi : SV_GroupIndex)
{
    //float4 Color = float4(1.0f, 0.0f, 0.0f, 0.0f);
    
    Volume[DTid.xyz] = 1;
    
    GroupMemoryBarrierWithGroupSync();
    
    return;   
}