#pragma once
#include "ProjectHeader.h"

struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};

struct VertexColorTex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
	XMFLOAT2 Tex;
};

struct VertexTex
{
	XMFLOAT3 Pos;
	XMFLOAT2 Tex;
};

struct ConstBufColor
{
	XMFLOAT4 Color;
};

struct ConstantBufferProjection
{
	XMMATRIX World;
	XMMATRIX View;
	XMMATRIX Projection;
};

class BaseModel
{
public:
	BaseModel ();
	~BaseModel ();
};

