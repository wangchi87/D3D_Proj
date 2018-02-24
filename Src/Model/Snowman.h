#pragma once

#include "Model\BasicGeometry.h"


class Snowman 
{
	// device handle
	ID3D11Device*				pd3dDevice;
	const DXGI_SURFACE_DESC*	pBackBufferSurfaceDesc;
	void*						pUserContext;

	vector<BaseModel*>			models;

public:
	Snowman ();

	Snowman (
		ID3D11Device* pd3dDevice ,
		const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc ,
		void* pUserContext
		);

	void Initialise( 
		ID3D11Device* pd3dDevice ,
		const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc ,
		void* pUserContext
		);

	~Snowman ();

	void SetupSnowman ();

	void ConfigHead ();
	void ConfigBody ();
	void ConfigEye ();
	void ConfigNose ();
	void ConfigHand ();

	void SetWorldMatrix ( XMMATRIX worldMatrix ) ;

	void SetViewMatrix ( XMMATRIX viewMatrix ) ;

	void SetProjMatrix ( XMMATRIX projMatrix ) ;

	void RenderSnowman (
		double fTime ,
		float fElapsedTime ,
		void* pUserContex
	) ;

	void Release ();
};

