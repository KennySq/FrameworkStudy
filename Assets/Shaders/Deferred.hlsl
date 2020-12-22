#include"ConstantBuffers.hlsli"
#include"HLSLMath.hlsl"

SamplerState DefaultSampler : register(s0);

struct Vertex
{
    float4 Position : POSITION0;
    float3 Normal : NORMAL0;
    float2 UV : TEXCOORD0;
};

struct QuadVTP
{
    float4 Position : SV_Position;
    float4 Normal : TEXCOORD1;
    float2 UV : TEXCOORD2;
};

struct DeferredVTP
{
    float4 Position : SV_Position;
    float4 WorldPosition : TEXCOORD0;
    float4 Normal : TEXCOORD1;
    float4 UV : TEXCOORD2;
    float4 ViewPosition : TEXCOORD3;
};

struct DeferredPixel
{
    float4 Projected : SV_Target0;
    float4 WorldPosition : SV_Target1;
    float4 Normal : SV_Target2;
    float4 UV : SV_Target3;
    float4 ViewPosition : SV_Target4;
};

QuadVTP QuadVS(Vertex Input)
{
    QuadVTP Output = (QuadVTP) 0;
    
    Output.Position = Input.Position;
    Output.Normal = float4(Input.Normal, 1.0f);
    Output.UV = Input.UV;
    
    return Output;
}

DeferredVTP DeferredVS(Vertex Input)
{ 
    DeferredVTP Output = (DeferredVTP) 0;
    
    Output.Position = Input.Position;
    Output.Position.w = 1.0f;
    
    Output.Position = mul(Output.Position, World);
    Output.WorldPosition = Output.Position;
    
    Output.Position = mul(Output.Position, View);
    Output.ViewPosition = Output.Position;
    
    Output.Position = mul(Output.Position, Projection);
    
    Output.Normal = mul(Input.Normal, (float3x4) World);
    Output.UV = Input.UV.xyxy;
    
    return Output;
}


DeferredPixel DeferredPS(DeferredVTP Input)
{
    DeferredPixel Output = (DeferredPixel) 0;
    
    Output.Projected = Input.Position;
    Output.Normal = Input.Normal;
    Output.UV = Input.UV;
    Output.ViewPosition = Input.ViewPosition;
    Output.WorldPosition = Input.WorldPosition;
    
    return Output;
}