float FConduct(float n, float k, float cosi)
{
	float Ret = 0.0f;
	
	float Perp = 0.0f;
    float Parl = 0.0f;

	Parl = ((n * n + k * k) * (cosi * cosi) - 2 * n * cosi + 1) /
			((n * n + k * k) * (cosi * cosi) + 2 * n * cosi + 1);
	
	Perp = ((n * n + k * k) - (2 * n * cosi) + (cosi * cosi)) /
			((n * n + k * k) + (2 * n * cosi) + (cosi * cosi));
	
    Ret = (Parl + Perp) / 2.0f;
	
    return abs(Ret);
}

float FDielect(float4 LightDirection, float4 Normal,float4 Surface, float4 ViewPosition, float nt, float ni)
{
    float4 LD = -normalize(LightDirection);
	
    float4 ViewDir = -normalize(Surface - ViewPosition);
    Normal = normalize(Normal);
	
	float Ret = 0.0f;
	
	float Parl = 0.0f;
	float Perp = 0.0f;
	
	float4 wi = 0.0f, wt = 0.0f;
	
	float ntwi = 0.0f, niwt = 0.0f;
	float ntwt = 0.0f, niwi = 0.0f;
	
	float ldoti, ldott;
	
	wi = LD;
    wt = normalize(refract(LD, Normal, nt));
	
    ldoti = abs(dot(wi, Normal));
    ldott = abs(dot(wt, ViewDir));
	
	ntwi = nt * ldoti;
	niwt = ni * ldott;
	niwi = ni * ldoti;
	ntwt = nt * ldott;
	
	Parl = (ntwi - niwt) / (ntwi + niwt);
	Perp = (niwi - ntwt) / (niwi + ntwt);
	
	Ret = ((Parl * Parl) + (Perp * Perp)) / 2.0f;
	
	return Ret;
}

float BlinnAnisotropic(float3 LightDir, float3 ViewDir, float3 Normal, float ex, float ey)
{
    float3 wi, wo;
    wi = LightDir;
    wo = ViewDir;
    float3 wh = normalize(wi + wo);
    
    float Pi2 = 3.141592f * 2.0f;
    
    float ndotwh = abs(dot(wh, Normal));
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
    
    float3 wi = -LightDir;
    float3 wo = -ViewDir;
    
    
    Coso = dot(wo, Normal);
    Cosi = dot(wi, Normal);
    
    float3 wh = normalize(wi + wo);
    
    float ndotwh = dot(wh, Normal);
    float ndotwo = dot(wo, Normal);
    float ndotwi = dot(wi, Normal);
    float whdotwi = dot(wi, wh);
    
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