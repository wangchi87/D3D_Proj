
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
};

struct PS_INPUT
{
	float4 Pos: SV_POSITION;
	float4 Color: COLOR0;
};

PS_INPUT VS ( VS_INPUT input )
{
	PS_INPUT output = ( PS_INPUT ) 0;
	
	output.Pos = input.Pos;
	output.Color = input.Color;

	return output;
}

float4 PS ( PS_INPUT input ) : SV_Target
{
	return MyColor;
}