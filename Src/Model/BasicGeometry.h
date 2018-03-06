#pragma once

#include "ProjectHeader.h"
#include "Model\BaseModel.h"

// this class is needed for anything related to the effect shader configuration
class BasicGeometry: public BaseModel
{

	ID3DX11EffectTechnique*					g_pTechnique;

	ID3DX11EffectShaderResourceVariable*	g_ptxDiffuseVariable;

	// effect file variables
	ID3DX11EffectVectorVariable*			cameraPosVariable;
	ID3DX11EffectVectorVariable*			directionalLightSouceDirectionVariable;
	ID3DX11EffectVectorVariable*			pointLightSourcePosVariable;
	ID3DX11EffectScalarVariable*			materialRoughnessVariable;

public:
	BasicGeometry () { vertexIndicesNum = 0; };

	BasicGeometry (
		ID3D11Device* pd3dDevice ,
		const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc ,
		void* pUserContext
	) :BaseModel (
		pd3dDevice ,
		pBackBufferSurfaceDesc ,
		pUserContext
	) {};

	HRESULT InitVertexLayout ();

	HRESULT InitEffects ();

	void RenderScene (
		double fTime ,
		float fElapsedTime ,
		void* userContext
	) override;

	void AddResources () override
	{
		InitEffects ();

		InitVertexLayout ();
		InitVertexData ();
		InitIndexBuffer ();
	}

	// override virtual base functions
	void SetCameraPos ( XMVECTOR camPos ) override;

	void SetDirectionalLightDirection ( XMVECTOR lightDir ) override;

	void SetPointLightSourcePos ( XMVECTOR lightPos ) override;

	void SetMaterialRoughness ( float rough ) override; 

	void Release () override;

	~BasicGeometry ();

};

