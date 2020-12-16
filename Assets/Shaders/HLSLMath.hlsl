float FConduct(float4 LightDirection, float4 Normal, float n, float k)
{
	float Ret = 0.0f;
	
	float Perp = 0.0f;
	float Parl = 0.0f;
	
	float cosi;
	
	cosi = dot(-LightDirection, Normal);
	
	Parl = ((n * n + k * k) * (cosi * cosi) - 2 * n * cosi + 1) /
			((n * n + k * k) * (cosi * cosi) + 2 * n * cosi + 1);
	
	Perp = ((n * n + k * k) - (2 * n * cosi) + (cosi * cosi)) /
			((n * n + k * k) + (2 * n * cosi) + (cosi * cosi));
	
	Ret = (Parl * Parl + Perp * Perp) / 2.0f;
	
    return 1.0f - Ret;
}

float FDielect(float4 LightDirection, float4 Normal, float4 ViewPosition, float nt, float ni)
{
	float4 LD = -LightDirection;
	
	
	float4 ViewDir = normalize(Normal - ViewPosition);
	
	Normal = normalize(-Normal);
	
	float Ret = 0.0f;
	
	float Parl = 0.0f;
	float Perp = 0.0f;
	
	float4 wi = 0.0f, wt = 0.0f;
	
	float4 ntwi = 0.0f, niwt = 0.0f;
	float4 ntwt = 0.0f, niwi = 0.0f;
	
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
	
	Ret = ((Parl * Parl) + (Perp * Perp)) / 2;
	
	return Ret;
}

//float FDielct(float4 LightPosition, float4 Normal, float4 Surface, float nt, float ni)
//{
//	float4 LD = -normalize(Surface - LightPosition);
//	float Distance = distance(LightPosition, Surface);
	
//	float Ret = 0.0f;
	
//	float Parl = 0.0f;
//	float Perp = 0.0f;
	
//	float4 wi = 0.0f, wt = 0.0f;
	
//	float4 ntwi = 0.0f, niwt = 0.0f;
//	float4 ntwt = 0.0f, niwi = 0.0f;
	
//	wi = LD;
//	wt = refract(LD, Normal, nt);
	
//	float cosi, cost;
	
//    cosi = abs(dot(LD, wi)) / Distance;
//    cost = abs(dot(LD, wt)) / Distance;
	
//	ntwi = nt * cosi;
//	niwt = ni * cost;
//	niwi = ni * cosi;
//	ntwt = nt * cost;
	
//	Parl = (ntwi - niwt) / (ntwi + niwt);
//	Perp = (niwi - ntwt) / (niwi + ntwt);
	
//	Ret = ((Parl * Parl) + (Perp * Perp)) / 2.0f;
	
//	return 1.0f - Ret;
//}