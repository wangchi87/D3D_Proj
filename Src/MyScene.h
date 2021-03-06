#pragma once

#include "ProjectHeader.h"

#include "Model\BasicGeometry.h"
#include "Model\Snowman.h"

class MyScene
{
	ID3D11Device*				pd3dDevice;
	const DXGI_SURFACE_DESC*	pBackBufferSurfaceDesc;
	ID3D11DeviceContext*		pd3dImmediateContext;
	void*						pUserContext;

	DWORD						dwShaderFlags;
	
	// variable for first person camera
	FirstPersonCamera			camera;
	bool						firstMouseEntry;
	UINT						lastMousePosX;
	UINT						lastMousePosY;

	// geometry models
	vector<BaseModel*>			models;
	Snowman						snowman;
	Snowman						snowmanOnBox;

	// time elapsed between two frames
	float						deltaTime;
	// total running time
	float						totalTime;

	// variables for camera roaming on box
	bool						isCameraOnBoard;
	XMVECTOR					boxPos;
	XMVECTOR					cameraPos;

	// model material roughness( 0-1,the larger, the rougher)
	float						materialRoughness;

public:
	MyScene ();
	~MyScene ();

	void InitScene ( ID3D11Device* pd3dDevice ,
		const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc ,
		void* pUserContext );

	void AddModel ();

	void RenderScene ( double fTime , float fElapsedTime , void* pUserContext );

	// change roughness
	void IncreaseMaterialRoughness ();

	void DecreaseMaterialRoughness ();

	// change camera pos
	void InitCamera ();

	void MouseLeave ();

	void UpdateCameraPos ( char c );

	void UpdateMousePos ( int xPos , int yPos );

	void CameraTryOnBoard ();

private:
	void UpdateWorldMatrix ();

	void UpdateViewProjMatrix ();

	void SetDirectionalLightDirection ();

	void SetPointLightSourcePos ();
};

