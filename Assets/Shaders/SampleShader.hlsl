SamplerState DefaultSampler : register(s0);

struct DirectionalLight
{
    float3 Direction;
    float Intensity;
};

struct SpotLight
{
    float3 Position;
    float Radius;
};

cbuffer Instance : register(b0)
{
    matrix World;
    
};

cbuffer Transform : register(b1)
{
    matrix View;
    matrix Projection;
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
    float4 Normal : TEXCOORD0;
    float2 UV : TEXCOORD1;
};

VTP SampleVS(Vertex Input)
{
    VTP Output = (VTP)0;
    
    Output.Position = mul(Input.Position, World);
    Output.Position = mul(Output.Position, View);
    Output.Position = mul(Output.Position, Projection);
    
    Output.Normal = normalize(mul(float4(Input.Normal, 1.0f), World));
    Output.UV = Input.UV;
    
    return Output;

}

float DirectionalDiffuse(DirectionalLight Light, float4 Normal)
{
    Light.Direction = normalize(Light.Direction);
    Normal = normalize(Normal);
    
    return saturate(dot(float4(Light.Direction, 1.0f), Normal));
}

float SpotDiffuse(SpotLight Light, float4 Position, float4 Normal)
{
    float4 LightDir = normalize(Position - float4(Light.Position, 1.0f));
    float Distance = distance(Position, float4(Light.Position, 1.0f));
 
    Normal = normalize(Normal);
    
    return saturate(dot(LightDir, Normal) / Distance);

}

float4 SamplePS(VTP Input) : SV_Target0
{
    float4 Color;
    
    float Diffuse = 0.0f;
    
    for (unsigned int i = 0; i < DirectionalCount; i++)
    {
        Diffuse = DirectionalDiffuse(DirectionalLights[i].Direction, Input.Normal);
    }
    
    for (unsigned int j = 0; j < SpotCount; j++)
    {
        Diffuse += SpotDiffuse(SpotLights[j], Input.Position, Input.Normal);
    }
    
    return Diffuse.xxxx;
}