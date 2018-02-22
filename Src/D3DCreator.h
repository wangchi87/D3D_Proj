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

class Cube
{
	// input parameters from DXUT OnCreation Function
	ID3D11Device*				pd3dDevice;
	const DXGI_SURFACE_DESC*	pBackBufferSurfaceDesc;
	void* pUserContext;

	ID3D11DeviceContext*		pd3dImmediateContext;
	DWORD						dwShaderFlags;

	ID3D11VertexShader*			g_pVertexShader;
	ID3DBlob*					pVSBlob;
	ID3D11InputLayout*			g_pVertexLayout;
	ID3D11PixelShader*			g_pPixelShader;
	
	ID3D11Buffer*				g_pVertexBuffer;
	ID3D11Buffer*				g_pIndexBuffer;
	ID3D11Buffer*				g_pConstantBuffer;

	UINT						indexNum;
	
	FirstPersonCamera			camera;
	XMMATRIX					g_World;
	XMMATRIX					g_View;
	XMMATRIX					g_Projection;

	ID3D11ShaderResourceView*	g_pTextureRV;
	ID3D11SamplerState*			g_pSamplerState;

	bool						firstMouseEntry;
	float						lastFrameTime;
	UINT						lastMousePosX;
	UINT						lastMousePosY;

public:
	Cube () 
	{
		firstMouseEntry = true;
		indexNum = 0;
		lastFrameTime = DXUTGetTime ();
	};

	Cube ( ID3D11Device* pd3dDevice );
	Cube( ID3D11Device* pd3dDevice , 
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

	void MouseLeave ()
	{
		firstMouseEntry = true;
	}

	void UpdateMousePos ( int xPos , int yPos )
	{
		if (firstMouseEntry)
		{
			lastMousePosX = xPos;
			lastMousePosY = yPos;
			firstMouseEntry = false;
		}
		/*else
		{
			printf ( "not first mouse" );
		}*/

		int xOffSet = xPos - lastMousePosX;
		int yOffSet = -yPos + lastMousePosY;

		camera.ProcessMouseMovement(xOffSet , yOffSet, true);

		lastMousePosX = xPos;
		lastMousePosY = yPos;
	}

	void UpdateWVPMatrix ()
	{
		ConstantBufferProjection cbp;
		cbp.World = XMMatrixTranspose ( XMMatrixRotationY ( 0 ) );

		cbp.View = camera.GetTransposedViewMatrix ();

		float fAspectRatio = pBackBufferSurfaceDesc->Width / ( FLOAT ) pBackBufferSurfaceDesc->Height;

		// Initialize the projection matrix
		cbp.Projection = XMMatrixTranspose ( XMMatrixPerspectiveFovLH ( XM_PIDIV4 , fAspectRatio , 0.01f , 100.0f ) );

		pd3dImmediateContext->UpdateSubresource ( g_pConstantBuffer , 0 , nullptr , &cbp , 0 , 0 );
	}
	void UpdateCameraPos ( char c )
	{
		float currentFrame = DXUTGetTime ();
		float deltaTime = currentFrame - lastFrameTime;
		lastFrameTime = currentFrame;

		if (c == 'W')
			camera.ProcessKeyboard ( FORWARD , deltaTime );
		if (c == 'S')
			camera.ProcessKeyboard ( BACKWARD , deltaTime );
		if (c == 'A')
			camera.ProcessKeyboard ( LEFT , deltaTime );
		if (c == 'D')
			camera.ProcessKeyboard ( RIGHT , deltaTime );
	}

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
	~Cube ();
};

