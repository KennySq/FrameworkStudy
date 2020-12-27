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

Texture3D Tex_Volume : register(t6);

QuadVTP QuadVS(Vertex Input)
{
    QuadVTP Output = (QuadVTP) 0;
    
    Output.Position = Input.Position;
    Output.Normal = float4(Input.Normal, 1.0f);
    Output.UV = Input.UV;
    
    return Output;
}

float GetDist(float3 Point)
{
    float4 VolumePos = float4(0.0f, 1.0f, 6.0f, 1.0f);

    float DistanceVolume = length(Point - VolumePos.xyz) - VolumePos.w;
    float DistancePoint = Point.y;
    float Distance = min(DistanceVolume, DistancePoint);
    
    return Distance;

}

float4 RayMarch(float2 UV,float3 ViewPos, float3 ViewDir, int Depth)
{
    float4 Sam;
    
    const uint MaxStep = 256;
    
    float SampleVolume = Tex_Volume.Load(int4(UV.xy, 0, 0), int3(0, 0, 0)).xxxx;
    
    float dO = 0.f;
    float3 ro = ViewPos;
    float3 rd = ViewDir;
    
    for (int i = 0; i < MaxStep;i++)
    {
        float3 Point = ro + rd * dO;
        float dS = GetDist(Point);
        dO += dS;
        if (dO > 100.0f || dS < 0.01f)
            break;

    }
    
    return dO;
}

float4 QuadPS(QuadVTP Input) : SV_Target0
{
    float4 DiffuseColor;
    float2 UVOffset = float2(1.0f, 1.0f) / float2(800, 600);
    
    float2 AdjustedUV = Input.UV + UVOffset;
    
    float4 SampleProjected = Buffer_Proected.Sample(DefaultSampler, AdjustedUV);
    float4 SampleWorldPosition = Buffer_WorldPosition.Sample(DefaultSampler, AdjustedUV);
    float4 SampleViewPosition = Buffer_ViewPosition.Sample(DefaultSampler, AdjustedUV);
    float4 SampleNormal = Buffer_Normal.Sample(DefaultSampler, AdjustedUV);
    float4 SampleUV = Buffer_UV.Sample(DefaultSampler, AdjustedUV);
    float4 SampleMaterialID = Buffer_MaterialID.Sample(DefaultSampler, AdjustedUV);
    
    float4 SampleVolumeTexture = Tex_Volume.Load(int4(AdjustedUV.xy, 0, 0), int3(0, 0, 0)).xxxx;
    
    float3 LightDir;
    float3 ViewDir = normalize(SampleNormal - ViewPosition).xyz;
    float Diffuse = 0.0f;
    
    for (unsigned int i = 0; i < DirectionalCount; i++)
    {
        LightDir = normalize(DirectionalLights[i].Direction.xyz);
        Diffuse += BlinnAnisotropic(LightDir, ViewDir, SampleNormal.xyz,10.0f,10.0f);
        Diffuse += Torrance(Diffuse, SampleNormal.xyz, LightDir, ViewDir, 0.5f);
    }
    
    float RM = RayMarch(AdjustedUV, ViewPosition.xyz, normalize(float3(AdjustedUV, 0.0f)), 1);
   
    DiffuseColor = Diffuse.xxxx;
    
    return DiffuseColor;
   // return float4(float3(AdjustedUV, 0.0f), 0.0f);
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
    
    Output.MaterialID = MaterialID.xxxx;
    
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
    Output.MaterialID = Input.MaterialID;
    return Output;
}