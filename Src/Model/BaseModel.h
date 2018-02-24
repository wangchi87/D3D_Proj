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

	// texture
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
		const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc ,
		void* pUserContext
		);

	void SetEffectFileName ( LPCWSTR fileName );

	void Initiallise ( LPCWSTR eFileName , MeshData model , const wchar_t* txFileName )
	{
		SetEffectFileName ( eFileName );
		InitGeometryData ( model );
		InitTexture ( txFileName );
		AddResources ();
	}

	virtual void InitDeviceHandle ( 
		ID3D11Device* pd3dDevice ,
		const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc ,
		void* pUserContext );

	virtual void RenderScene ( 
		double fTime , 
		float fElapsedTime , 
		void* pUserContext );

	virtual void AddResources () {}

	virtual void SetWorldMatrix ( XMMATRIX worldMatrix );

	virtual void SetViewMatrix ( XMMATRIX viewMatrix );

	virtual void SetProjMatrix ( XMMATRIX projMatrix );

	virtual HRESULT InitVertexData ();

	virtual HRESULT InitIndexBuffer ();

	virtual void InitGeometryData ( MeshData model );

	virtual void InitTexture ( const wchar_t* fileName );

	virtual void SetTextureRV ( ID3D11ShaderResourceView * txRV );

	virtual void Release ()
	{

		SAFE_RELEASE ( g_pVertexBuffer );
		SAFE_RELEASE ( g_pIndexBuffer );

		SAFE_RELEASE ( g_pVertexLayout );
		SAFE_RELEASE ( g_pEffect );
		SAFE_RELEASE ( g_pTextureRV );

		delete [] vertices;
		delete [] indices;

		/*
		SAFE_RELEASE ( constBufWorld );
		SAFE_RELEASE ( worldVariable );
		SAFE_RELEASE ( viewVariable );
		SAFE_RELEASE ( projVariable );*/
	}
	~BaseModel ();
};

