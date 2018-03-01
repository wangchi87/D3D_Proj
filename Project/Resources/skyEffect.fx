
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

cbuffer ConstantBuffer
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
	output.Tex = input.Tex * 20;
	return output;
}

float4 PS ( PS_INPUT input ) : SV_Target
{
	float4 apexColor = float4(0.0f, 0.05f, 0.6f, 1.0f);
	float4 centerColor = float4(0.0f, 0.5f, 0.8f, 1.0f);
	
	float height = input.Pos.y/200;
	if(height<0)
		height = 0;
	
	return lerp(apexColor,centerColor,height);
	
	//return float4(1.0f,0,0,1.0f);
	//return g_txDiffuse.Sample ( samLinear, input.Tex );

}

technique11 Render
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_4_0, VS() ) );  
        SetPixelShader( CompileShader( ps_4_0, PS() ) );      
		SetRasterizerState( MyCull );
	}
}





