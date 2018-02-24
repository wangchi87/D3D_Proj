#pragma once

#ifndef PROJECT_HEADER
#define PROJECT_HEADER

// DXUT header
#include "DXUT\DXUT.h"
#include "DXUTOpt\SDKmisc.h"
using namespace DirectX;

#include <vector>
using namespace std;

#include "Effects11\d3dx11effect.h"

// self-defined header

#include "Camera\FirstPersonCamera.h"
#include "Texture\DDSTextureLoader.h"

struct Vertex
{
	Vertex () {}
	Vertex ( const XMFLOAT3& p , const XMFLOAT3& n , const XMFLOAT3& t , const XMFLOAT2& uv )
		: Position ( p ) , Normal ( n ) , TangentU ( t ) , TexC ( uv ) {}
	Vertex (
		float px , float py , float pz ,
		float nx , float ny , float nz ,
		float tx , float ty , float tz ,
		float u , float v )
		: Position ( px , py , pz ) , Normal ( nx , ny , nz ) ,
		TangentU ( tx , ty , tz ) , TexC ( u , v ) {}

	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT3 TangentU;
	XMFLOAT2 TexC;
};

#endif // PROJECT_HEADER

