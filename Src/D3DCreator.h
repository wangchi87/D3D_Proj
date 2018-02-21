#pragma once

#include "DXUT\DXUT.h"
#include "DXUTOpt\SDKmisc.h"

#include "DDSTextureLoader.h"

using namespace DirectX;

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

class MyD3DCreator
{
	// input parameters from DXUT OnCreation Function
	ID3D11Device* pd3dDevice;
	const DXGI_SURFACE_DESC*	pBackBufferSurfaceDesc;
	void* pUserContext;

	ID3D11DeviceContext*		pd3dImmediateContext;
	DWORD dwShaderFlags;

	ID3D11VertexShader*			g_pVertexShader;
	ID3DBlob*					pVSBlob;
	ID3D11InputLayout*			g_pVertexLayout;
	ID3D11PixelShader*			g_pPixelShader;
	
	ID3D11Buffer*				g_pVertexBuffer;
	ID3D11Buffer*				g_pIndexBuffer;
	ID3D11Buffer*				g_pConstantBuffer;


	UINT						indexNum;

	XMMATRIX					g_World;
	XMMATRIX					g_View;
	XMMATRIX					g_Projection;

	ID3D11ShaderResourceView*	g_pTextureRV;
	ID3D11SamplerState*			g_pSamplerState;

public:
	MyD3DCreator () {	};

	MyD3DCreator ( ID3D11Device* pd3dDevice );
	MyD3DCreator( ID3D11Device* pd3dDevice , 
				const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc ,
				void* pUserContext 
				);

	void InitCreator ( ID3D11Device* pd3dDevice ,
		const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc ,
		void* pUserContext );

	HRESULT InitVertexShader ( );
	HRESULT InitVertexLayout ();
	HRESULT InitPixelShader ( );
	
	HRESULT InitVertexData ();
	HRESULT InitIndexBuffer ();
	HRESULT InitConstBufferColor ();
	HRESULT InitConstBufferProjection ();

	HRESULT InitTexture ();

	void RenderScene ( double fTime , float fElapsedTime , void* pUserContext );
	void AddResources ()
	{
		InitVertexShader ();

		InitVertexLayout ();

		InitPixelShader ();

		InitVertexData ();

		InitIndexBuffer ();

		InitConstBufferProjection ();

		InitTexture ();
	}

	ID3D11DeviceContext* GetDeviceContext ()
	{
		return pd3dImmediateContext;
	}
	~MyD3DCreator ();
};

