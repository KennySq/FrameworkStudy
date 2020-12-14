#include"HLSLMath.hlsl"
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

VTP SampleVS(Vertex Input)
{
    VTP Output = (VTP)0;
    
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

float Blinn(float4 LightDir, float4 ViewDir, float4 Normal, float Exp)
{
    float4 wi, wo;
    wi = LightDir;
    wo = reflect(LightDir, Normal);
    float4 wh = normalize(wi + wo);
    
    float4 coswh = abs(cos(dot(-wh, Normal)));
    float ViewLight = abs(dot(-wo, ViewDir));
    float Diffuse = abs(dot(-wi, Normal));
    
    float Pi2 = 3.141592 * 2.0f;
    
    float D = ((Exp + 2) / (Pi2)) * pow(wh, Exp);

    return D;
}

float Torrance(float Fresnel, float Diffuse, float4 Normal, float4 LightDir, float4 ViewPos, float4 Surface)
{
    float Ret = 0.0f;
    
    float4 ViewDir = normalize(Surface - ViewPos);
    float Coso, Cosi;
    
    float4 wi = LightDir;
    float wo = reflect(wi, Normal);
    
    Coso = abs(dot(-ViewDir, Normal));
    Cosi = abs(dot(-LightDir, Normal));
    
   // float4 Wh = normalize( Cosi + Coso);
    float4 Wh = normalize(wi + wo); //abs(dot(ViewDir, -Normal));
    
    float Roughness;
    

    
    float min1 = (2.0f * (Normal * Wh) * (Normal * wo)) / (wo * Wh);
    float min2 = (2.0f * (Normal * Wh) * (Normal * wi)) / (wo * Wh);
    
    Roughness = min(1, min(min1, min2));
    
    Ret = (Roughness *Fresnel * Wh) / (4.0f * Coso * Cosi);
    
    return saturate(Ret);
}

float4 SamplePS(VTP Input) : SV_Target0
{
    float4 Color;
    
    float4 Gold = float4(0.8f, 0.6f, 0.01f, 1.0f);
    float Diffuse = 0.0f;
    
    float4 ViewDir = normalize(Input.Normal - ViewPosition);
    
    for (unsigned int i = 0; i < DirectionalCount; i++)
    {
        Diffuse += Blinn(float4(DirectionalLights[i].Direction, 1.0f), ViewDir, Input.Normal, 2.0f);
        
       
    }
    
    //for (unsigned int i = 0; i < DirectionalCount;i++)
    //{
    //    float Fr = FDielect(float4(DirectionalLights[i].Direction, 1.0f), Input.Normal, 0.5f, 1.0f);
        
    //    Diffuse += Torrance(Fr, saturate(Diffuse), Input.Normal, float4(DirectionalLights[i].Direction, 1.0f), ViewPosition, Input.WorldPosition);

    //}
    
        for (unsigned int j = 0; j < SpotCount; j++)
        {
      //  Diffuse += SpotDiffuse(SpotLights[j], Input.WorldPosition, Input.Normal);
     //   Diffuse += FDielct(float4(SpotLights[j].Position, 1.0f),
     //                        Input.Normal, Input.WorldPosition, 1.0f, 1.0f);

        }
    
    
    
    return Diffuse.xxxx;
}