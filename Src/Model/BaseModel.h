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

//struct ConstBufColor
//{
//	XMFLOAT4 Color;
//};

struct ConstBufMatrix1
{
	XMMATRIX Mat;
};

//
//struct ConstantBufferProjection
//{
//	XMMATRIX World;
//	XMMATRIX View;
//	XMMATRIX Projection;
//};

class BaseModel
{
protected:
	ID3D11Device*				pd3dDevice;
	ID3D11DeviceContext*		pd3dImmediateContext;
	const DXGI_SURFACE_DESC*	pBackBufferSurfaceDesc;

	void*						pUserContext;

	DWORD						dwShaderFlags;
	UINT						vertexIndicesNum;

	ID3D11InputLayout*			g_pVertexLayout;
	ID3D11Buffer*				constBufWorld;


	ID3DX11Effect*					g_pEffect;
	
	ID3DX11EffectMatrixVariable*	worldVariable;
	ID3DX11EffectMatrixVariable*	viewVariable;
	ID3DX11EffectMatrixVariable*	projVariable;

public:
	BaseModel ();
	BaseModel ( 
		ID3D11Device* pd3dDevice ,
		const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc ,
		void* pUserContext
		);

	virtual void InitDeviceHandle ( 
		ID3D11Device* pd3dDevice ,
		const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc ,
		void* pUserContext );

	virtual void RenderScene ( 
		double fTime , 
		float fElapsedTime , 
		void* pUserContext , 
		ID3D11Buffer * constBufView , 
		ID3D11Buffer * constBufProj ) {}

	virtual void AddResources () {}

	virtual void SetWorldMatrix ( ConstBufMatrix1 worldMatrix );

	virtual void SetViewMatrix ( XMMATRIX viewMatrix );

	virtual void SetProjMatrix ( XMMATRIX projMatrix );

	virtual void Release ()
	{
		SAFE_RELEASE ( g_pVertexLayout );
		SAFE_RELEASE ( constBufWorld );

		SAFE_RELEASE ( g_pEffect );
		/*SAFE_RELEASE ( worldVariable );
		SAFE_RELEASE ( viewVariable );
		SAFE_RELEASE ( projVariable );*/
	}
	~BaseModel ();
};

