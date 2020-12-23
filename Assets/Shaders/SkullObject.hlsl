#include"Deferred.hlsl"

TextureCube Cubemap : register(t0);
float4 SkullObjectPS(DeferredGTP Input ) : SV_Target0
{
    float4 Color;
    
    float4 Gold = float4(0.9f, 0.7f, 0.01f, 1.0f);
    float3 LightDir;
    float Diffuse = 0.0f;
    float Coso = 0.0f;
    
    const float Roughness = 0.5f;
    
    float3 ViewDir = normalize(Input.Normal.xyz - ViewPosition.xyz);
    float3 Normal = normalize(Input.Normal.xyz);
    
    
    
    for (unsigned int i = 0; i < DirectionalCount; i++)
    {
        LightDir = normalize(DirectionalLights[i].Direction.xyz);
       // Diffuse += BlinnAnisotropic(LightDir, ViewDir, Normal,1.0f,1.0f);
        Diffuse += Torrance(Diffuse, Normal, LightDir, ViewDir, Roughness);

        
        

    }
    
    for (unsigned int j = 0; j < SpotCount; j++)
    {
    //  Diffuse += SpotDiffuse(SpotLights[j], Input.WorldPosition, Input.Normal);
    //   Diffuse += FDielct(float4(SpotLights[j].Position, 1.0f),
    //                        Input.Normal, Input.WorldPosition, 1.0f, 1.0f);

    }
    float3 Ref = reflect(normalize(ViewDir), Normal);
    float4 Cube = Cubemap.Sample(DefaultSampler, Ref.xyz);
    float4 Ambient = float4(0.05f, 0.05f, 0.05f, 1.0f);
    float4 Albedo = float4(0.3f, 0.3f, 0.3f, 1.0f);
   // return (Cube);
    return (float4(Diffuse.xxx, 1.0f)) * (Cube * (1.0f - Roughness));
}