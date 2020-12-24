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

struct DeferredVTG
{
    float4 Position : POSITION0;
    float4 WorldPosition : TEXCOORD0;
    float4 Normal : TEXCOORD1;
    float4 UV : TEXCOORD2;
    float4 ViewPosition : TEXCOORD3;
};

struct DeferredGTP
{   
    float4 Position : SV_Position;
    float4 WorldPosition : TEXCOORD0;
    float4 Normal : TEXCOORD1;
    float4 UV : TEXCOORD2;
    float4 ViewPosition : TEXCOORD3;
    float4 MaterialID : TEXCOORD4;
};



struct DeferredPixel
{
    float4 Projected : SV_Target0;
    float4 WorldPosition : SV_Target1;
    float4 ViewPosition : SV_Target2;
    float4 Normal : SV_Target3;
    float4 UV : SV_Target4;
    float4 MaterialID : SV_Target5;
    
};

Texture2D<float4> Buffer_Proected : register(t0);
Texture2D<float4> Buffer_WorldPosition : register(t1);
Texture2D<float4> Buffer_ViewPosition : register(t2);
Texture2D<float4> Buffer_Normal : register(t3);
Texture2D<float4> Buffer_UV : register(t4);
Texture2D<float4> Buffer_MaterialID : register(t5);

QuadVTP QuadVS(Vertex Input)
{
    QuadVTP Output = (QuadVTP) 0;
    
    Output.Position = Input.Position;
    Output.Normal = float4(Input.Normal, 1.0f);
    Output.UV = Input.UV;
    
    return Output;
}

float4 QuadPS(QuadVTP Input) : SV_Target0
{
    float4 DiffuseColor;
    
    float4 SampleProjected = Buffer_Proected.Sample(DefaultSampler, Input.UV);
    float4 SampleWorldPosition = Buffer_WorldPosition.Sample(DefaultSampler, Input.UV);
    float4 SampleViewPosition = Buffer_ViewPosition.Sample(DefaultSampler, Input.UV);
    float4 SampleNormal = Buffer_Normal.Sample(DefaultSampler, Input.UV);
    float4 SampleUV = Buffer_UV.Sample(DefaultSampler, Input.UV);
    
    float3 LightDir;
    float3 ViewDir = normalize(SampleNormal - ViewPosition).xyz;
    float Diffuse = 0.0f;
    
    for (unsigned int i = 0; i < DirectionalCount; i++)
    {
        LightDir = normalize(DirectionalLights[i].Direction.xyz);
        Diffuse += BlinnAnisotropic(LightDir, ViewDir, SampleNormal.xyz,10.0f,10.0f);
        Diffuse += Torrance(Diffuse, SampleNormal.xyz, LightDir, ViewDir, 0.5f);
    }
    
    DiffuseColor = Diffuse.xxxx;
    
    return DiffuseColor;
}

DeferredGTP DeferredVS(Vertex Input)
{ 
    DeferredGTP Output = (DeferredGTP) 0;
    
    Output.Position = Input.Position;
    Output.Position.w = 1.0f;
    
    Output.Position = mul(Output.Position, World);
    Output.WorldPosition = Output.Position;
    
    Output.Position = mul(Output.Position, View);
    Output.ViewPosition = Output.Position;
    
    Output.Position = mul(Output.Position, Projection);
    
    Output.Normal = mul(Input.Normal, (float3x4) World);
    Output.UV = Input.UV.xyxy;
    
    //Output.MaterialID = Input.
    
    return Output;
}

//[maxvertexcount(4)]
//void DeferredGS(point DeferredVTG Input[1], inout PointStream<DeferredGTP> Output)
//{
//    DeferredGTP Output = (DeferredGTP) 0;
    
//    Output.Position = Input[0].Position;
//    Output.Position.w = 1.0f;
    
//    Output.Position = mul(Output.Position, World);
//    Output.WorldPosition = Output.Position;
    
//    Output.Position = mul(Output.Position, View);
//    Output.ViewPosition = Output.Position;
    
//    Output.Position = mul(Output.Position, Projection);
    
//    Output.Normal = mul(Input[0].Normal, World);
//    Output.UV = Input[0].UV;
//}

DeferredPixel DeferredPS(DeferredGTP Input)
{
    DeferredPixel Output = (DeferredPixel) 0;
    
    Output.Projected = Input.Position;
    Output.Normal = Input.Normal;
    Output.UV = Input.UV;
    Output.ViewPosition= Input.ViewPosition;
    Output.WorldPosition = Input.WorldPosition;
    
    return Output;
}