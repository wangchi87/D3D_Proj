#pragma once

#ifndef PROJECT_HEADER
#define PROJECT_HEADER

// DXUT header
#include <DXUT\DXUT.h>
#include <DXUTOpt\SDKmisc.h>
#include <DXUT\DDSTextureLoader.h>
#include <Effects11\d3dx11effect.h>
using namespace DirectX;

// std header
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

// self-defined content
#include "Camera\FirstPersonCamera.h"

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

struct MeshData
{
	std::vector<Vertex> Vertices;
	std::vector<WORD> Indices;
};

#include "Model\GeometryGenerator.h"

#endif // PROJECT_HEADER

