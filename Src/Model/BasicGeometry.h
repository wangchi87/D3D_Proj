#pragma once

#include "ProjectHeader.h"
#include "Model\BaseModel.h"

// this class is needed for anything related to the effect shader configuration
class BasicGeometry: public BaseModel
{

	ID3DX11EffectTechnique*					g_pTechnique;

	ID3DX11EffectShaderResourceVariable*	g_ptxDiffuseVariable;

	ID3DX11EffectVectorVariable*			cameraPosVariable;
	ID3DX11EffectVectorVariable*			lightDirectionVariable;


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

	void InitBasicGeometryData ();

	HRESULT InitVertexLayout ();

	HRESULT InitEffects ();

	void RenderScene (
		double fTime ,
		float fElapsedTime ,
		void* pUserContex
	) override;

	void AddResources () override
	{
		//InitBasicGeometryData ();

		InitEffects ();
		InitVertexLayout ();

		InitVertexData ();
		InitIndexBuffer ();
	}
	void SetCameraPos ( XMVECTOR camPos ) override;
	void SetLightDirection ( XMVECTOR lightDir ) override;

	void Release () override;

	~BasicGeometry ();

};

