#include"Light.hlsl"

SamplerState DefaultSampler : register(s0);

cbuffer Instance : register(b0)
{
    matrix World;
    
};

cbuffer Transform : register(b1)
{
    matrix View;
    matrix Projection;
    float4 ViewPosition;
};

cbuffer Scene : register(b2)
{
    DirectionalLight DirectionalLights[1024];
    SpotLight SpotLights[1024];
    
    uint DirectionalCount;
    uint SpotCount;
    uint Padding;
    uint Padding2;
};

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
};

TextureCube SkyboxTexture : register(t0);

VTP SkyboxVS(Vertex Input)
{
    VTP Output = (VTP) 0;
    
    Output.Position = Input.Position;
    Output.Position.w = 1.0f;
    
    Output.Position = mul(Output.Position, World);
    Output.WorldPosition = Output.Position;
    
    Output.Position = mul(Output.Position, View);
    Output.Position = mul(Output.Position, Projection);
    
    Output.Normal = mul(float4(Input.Normal, 1.0f), World);
    Output.UV = Input.UV;
    
    return Output;
}

float4 SkyboxPS(VTP Input) : SV_Target0
{
   // float4 Color = SkyboxTexture.Sample(DefaultSampler, Input.Normal.xyz);
    
    return float4(1.0f, 1.0f, 1.0f, 1.0f);
}