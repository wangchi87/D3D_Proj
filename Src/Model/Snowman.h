#pragma once

#include "Model\BasicGeometry.h"


class Snowman 
{
	// device handle
	ID3D11Device*				pd3dDevice;
	const DXGI_SURFACE_DESC*	pBackBufferSurfaceDesc;
	void*						pUserContext;

	vector<BaseModel*>			models;
	vector<XMMATRIX>			componentPos;

public:
	Snowman ();

	Snowman (
		ID3D11Device* pd3dDevice ,
		const DXGI_SURFACE_DESC* backBufferSurfaceDesc ,
		void* userContext
		);

	void Initialise( 
		ID3D11Device* pd3dDevice ,
		const DXGI_SURFACE_DESC* backBufferSurfaceDesc ,
		void* userContext
		);

	~Snowman ();

	void SetupSnowman ();

	void ConfigHead ();
	void ConfigBody ();
	void ConfigEye ();
	void ConfigNose ();
	void ConfigHand ();

	// set world, view , projection matrix
	void ApplyExtraWorldMatrix ( XMMATRIX worldMatrix ) ;
	void SetViewMatrix ( XMMATRIX viewMatrix ) ;
	void SetProjMatrix ( XMMATRIX projMatrix ) ;

	void SetCameraPos ( XMVECTOR camPos );

	// set light source
	void SetDirectionalLightDirection ( XMVECTOR lightDir );
	void SetPointLightSourcePos ( XMVECTOR lightPos );

	void RenderSnowman (
		double fTime ,
		float fElapsedTime ,
		void* pUserContex
	) ;

	void Release ();
};

