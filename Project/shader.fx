

Texture2D txDiff : register( t0 );
SamplerState sampState : register( s0 );

cbuffer ConstantBuffer: register( b0 )
{
	matrix World;
	matrix View;
	matrix Projection;
};

cbuffer ConstantBuffer: register( b1 )
{
	float4 MyColor;
};

struct VS_INPUT
{
	float4 Pos: POSITION;
	float4 Color: COLOR;
	float2 Tex:	TEXCOORD;
};

struct PS_INPUT
{
	float4 Pos: SV_POSITION;
	float4 Color: COLOR0;
	float2 Tex:TEXCOORD0;
};

PS_INPUT VS ( VS_INPUT input )
{
	PS_INPUT output = ( PS_INPUT ) 0;
	
	output.Pos = mul( input.Pos, World);
	output.Pos = mul ( output.Pos , View );
	output.Pos = mul ( output.Pos , Projection );
	output.Color = input.Color;
	output.Tex = input.Tex;
	return output;
}

float4 PS ( PS_INPUT input ) : SV_Target
{
	return txDiff.Sample ( sampState, input.Tex );
	//return input.Color;
}