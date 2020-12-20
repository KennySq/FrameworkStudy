#include"HLSLMath.hlsl"
#include"Light.hlsl"

SamplerState DefaultSampler : register(s0);

TextureCube Cubemap : register(t0);


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

float BlinnAnisotropic(float3 LightDir, float3 ViewDir, float3 Normal, float ex, float ey)
{
    float3 wi, wo;
    wi = LightDir;
    wo = ViewDir;
    float3 wh = normalize(wi + wo);
    
    float Pi2 = 3.141592f * 2.0f;
    
    float ndotwh = abs(dot(wh, Normal));
   // float D = ((Exp + 2) / (Pi2)) * pow(wh, Exp);
    float d = 1.0f - ndotwh * ndotwh;
    float exsq = (ex * wh.x * wh.x + ey * wh.y * wh.y) / d;
    float D = (sqrt((ex + 2) * (ey + 2))) / Pi2 * pow(ndotwh, exsq);
    
    return D;
}

float Blinn(float4 LightDir, float4 ViewDir, float4 Normal, float Exp)
{
    float4 wi, wo;
    wi = normalize(reflect(ViewDir, -Normal));
    wo = normalize(reflect(LightDir, -Normal));
    float4 wh = normalize(wi + wo);
    
    float4 coswh = abs(cos(dot(-wh, Normal)));
    float ViewLight = abs(dot(-wo, ViewDir));
    float Diffuse = abs(dot(-wi, Normal));
    
    float Pi2 = 3.141592 * 2.0f;
    
    float D = ((Exp + 2) / (Pi2)) * pow(wh, Exp);

    return D;
}

float Torrance(float Diffuse, float3 Normal, float3 LightDir, float3 ViewDir, float Roughness)
{
    float Specular = 0.0f;
    float Coso, Cosi;
    
    Normal = Normal;
    
//	Normal.xyz = normalize(Normal.xyz);
//    LightDir.xyz = normalize(-LightDir.xyz);
  //  float4 wi = reflect(LightDir, Normal);
  //  float4 wo = reflect(ViewDir, Normal); 
    float3 wi = -LightDir;
    float3 wo = -ViewDir;
    
    
    Coso = dot(wo, Normal);
    Cosi = dot(wi, Normal);
    
    float3 wh = normalize(wi + wo);
    
    float ndotwh = dot(wh, Normal);
    float ndotwo = dot(wo, Normal);
    float ndotwi = dot(wi, Normal);
    float whdotwi = dot(wi, wh);
    
  //  float Fr = FConduct(LightDir, Normal, 3.212f, 3.3f, ndotwh); // Chrome fresnel
    float Fr = FConduct(3.212f, 3.3f, ndotwh); // Chrome fresnel
    
    float min1 = (2.0f * ndotwh * ndotwo / whdotwi);
    float min2 = (2.0f * ndotwh * ndotwi / whdotwi);
    
    
    float ndotwhsq = ndotwh * ndotwh;
    float ndotwhqd = ndotwhsq * ndotwhsq;
    
    float Roughsq = saturate(Roughness * Roughness + 0.01f);
    float R = 1.0f / (4.0f * Roughsq * ndotwhqd);
    R *= exp((ndotwhsq - 1) / (ndotwhsq * Roughsq));
    
    float G = min(1.0f, min(min1, min2));
    
    Specular = (G * Fr * R * ndotwh) / (4.0f * Coso * Cosi);
    
    return Specular;
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