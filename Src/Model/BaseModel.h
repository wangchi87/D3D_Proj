#pragma once

#include "ProjectHeader.h"


class BaseModel
{
protected:
	// device and context handle
	ID3D11Device*				pd3dDevice;
	ID3D11DeviceContext*		pd3dImmediateContext;
	const DXGI_SURFACE_DESC*	pBackBufferSurfaceDesc;

	void*						pUserContext;
	DWORD						dwShaderFlags;

	// vertices and indices
	ID3D11Buffer*				g_pVertexBuffer;
	ID3D11Buffer*				g_pIndexBuffer;

	ID3D11InputLayout*			g_pVertexLayout;

	Vertex*						vertices;
	UINT						vertexNum;
	WORD*						indices;
	UINT						vertexIndicesNum;

	// texture resource
	ID3D11ShaderResourceView*	g_pTextureRV;

	// effects configurations
	LPCWSTR 						effectFileName;
	ID3DX11Effect*					g_pEffect;
	
	ID3DX11EffectMatrixVariable*	worldVariable;
	ID3DX11EffectMatrixVariable*	viewVariable;
	ID3DX11EffectMatrixVariable*	projVariable;

public:
	BaseModel ();
	BaseModel ( 
		ID3D11Device* pd3dDevice ,
		const DXGI_SURFACE_DESC* backBufferSurfaceDesc ,
		void* userContext
		);

	void SetEffectFileName ( LPCWSTR fileName );

	// set effect file, geometry data and texture file path
	void Initiallise ( 
		LPCWSTR eFileName , 
		MeshData model , 
		const wchar_t* txFileName 
	);

	// set d3d device
	virtual void InitDeviceHandle ( 
		ID3D11Device* pd3dDevice ,
		const DXGI_SURFACE_DESC* backBufferSurfaceDesc ,
		void* userContext 
	);

	virtual void RenderScene ( 
		double fTime , 
		float fElapsedTime , 
		void* pUserContext 
	);

	virtual void AddResources () {}

	// set transformation matrix
	virtual void SetWorldMatrix ( XMMATRIX worldMatrix );
	virtual void SetViewMatrix ( XMMATRIX viewMatrix );
	virtual void SetProjMatrix ( XMMATRIX projMatrix );

	// set d3d vertex and index data
	virtual HRESULT InitVertexData ();
	virtual HRESULT InitIndexBuffer ();

	// init geometry and texture data
	virtual void InitGeometryData ( MeshData model );
	virtual void InitTexture ( const wchar_t* fileName );
	virtual void SetTextureRV ( ID3D11ShaderResourceView * txRV );
	virtual void SetMaterialRoughness ( float rough ) {};

	// set camera position
	virtual void SetCameraPos ( XMVECTOR camPos ) {};

	// set light source
	virtual void SetDirectionalLightDirection ( XMVECTOR lightDir ) {};
	virtual void SetPointLightSourcePos ( XMVECTOR lightPos ) {};

	// release objects
	virtual void Release ();

	// modify world matrix based on current ont
	void ApplyExtraWorldMatrix ( XMMATRIX extraWorldMatrix );

	XMMATRIX GetWorldMatrix ();
	
	~BaseModel ();
};

