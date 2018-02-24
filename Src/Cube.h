#pragma once

#include "ProjectHeader.h"
#include "Model\BaseModel.h"
#include "Model\GeometryGenerator.h"

class Cube: public BaseModel
{
	GeometryGenerator::MeshData box;
	GeometryGenerator geoGen;


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
		InitCubeData ();
		InitEffects ();
		//InitVertexShader ();
		InitVertexLayout ();
		//InitPixelShader ();
		InitVertexData ();
		InitIndexBuffer ();
		InitConstBufferWorld ();
		InitTexture ();

		
	}

	void Release () override;

	~Cube ();
};

