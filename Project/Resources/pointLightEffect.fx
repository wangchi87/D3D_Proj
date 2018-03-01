
cbuffer WorldViewProjMatrix
{
	matrix World;
	matrix View;
	matrix Projection;
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
};


PS_INPUT VS ( VS_INPUT input )
{
	PS_INPUT output = ( PS_INPUT ) 0;
	

	output.Pos = mul ( input.Pos  , World );
	output.Pos = mul ( output.Pos , View );
	output.Pos = mul ( output.Pos , Projection );
	
	output.Normal = input.Normal;
	output.TangentU = input.TangentU;
	output.Tex = input.Tex;
	return output;
}

float4 PS ( PS_INPUT input ) : SV_Target
{

	return float4(1.0f,1.0f,1.0f,1.0f);
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





