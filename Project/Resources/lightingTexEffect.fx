
Texture2D g_txDiffuse;
SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

RasterizerState MyCull 
{
    CullMode = NONE;
};

cbuffer WorldViewProjMatrix
{
	matrix World;
	matrix View;
	matrix Projection;
};


cbuffer Lighting
{	
	// directional light source
	vector DirectionalLightSourceDirection;
	vector PointLightSourcePosition;
	vector CameraPos;
	float  MaterialRoughness;
};


struct VS_INPUT
{
	float4 Pos		: 	POSITION;
	float3 Normal	: 	NORMAL;
	float3 TangentU	:	TANGENTU;
	float2 Tex		:	TEXCOORD;
};

struct PS_INPUT
{
	float4 Pos		: 	SV_POSITION;
	float3 Normal	: 	NORMAL0;
	float3 TangentU	:	TANGENTU0;
	float2 Tex		:	TEXCOORD0;
	float4 ObjWorldPos:  PIXELWORLDPOS0;
};


float Blinn_Phong_BRDF(float3 lightDir, float3 normal, float3 viewDir, float roughness)
{
	float Ks = 0.7;
	float Kd = 0.3;

	float3 lightDirection = - normalize(lightDir);
	
	float3 halfVector = normalize (lightDirection + viewDir);
		
	float cos_ni = dot(normal, lightDirection);
	
	if (cos_ni < 0)
		return 0;
	
	float cos_nh = dot(normal, halfVector);
		
	float diffuse = Kd *  max(cos_ni, 0);
	
	float specular = Ks * pow(max(cos_nh, 0), roughness);
	
	return diffuse + specular;
}

float Cook_Torrance_BRDF(float3 lightDir, float3 normal, float3 viewDir, float roughness)
{
	// fresnel reflectance at zero degree of incidence
	float F0 = 0.7;

	float3 lightDirection = - normalize(lightDir);
	float3 halfVector = normalize (lightDirection + viewDir);
		
	float cos_ni = dot(normal, lightDirection);
	
	if (cos_ni < 0)
		return 0 ;
	
	float cos_nv = dot(normal, viewDir);
	float cos_nh = dot(normal, halfVector);
	float cos_hv = dot(halfVector,viewDir);
		
	// masking and shadowing term
	float nh2 = 2 * cos_nh;
	float g1 = (nh2 * cos_nv) / cos_hv;
	float g2 = (nh2 * cos_ni) / cos_hv;
	float G = min(1.0, min(g1,g2));
	
	// Beckmann micro-facet normal distribution term
	float r1 = 1.0f/( 4.0 * roughness * roughness * pow(cos_nh,4) );
	float r2 = (cos_nh * cos_nh - 1.0 ) / (roughness * roughness * cos_nh * cos_nh );
	float D = r1 * exp(r2);
	
	float F = pow( (1-cos_hv), 5 );
	F = F * ( 1- F0 );
	F = F + F0;
	
	float BRDF = (G * D * F)/(4 * cos_ni * cos_nv);
	
	// add a diffuse term to compensate for multi-scattering term
	return BRDF + (1-F)/3.14;
}

PS_INPUT VS ( VS_INPUT input )
{
	PS_INPUT output = ( PS_INPUT ) 0;
	
	output.ObjWorldPos = mul ( input.Pos  , World );
	
	output.Pos = output.ObjWorldPos;
	output.Pos = mul ( output.Pos , View );
	output.Pos = mul ( output.Pos , Projection );
	
	output.Normal = mul(input.Normal, World);
	output.TangentU = input.TangentU;
	output.Tex = input.Tex;
	
	return output;
}

float4 PS ( PS_INPUT input ) : SV_Target
{
	// get view direction, from object point to camera
	float3 viewDirection = CameraPos.xyz - input.ObjWorldPos.xyz;
	viewDirection = normalize(viewDirection);
	
	// define lighting parameters
	float Kd = 0.5;
	
	float3 globalAmbient = Kd * float3(0.50f,0.50f,0.50f);
	
	float3 lightColor = float3(1.0f,1.0f,0.2f);
	
	float BRDF = 0;
	float3 attenuatedLight = float3(0.0f,0.0f,0.0f);
	
	// ------ contribution of directional light source
	//BRDF = Blinn_Phong_BRDF( DirectionalLightSourceDirection, input.Normal, viewDirection, 2.0f );
	
	BRDF = Cook_Torrance_BRDF( DirectionalLightSourceDirection, input.Normal, viewDirection, MaterialRoughness);
	
	attenuatedLight = lightColor * BRDF + globalAmbient;
	
	// ------ contribution of point light source

	float3 pointLightDirection = input.ObjWorldPos.xyz - PointLightSourcePosition.xyz;
	//pointLightDirection = normalize(pointLightDirection);
	
	BRDF = Cook_Torrance_BRDF( pointLightDirection, input.Normal, viewDirection, MaterialRoughness);
	
	attenuatedLight += lightColor * BRDF;
	
	return saturate( g_txDiffuse.Sample ( samLinear, input.Tex ) * float4(attenuatedLight, 1.0f) );
}



technique11 Render
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_4_0, VS() ) );  
        SetPixelShader( CompileShader( ps_4_0, PS() ) );      
		//SetRasterizerState( MyCull );
	}
}





