//--------------------------------------------------------------------------------------
// File: Tutorial11.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------

cbuffer cbChangesEveryFrame
{
    matrix World;
    matrix View;
    matrix Projection;
};


struct VS_INPUT
{
	float4 Pos	: 	POSITION;
	float4 Color: 	COLOR;
	float2 Tex	:	TEXCOORD;
};

struct PS_INPUT
{
	float4 Pos	: 	SV_POSITION;
	float4 Color: 	COLOR0;
	float2 Tex	:	TEXCOORD0;
};



//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------

PS_INPUT VS ( VS_INPUT input )
{
	PS_INPUT output = ( PS_INPUT ) 0;
	
	output.Pos = mul ( input.Pos  , World );
	output.Pos = mul ( output.Pos , View );
	output.Pos = mul ( output.Pos , Projection );

	output.Color = input.Color;
	output.Tex = input.Tex;
	return output;
}

float4 PS ( PS_INPUT input ) : SV_Target
{
	//return txDiff.Sample ( sampState, input.Tex );
	return input.Color;
}



//--------------------------------------------------------------------------------------
// Technique
//--------------------------------------------------------------------------------------
technique11 Render
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );        

    }
}

