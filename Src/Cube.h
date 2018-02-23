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

	HRESULT InitVertexShader ( );
	HRESULT InitVertexLayout ();
	HRESULT InitPixelShader ( );
	
	HRESULT InitVertexData ();
	HRESULT InitIndexBuffer ();

	HRESULT InitConstBufferWorld ();

	HRESULT InitTexture ();

	HRESULT InitEffects ();

	void UpdateWorldMatrix ();

	void SetWorldMatrix ( 
		ConstBufMatrix1 worldMatrix 
		) override;
	
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

		InitEffects ();
	}

	void Release () override;

	~Cube ();
};

