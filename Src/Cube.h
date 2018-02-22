#pragma once

#include "ProjectHeader.h"
#include "Model\BaseModel.h"


class Cube: public BaseModel
{
	
	ID3D11VertexShader*			g_pVertexShader;
	ID3DBlob*					pVSBlob;
	
	ID3D11PixelShader*			g_pPixelShader;
	
	ID3D11Buffer*				g_pVertexBuffer;
	ID3D11Buffer*				g_pIndexBuffer;

	ID3D11ShaderResourceView*	g_pTextureRV;
	ID3D11SamplerState*			g_pSamplerState;

public:
	Cube () {vertexIndicesNum = 0;};

	Cube ( 
		ID3D11Device* pd3dDevice ,
		const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc ,
		void* pUserContext
		) :BaseModel ( 
			pd3dDevice , 
			pBackBufferSurfaceDesc , 
			pUserContext 
			) {};

	HRESULT InitVertexShader ( );
	HRESULT InitVertexLayout ();
	HRESULT InitPixelShader ( );
	
	HRESULT InitVertexData ();
	HRESULT InitIndexBuffer ();

	HRESULT InitConstBufferWorld ();

	HRESULT InitTexture ();

	void UpdateWorldMatrix ();
	
	void RenderScene ( 
		double fTime , 
		float fElapsedTime , 
		void* pUserContext, 
		ID3D11Buffer * constBufView , 
		ID3D11Buffer * constBufProj 
	) override;

	void AddResources () override
	{
		InitVertexShader ();
		InitVertexLayout ();
		InitPixelShader ();
		InitVertexData ();
		InitIndexBuffer ();
		InitConstBufferWorld ();
		InitTexture ();
	}

	void Release () override;

	~Cube ();
};
