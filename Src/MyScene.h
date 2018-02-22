#pragma once

#include "ProjectHeader.h"
#include "D3DCreator.h"



class MyScene
{
	ID3D11Device*				pd3dDevice;
	const DXGI_SURFACE_DESC*	pBackBufferSurfaceDesc;
	ID3D11DeviceContext*		pd3dImmediateContext;
	void*						pUserContext;

	DWORD						dwShaderFlags;

	ID3D11Buffer*				constBufView;
	ID3D11Buffer*				constBufProj;

	XMMATRIX					g_View;
	XMMATRIX					g_Projection;
	
	FirstPersonCamera			camera;
	bool						firstMouseEntry;
	float						lastFrameTime;
	UINT						lastMousePosX;
	UINT						lastMousePosY;

	vector<Cube*>				models;

public:
	MyScene ();
	~MyScene ();

	void InitScene ( ID3D11Device* pd3dDevice ,
		const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc ,
		void* pUserContext );

	void InitCamera ();

	void UpdateViewProjBuffer ();

	void AddModel ();

	void RenderScene ( double fTime , float fElapsedTime , void* pUserContext );

	void MouseLeave ();

	void UpdateCameraPos ( char c );

	void UpdateMousePos ( int xPos , int yPos );
};

