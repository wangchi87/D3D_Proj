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

	
	ID3DX11EffectTechnique*					g_pTechnique;
	ID3DX11EffectShaderResourceVariable*	g_ptxDiffuseVariable;

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

	void InitCubeData ();

	HRESULT InitVertexShader ( );
	HRESULT InitVertexLayout ();
	HRESULT InitPixelShader ( );
	
	HRESULT InitVertexData ();
	HRESULT InitIndexBuffer ();

	HRESULT InitConstBufferWorld ();

	HRESULT InitTexture ();

	HRESULT InitEffects ();

	void UpdateWorldMatrix ();


	void RenderScene ( 
		double fTime , 
		float fElapsedTime , 
		void* pUserContex
	) override;

	void AddResources () override
	{
		//InitCubeData ();
		InitEffects ();
		//InitVertexShader ();
		InitVertexLayout ();
		//InitPixelShader ();
		InitVertexData ();
		InitIndexBuffer ();
		//InitConstBufferWorld ();
		InitTexture ();

		
	}

	void Release () override;

	~Cube ();
};

inline Vertex * GetCube ()
{
	
	Vertex vct [24] =
	{
		{ XMFLOAT3 ( -1.0f, 1.0f, -1.0f ), XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),  XMFLOAT3 ( 0.0f, 1.0f, 1.0f ), XMFLOAT2 ( 1.0f, 0.0f ) },
	{ XMFLOAT3 ( 1.0f, 1.0f, -1.0f ),XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),  XMFLOAT3 ( 0.0f, 1.0f, 1.0f ), XMFLOAT2 ( 0.0f, 0.0f ) },
	{ XMFLOAT3 ( 1.0f, 1.0f, 1.0f ),XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),  XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),XMFLOAT2 ( 0.0f, 1.0f ) },
	{ XMFLOAT3 ( -1.0f, 1.0f, 1.0f ),XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),  XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),XMFLOAT2 ( 1.0f, 1.0f ) },
	
	{ XMFLOAT3 ( -1.0f, -1.0f, -1.0f ),XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),  XMFLOAT3 ( 0.0f, 1.0f, 1.0f ), XMFLOAT2 ( 0.0f, 0.0f ) },
	{ XMFLOAT3 ( 1.0f, -1.0f, -1.0f ),XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),  XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),XMFLOAT2 ( 1.0f, 0.0f ) },
	{ XMFLOAT3 ( 1.0f, -1.0f, 1.0f ),XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),  XMFLOAT3 ( 0.0f, 1.0f, 1.0f ), XMFLOAT2 ( 1.0f, 1.0f ) },
	{ XMFLOAT3 ( -1.0f, -1.0f, 1.0f ),XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),  XMFLOAT3 ( 0.0f, 1.0f, 1.0f ), XMFLOAT2 ( 0.0f, 1.0f ) },
	
	{ XMFLOAT3 ( -1.0f, -1.0f, 1.0f ),XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),  XMFLOAT3 ( 0.0f, 1.0f, 1.0f ), XMFLOAT2 ( 0.0f, 1.0f ) },
	{ XMFLOAT3 ( -1.0f, -1.0f, -1.0f ), XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),  XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),XMFLOAT2 ( 1.0f, 1.0f ) },
	{ XMFLOAT3 ( -1.0f, 1.0f, -1.0f ),XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),  XMFLOAT3 ( 0.0f, 1.0f, 1.0f ), XMFLOAT2 ( 1.0f, 0.0f ) },
	{ XMFLOAT3 ( -1.0f, 1.0f, 1.0f ), XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),  XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),XMFLOAT2 ( 0.0f, 0.0f ) },
	
	{ XMFLOAT3 ( 1.0f, -1.0f, 1.0f ),XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),  XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),XMFLOAT2 ( 1.0f, 1.0f ) },
	{ XMFLOAT3 ( 1.0f, -1.0f, -1.0f ),XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),  XMFLOAT3 ( 0.0f, 1.0f, 1.0f ), XMFLOAT2 ( 0.0f, 1.0f ) },
	{ XMFLOAT3 ( 1.0f, 1.0f, -1.0f ),XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),  XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),XMFLOAT2 ( 0.0f, 0.0f ) },
	{ XMFLOAT3 ( 1.0f, 1.0f, 1.0f ), XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),  XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),XMFLOAT2 ( 1.0f, 0.0f ) },
	
	{ XMFLOAT3 ( -1.0f, -1.0f, -1.0f ),XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),  XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),XMFLOAT2 ( 0.0f, 1.0f ) },
	{ XMFLOAT3 ( 1.0f, -1.0f, -1.0f ), XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),  XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),XMFLOAT2 ( 1.0f, 1.0f ) },
	{ XMFLOAT3 ( 1.0f, 1.0f, -1.0f ), XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),  XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),XMFLOAT2 ( 1.0f, 0.0f ) },
	{ XMFLOAT3 ( -1.0f, 1.0f, -1.0f ),XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),  XMFLOAT3 ( 0.0f, 1.0f, 1.0f ), XMFLOAT2 ( 0.0f, 0.0f ) },
	
	{ XMFLOAT3 ( -1.0f, -1.0f, 1.0f ), XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),  XMFLOAT3 ( 0.0f, 1.0f, 1.0f ), XMFLOAT2 ( 1.0f, 1.0f ) },
	{ XMFLOAT3 ( 1.0f, -1.0f, 1.0f ), XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),  XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),XMFLOAT2 ( 0.0f, 1.0f ) },
	{ XMFLOAT3 ( 1.0f, 1.0f, 1.0f ),XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),  XMFLOAT3 ( 0.0f, 1.0f, 1.0f ), XMFLOAT2 ( 0.0f, 0.0f ) },
	{ XMFLOAT3 ( -1.0f, 1.0f, 1.0f ),XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),  XMFLOAT3 ( 0.0f, 1.0f, 1.0f ), XMFLOAT2 ( 1.0f, 0.0f ) },
	};

	return vct;
}

inline WORD * GetCubeIndices ()
{
	WORD cubeIndices [36] =
	{
		3,1,0,
		2,1,3,
	
		6,4,5,
		7,4,6,
	
		11,9,8,
		10,9,11,
	
		14,12,13,
		15,12,14,
	
		19,17,16,
		18,17,19,
	
		22,20,21,
		23,20,22
	};
	return cubeIndices;
}