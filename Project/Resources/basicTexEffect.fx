
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


PS_INPUT VS ( VS_INPUT input )
{
	PS_INPUT output = ( PS_INPUT ) 0;

	output.ObjWorldPos = mul ( input.Pos , World );

	output.Pos = output.ObjWorldPos;
	output.Pos = mul ( output.Pos , View );
	output.Pos = mul ( output.Pos , Projection );

	output.Normal = mul ( input.Normal , World );
	output.TangentU = input.TangentU;
	output.Tex = input.Tex;

	return output;
}

float Lambertian_BRDF ( float3 lightDir , float3 normal , float reflectance )
{
	float3 lightDirection = -normalize ( lightDir );

	float cos_ni = dot ( normal , lightDirection );

	if (cos_ni < 0)
		return 0;

	return reflectance * cos_ni / 3.1415;
}

float4 PS ( PS_INPUT input ) : SV_Target
{
	// define lighting parameters
	float reflectance = 0.9;
	float BRDF = 0;
	float3 globalAmbient = 0.3 * float3( 0.50f,0.50f,0.50f );
	float3 lightColor = float3( 1.0f,1.0f,0.2f );

	float3 attenuatedLight = float3( 0.0f,0.0f,0.0f );

	// illumination from directional light
	BRDF = Lambertian_BRDF ( DirectionalLightSourceDirection , input.Normal , reflectance );
	attenuatedLight += lightColor * BRDF;

	// illumination from point light
	float3 pointLightDirection = input.ObjWorldPos.xyz - PointLightSourcePosition.xyz;
	BRDF = Lambertian_BRDF ( pointLightDirection , input.Normal , reflectance );
	attenuatedLight += lightColor * BRDF;

	attenuatedLight += globalAmbient;
	return saturate ( g_txDiffuse.Sample ( samLinear , input.Tex ) * float4( attenuatedLight , 1.0f ) );
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





