#include "BasicGeometry.h"


HRESULT BasicGeometry::InitVertexLayout ()
{
	HRESULT hr = S_OK;

	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout [] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENTU", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE ( layout );

	D3DX11_PASS_DESC PassDesc;
	V_RETURN ( g_pTechnique->GetPassByIndex ( 0 )->GetDesc ( &PassDesc ) );
	V_RETURN ( pd3dDevice->CreateInputLayout ( layout , numElements , PassDesc.pIAInputSignature ,
		PassDesc.IAInputSignatureSize , &g_pVertexLayout ) );

	pd3dImmediateContext->IASetInputLayout ( g_pVertexLayout );
	return S_OK;
}


HRESULT BasicGeometry::InitEffects ()
{
	HRESULT hr;

#if D3D_COMPILER_VERSION >= 46
	// Read the D3DX effect file
	WCHAR str[ MAX_PATH ];
	V_RETURN ( DXUTFindDXSDKMediaFileCch ( str , MAX_PATH , effectFileName ) );

	V_RETURN ( D3DX11CompileEffectFromFile ( str , nullptr , D3D_COMPILE_STANDARD_FILE_INCLUDE , dwShaderFlags , 0 , pd3dDevice , &g_pEffect , nullptr ) );
#else
	ID3DBlob* pEffectBuffer = nullptr;
	V_RETURN ( DXUTCompileFromFile ( effectFileName , nullptr , "none" , "fx_5_0" , dwShaderFlags , 0 , &pEffectBuffer ) );
	hr = D3DX11CreateEffectFromMemory ( pEffectBuffer->GetBufferPointer () , pEffectBuffer->GetBufferSize () , 0 , pd3dDevice , &g_pEffect );
	SAFE_RELEASE ( pEffectBuffer );
	if (FAILED ( hr ))
		return hr;
#endif

	g_pTechnique = g_pEffect->GetTechniqueByName ( "Render" );

	worldVariable = g_pEffect->GetVariableByName ( "World" )->AsMatrix ();
	viewVariable = g_pEffect->GetVariableByName ( "View" )->AsMatrix ();
	projVariable = g_pEffect->GetVariableByName ( "Projection" )->AsMatrix ();

	cameraPosVariable = g_pEffect->GetVariableByName ( "CameraPos" )->AsVector();
	directionalLightSouceDirectionVariable = g_pEffect->GetVariableByName ( "DirectionalLightSourceDirection" )->AsVector ();
	pointLightSourcePosVariable = g_pEffect->GetVariableByName ( "PointLightSourcePosition" )->AsVector ();

	materialRoughnessVariable = g_pEffect->GetVariableByName ( "MaterialRoughness" )->AsScalar ();
	materialRoughnessVariable->SetFloat ( 0.3 );

	g_ptxDiffuseVariable = g_pEffect->GetVariableByName ( "g_txDiffuse" )->AsShaderResource ();

	return S_OK;
}


void BasicGeometry::RenderScene ( double fTime , float fElapsedTime , void* userContext )
{
	// write vertices and indices data to GPU
	BaseModel::RenderScene ( fTime , fElapsedTime , userContext );

	if ( g_pTextureRV != nullptr )
		g_ptxDiffuseVariable->SetResource ( g_pTextureRV );

	D3DX11_TECHNIQUE_DESC techDesc;
	HRESULT hr;
	V ( g_pTechnique->GetDesc ( &techDesc ) );

	for (UINT i = 0; i < techDesc.Passes; i++)
	{
		g_pTechnique->GetPassByIndex ( i )->Apply ( 0 , pd3dImmediateContext );
		pd3dImmediateContext->DrawIndexed ( vertexIndicesNum , 0 , 0 );
	}
}

void BasicGeometry::SetCameraPos ( XMVECTOR camPos )
{
	cameraPosVariable->SetFloatVector ( ( float* ) &camPos );
}

void BasicGeometry::SetDirectionalLightDirection ( XMVECTOR lightDir )
{
	directionalLightSouceDirectionVariable->SetFloatVector ( ( float* ) &lightDir );
}

void BasicGeometry::SetPointLightSourcePos ( XMVECTOR lightPos )
{
	pointLightSourcePosVariable->SetFloatVector ( ( float* ) &lightPos );
}

void BasicGeometry::SetMaterialRoughness ( float rough )
{
	materialRoughnessVariable->SetFloat ( rough );
}

void BasicGeometry::Release ()
{
	BaseModel::Release ();
}

BasicGeometry::~BasicGeometry ()
{
}
