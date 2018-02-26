
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
	vector LightDir;
	vector CameraPos;
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
	float3 viewDirection : VIEWDIR0;
};


PS_INPUT VS ( VS_INPUT input )
{
	PS_INPUT output = ( PS_INPUT ) 0;
	float4 worldPosition;
	
	worldPosition = mul ( input.Pos  , World );
	
	output.Pos = worldPosition;
	output.Pos = mul ( output.Pos , View );
	output.Pos = mul ( output.Pos , Projection );
	
	output.Normal = mul(input.Normal, World);
	output.TangentU = input.TangentU;
	output.Tex = input.Tex;
	
	// from point to camera
	output.viewDirection = CameraPos.xyz - worldPosition.xyz;
	output.viewDirection = normalize(output.viewDirection);
	
	return output;
}

float4 PS ( PS_INPUT input ) : SV_Target
{
	// blinn-phong BRDF model;
	float Ks = 0.9;
	float Kd = 0.1;
	float shininess = 2;
	float3 globalAmbient = float3(0.0f,0.0f,0.9f);
	float3 lightColor = float3(1.0f,1.0f,0.2f);
	
	float3 lightDirection = - normalize(LightDir);
	
	float3 halfVector = normalize (lightDirection + input.viewDirection);
	
	if( dot(halfVector, lightDirection) < 0 )
		return float4(1.0f, 1.0f, 1.0f, 1.0f ) ;
	if( dot(halfVector, input.viewDirection) <0 )
		return float4(1.0f, 1.0f, 1.0f, 1.0f ) ;

		
	float cos_ni = dot(input.Normal, lightDirection);
	
	float cos_nh = dot(input.Normal, halfVector);
		
	float3 diffuseColor = Kd * globalAmbient + Kd * lightColor * max(cos_ni, 0);
	
	float3 specularColor = Ks * lightColor * pow(max(cos_nh, 0), shininess);
	
	return saturate( g_txDiffuse.Sample ( samLinear, input.Tex ) * float4((diffuseColor + specularColor),1.0f) );

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





