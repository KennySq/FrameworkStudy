#include"ConstantBuffers.hlsli"
#include"HLSLMath.hlsl"

SamplerState DefaultSampler : register(s0);

struct Vertex
{
    float4 Position : POSITION0;
    float3 Normal : NORMAL0;
    float2 UV : TEXCOORD0;
};

struct VTP
{
    float4 Position : SV_Position;
    float4 WorldPosition : TEXCOORD0;
    float4 Normal : TEXCOORD1;
    float2 UV : TEXCOORD2;
    float4 ViewPosition : TEXCOORD3;
};

VTP SampleVS(Vertex Input)
{
    VTP Output = (VTP)0;
    
    Output.Position = Input.Position;
    Output.Position.w = 1.0f;
    
    Output.Position = mul(Output.Position, World);
    Output.WorldPosition = Output.Position;
    
    Output.Position = mul(Output.Position, View);
    Output.ViewPosition = Output.Position;
    
    Output.Position = mul(Output.Position, Projection);
    
    Output.Normal = mul(Input.Normal, (float3x4)World);
    Output.UV = Input.UV;
    
    return Output;
}

float DirectionalDiffuse(DirectionalLight Light, float4 Normal)
{
    Light.Direction = normalize(Light.Direction);
    Normal = normalize(Normal);
    
    return saturate(dot(float4(Light.Direction, 1.0f) * Light.Intensity, Normal));
}

float SpotDiffuse(SpotLight Light, float4 Position, float4 Normal)
{
    float4 LightDir = normalize(Position - float4(Light.Position, 1.0f));
    float Distance = distance(Position, float4(Light.Position, 1.0f));
 
    Normal = normalize(Normal);
    
    return saturate(dot(-LightDir, Normal) * (Light.Radius / Distance));

}



float4 SamplePS(VTP Input) : SV_Target0
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
    float4 Ambient = float4(0.05f, 0.05f, 0.05f,1.0f);
    float4 Albedo = float4(0.3f, 0.3f, 0.3f, 1.0f);
   // return (Cube);
    return (float4(Diffuse.xxx, 1.0f)) * (Cube * (1.0f - Roughness)) * Gold;
}