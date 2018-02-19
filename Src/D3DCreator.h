#pragma once

#include "DXUT\DXUT.h"
#include "DXUTOpt\SDKmisc.h"

using namespace DirectX;

struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};

struct ConstBufColor
{
	XMFLOAT4 Color;
};

struct ConstantBufferProjection
{
	XMFLOAT4X4 World;
	XMFLOAT4X4 View;
	XMFLOAT4X4 Projection;
};

class D3DCreator
{
	// input parameters from DXUT OnCreation Function
	ID3D11Device* pd3dDevice;
	const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc;
	void* pUserContext;

	ID3D11DeviceContext* pd3dImmediateContext;
	DWORD dwShaderFlags;

	ID3D11VertexShader*	g_pVertexShader;
	ID3DBlob*						pVSBlob;
	ID3D11InputLayout*		g_pVertexLayout;
	ID3D11PixelShader*		g_pPixelShader;
	
	ID3D11Buffer*				g_pVertexBuffer;
	ID3D11Buffer*				g_pIndexBuffer;
	ID3D11Buffer*				g_pConstantBuffer;

	XMMATRIX                g_World;
	XMMATRIX                g_View;
	XMMATRIX                g_Projection;

public:
	D3DCreator () {	};

	D3DCreator ( ID3D11Device* pd3dDevice );
	D3DCreator( ID3D11Device* pd3dDevice , 
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
	HRESULT InitConstBuffer ();

	void RenderScene ( double fTime , float fElapsedTime , void* pUserContext );

	ID3D11DeviceContext* GetDeviceContext ()
	{
		return pd3dImmediateContext;
	}
	~D3DCreator ();
};

