#include "Cube.h"

void Cube::InitCubeData ()
{
	MeshData box;
	GeometryGenerator geoGen;
	geoGen.CreateBox ( 1.0f , 2.0f , 4.0f , box );

	/*geoGen.CreateGeosphere ( 3 , 5 , box );

	geoGen.CreateGrid ( 5 , 10 , 10 , 15 , box );*/

	vertexNum = box.Vertices.size ();
	vertexIndicesNum = box.Indices.size ();

	vertices = new Vertex[ vertexNum ];

	for (WORD i = 0; i < vertexNum; i++)
		vertices[ i ] = box.Vertices[ i ];

	indices = new WORD [ vertexIndicesNum ];

	for (WORD i = 0; i < vertexIndicesNum; i++)
		indices[ i ] = box.Indices[ i ];

}
HRESULT Cube:: InitVertexShader ()
{
	HRESULT hr = S_OK;
	// Compile the vertex shader
	pVSBlob = nullptr;
	hr = DXUTCompileFromFile ( L"shader.fx" , nullptr , "VS" , "vs_4_0" , dwShaderFlags , 0 , &pVSBlob );
	if (FAILED ( hr ))
	{
		MessageBox ( nullptr ,
			L"The Vertex shader FX file cannot be compiled. " , L"Error" , MB_OK );
		return hr;
	}

	// Create the vertex shader
	hr = pd3dDevice->CreateVertexShader (
		pVSBlob->GetBufferPointer () ,
		pVSBlob->GetBufferSize () ,
		nullptr ,
		&g_pVertexShader
	);
	if (FAILED ( hr ))
	{
		SAFE_RELEASE ( pVSBlob );
		MessageBox ( nullptr ,L"Failed to create Vertex Shader." , L"Error" , MB_OK );
		return hr;
	}

	return S_OK;
}

HRESULT Cube::InitVertexLayout ()
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

	// Create the input layout
	/*hr = pd3dDevice->CreateInputLayout (
		layout ,
		numElements ,
		pVSBlob->GetBufferPointer () ,
		pVSBlob->GetBufferSize () ,
		&g_pVertexLayout
	);
	SAFE_RELEASE ( pVSBlob );
	if (FAILED ( hr ))
	{
		MessageBox ( nullptr , L"Failed to create Vertex Input Layout." , L"Error" , MB_OK );
		return hr;
	}*/

	D3DX11_PASS_DESC PassDesc;
	V_RETURN ( g_pTechnique->GetPassByIndex ( 0 )->GetDesc ( &PassDesc ) );
	V_RETURN ( pd3dDevice->CreateInputLayout ( layout , numElements , PassDesc.pIAInputSignature ,
		PassDesc.IAInputSignatureSize , &g_pVertexLayout ) );


	pd3dImmediateContext->IASetInputLayout ( g_pVertexLayout );
	return S_OK;
}

HRESULT Cube::InitPixelShader ( )
{
	HRESULT hr = S_OK;

	// Compile the pixel shader
	ID3DBlob* pPSBlob = nullptr;
	hr = DXUTCompileFromFile ( L"shader.fx" , nullptr , "PS" , "ps_4_0" , dwShaderFlags , 0 , &pPSBlob );
	if (FAILED ( hr ))
	{
		MessageBox ( nullptr ,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file." , L"Error" , MB_OK );
		return hr;
	}

	// Create the pixel shader
	hr = pd3dDevice->CreatePixelShader (
		pPSBlob->GetBufferPointer () ,
		pPSBlob->GetBufferSize () ,
		nullptr ,
		&g_pPixelShader
	);
	SAFE_RELEASE ( pPSBlob );
	if (FAILED ( hr ))
	{
		MessageBox ( nullptr , L"Failed to create Pixel Shader." , L"Error" , MB_OK );
		return hr;
	}
	return S_OK;
}

HRESULT Cube::InitVertexData ()
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bd;
	ZeroMemory ( &bd , sizeof ( bd ) );
	bd.Usage = D3D11_USAGE_DEFAULT;

	bd.ByteWidth = sizeof ( Vertex ) * vertexNum;

	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory ( &InitData , sizeof ( InitData ) );
	InitData.pSysMem = vertices;
	V_RETURN ( pd3dDevice->CreateBuffer ( &bd , &InitData , &g_pVertexBuffer ) );

	/*UINT stride = sizeof ( Vertex );
	UINT offset = 0;

	pd3dImmediateContext->IASetVertexBuffers ( 0 , 1 , &g_pVertexBuffer , &stride , &offset );*/

	return S_OK;
}

HRESULT Cube::InitIndexBuffer ()
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bd;
	ZeroMemory ( &bd , sizeof ( bd ) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof ( WORD ) * vertexIndicesNum;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory ( &InitData , sizeof ( InitData ) );
	InitData.pSysMem = indices;

	hr = pd3dDevice->CreateBuffer ( &bd , &InitData , &g_pIndexBuffer );
	if (FAILED ( hr ))
		return hr;

	//pd3dImmediateContext->IASetIndexBuffer ( g_pIndexBuffer , DXGI_FORMAT_R16_UINT , 0 );

	pd3dImmediateContext->IASetPrimitiveTopology ( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	return S_OK;
}

HRESULT Cube::InitConstBufferWorld ()
{
	HRESULT hr = S_OK;

	/*D3D11_BUFFER_DESC bd;
	ZeroMemory ( &bd , sizeof ( bd ) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.ByteWidth = sizeof ( ConstBufMatrix1 );
	hr = pd3dDevice->CreateBuffer ( &bd , nullptr , &constBufWorld );
	if (FAILED ( hr ))
	{
		MessageBox ( nullptr , L"Failed to create Constant Buffer." , L"Error" , MB_OK );
		return hr;
	}*/

	return S_OK;
}

HRESULT Cube::InitTexture ()
{
	HRESULT hr = S_OK;
	
	// Load the Texture
	hr = CreateDDSTextureFromFile ( pd3dDevice , L"seafloor.dds" , nullptr , &g_pTextureRV );
	if (FAILED ( hr ))
		return hr;

	// create Sampler
	//D3D11_SAMPLER_DESC sampDesc;
	//ZeroMemory ( &sampDesc , sizeof ( sampDesc ) );
	//sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	//sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	//sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	//sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	//sampDesc.MinLOD = 0;
	//sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	//hr = pd3dDevice->CreateSamplerState ( &sampDesc , &g_pSamplerState );
	//if (FAILED ( hr ))
	//	return hr;

	return S_OK;
}

HRESULT Cube::InitEffects ()
{
	HRESULT hr;

#if D3D_COMPILER_VERSION >= 46
	// Read the D3DX effect file
	WCHAR str[ MAX_PATH ];
	V_RETURN ( DXUTFindDXSDKMediaFileCch ( str , MAX_PATH , L"basicTexEffect.fx" ) );

	V_RETURN ( D3DX11CompileEffectFromFile ( str , nullptr , D3D_COMPILE_STANDARD_FILE_INCLUDE , dwShaderFlags , 0 , pd3dDevice , &g_pEffect , nullptr ) );
#else
	ID3DBlob* pEffectBuffer = nullptr;
	V_RETURN ( DXUTCompileFromFile ( L"basicTexEffect.fx" , nullptr , "none" , "fx_5_0" , dwShaderFlags , 0 , &pEffectBuffer ) );
	hr = D3DX11CreateEffectFromMemory ( pEffectBuffer->GetBufferPointer () , pEffectBuffer->GetBufferSize () , 0 , pd3dDevice , &g_pEffect );
	SAFE_RELEASE ( pEffectBuffer );
	if (FAILED ( hr ))
		return hr;
#endif
	
	g_pTechnique = g_pEffect->GetTechniqueByName ( "Render" );

	worldVariable = g_pEffect->GetVariableByName ( "World" )->AsMatrix ();
	viewVariable = g_pEffect->GetVariableByName ( "View" )->AsMatrix ();
	projVariable = g_pEffect->GetVariableByName ( "Projection" )->AsMatrix ();

	g_ptxDiffuseVariable = g_pEffect->GetVariableByName ( "g_txDiffuse" )->AsShaderResource ();

	return S_OK;
}

void Cube::UpdateWorldMatrix ()
{
	// Update our time
	static float t = 0.0f;
	{
		static ULONGLONG timeStart = 0;
		ULONGLONG timeCur = GetTickCount64 ();
		if (timeStart == 0)
			timeStart = timeCur;
		t = ( timeCur - timeStart ) / 500.0f;
	}
	//t = sinf ( t );
	/*ConstBufMatrix1 worldMat;
	worldMat.Mat = XMMatrixTranspose ( XMMatrixTranslation ( 3 , 1 , 2 ) );
	pd3dImmediateContext->UpdateSubresource ( constBufWorld , 0 , nullptr , &worldMat , 0 , 0 );*/

	// NOTE : you really can NOT add Transpose to the matrix when using Effect shader!!
	XMMATRIX worldMatrix;
	worldMatrix =  ( XMMatrixRotationY ( t ) );
	SetWorldMatrix ( worldMatrix );
}

void Cube::RenderScene ( double fTime , float fElapsedTime , void* pUserContext )
{
	UpdateWorldMatrix ();

	g_ptxDiffuseVariable->SetResource ( g_pTextureRV );

	UINT stride = sizeof ( Vertex );
	UINT offset = 0;

	pd3dImmediateContext->IASetVertexBuffers ( 0 , 1 , &g_pVertexBuffer , &stride , &offset );
	pd3dImmediateContext->IASetIndexBuffer ( g_pIndexBuffer , DXGI_FORMAT_R16_UINT , 0 );


	D3DX11_TECHNIQUE_DESC techDesc;
	HRESULT hr;
	V ( g_pTechnique->GetDesc ( &techDesc ) );

	for (UINT i = 0; i < techDesc.Passes; i++)
	{
		g_pTechnique->GetPassByIndex ( i )->Apply ( 0 , pd3dImmediateContext );
		pd3dImmediateContext->DrawIndexed ( vertexIndicesNum , 0 , 0 );
	}
		
	// *************************************************************************************
	// old code: render with HLSL (NOT with effect)

	//pd3dImmediateContext->VSSetShader ( g_pVertexShader , nullptr , 0 );
	//pd3dImmediateContext->VSSetConstantBuffers ( 1 , 1 , &constBufWorld );
	//pd3dImmediateContext->VSSetConstantBuffers ( 2 , 1 , &constBufView );
	//pd3dImmediateContext->VSSetConstantBuffers ( 3 , 1 , &constBufProj );
	////pd3dImmediateContext->VSSetConstantBuffers ( 0 , 1 , &g_pConstantBuffer );	
	//pd3dImmediateContext->PSSetShader ( g_pPixelShader , nullptr , 0 );
	////pd3dImmediateContext->PSSetConstantBuffers ( 1 , 1 , &g_pConstantBuffer );
	//
	//// set texture
	//pd3dImmediateContext->PSSetShaderResources ( 0 , 1 , &g_pTextureRV );
	//pd3dImmediateContext->PSSetSamplers ( 0 , 1 , &g_pSamplerState );

	//pd3dImmediateContext->DrawIndexed ( vertexIndicesNum , 0 , 0 );
}

void Cube::Release ()
{
	BaseModel::Release ();
	//SAFE_RELEASE ( g_pVertexShader );
	//SAFE_RELEASE ( g_pPixelShader );

	SAFE_RELEASE ( g_pVertexBuffer );
	SAFE_RELEASE ( g_pIndexBuffer );

	SAFE_RELEASE ( g_pTextureRV );


	//SAFE_RELEASE ( g_pSamplerState );

	//#if defined(DEBUG) || defined(_DEBUG)  
	//	ID3D11Debug *d3dDebug;
	//	HRESULT hr = pd3dDevice->QueryInterface ( __uuidof( ID3D11Debug ) , reinterpret_cast<void**>( &d3dDebug ) );
	//	if (SUCCEEDED ( hr ))
	//	{
	//		hr = d3dDebug->ReportLiveDeviceObjects ( D3D11_RLDO_DETAIL );
	//	}
	//	if (d3dDebug != nullptr)            d3dDebug->Release ();
	//#endif  
	//if (pd3dDevice != nullptr)             pd3dDevice->Release ();
}

Cube::~Cube ()
{
	printf ( "cube" );
}


// ******************** define vertex data ********************

//
//Vertex vct [] =
//{
//	{ XMFLOAT3 ( -1.0f, 1.0f, -1.0f ), XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),  XMFLOAT3 ( 0.0f, 1.0f, 1.0f ), XMFLOAT2 ( 1.0f, 0.0f ) },
//{ XMFLOAT3 ( 1.0f, 1.0f, -1.0f ),XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),  XMFLOAT3 ( 0.0f, 1.0f, 1.0f ), XMFLOAT2 ( 0.0f, 0.0f ) },
//{ XMFLOAT3 ( 1.0f, 1.0f, 1.0f ),XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),  XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),XMFLOAT2 ( 0.0f, 1.0f ) },
//{ XMFLOAT3 ( -1.0f, 1.0f, 1.0f ),XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),  XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),XMFLOAT2 ( 1.0f, 1.0f ) },
//
//{ XMFLOAT3 ( -1.0f, -1.0f, -1.0f ),XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),  XMFLOAT3 ( 0.0f, 1.0f, 1.0f ), XMFLOAT2 ( 0.0f, 0.0f ) },
//{ XMFLOAT3 ( 1.0f, -1.0f, -1.0f ),XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),  XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),XMFLOAT2 ( 1.0f, 0.0f ) },
//{ XMFLOAT3 ( 1.0f, -1.0f, 1.0f ),XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),  XMFLOAT3 ( 0.0f, 1.0f, 1.0f ), XMFLOAT2 ( 1.0f, 1.0f ) },
//{ XMFLOAT3 ( -1.0f, -1.0f, 1.0f ),XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),  XMFLOAT3 ( 0.0f, 1.0f, 1.0f ), XMFLOAT2 ( 0.0f, 1.0f ) },
//
//{ XMFLOAT3 ( -1.0f, -1.0f, 1.0f ),XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),  XMFLOAT3 ( 0.0f, 1.0f, 1.0f ), XMFLOAT2 ( 0.0f, 1.0f ) },
//{ XMFLOAT3 ( -1.0f, -1.0f, -1.0f ), XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),  XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),XMFLOAT2 ( 1.0f, 1.0f ) },
//{ XMFLOAT3 ( -1.0f, 1.0f, -1.0f ),XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),  XMFLOAT3 ( 0.0f, 1.0f, 1.0f ), XMFLOAT2 ( 1.0f, 0.0f ) },
//{ XMFLOAT3 ( -1.0f, 1.0f, 1.0f ), XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),  XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),XMFLOAT2 ( 0.0f, 0.0f ) },
//
//{ XMFLOAT3 ( 1.0f, -1.0f, 1.0f ),XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),  XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),XMFLOAT2 ( 1.0f, 1.0f ) },
//{ XMFLOAT3 ( 1.0f, -1.0f, -1.0f ),XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),  XMFLOAT3 ( 0.0f, 1.0f, 1.0f ), XMFLOAT2 ( 0.0f, 1.0f ) },
//{ XMFLOAT3 ( 1.0f, 1.0f, -1.0f ),XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),  XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),XMFLOAT2 ( 0.0f, 0.0f ) },
//{ XMFLOAT3 ( 1.0f, 1.0f, 1.0f ), XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),  XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),XMFLOAT2 ( 1.0f, 0.0f ) },
//
//{ XMFLOAT3 ( -1.0f, -1.0f, -1.0f ),XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),  XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),XMFLOAT2 ( 0.0f, 1.0f ) },
//{ XMFLOAT3 ( 1.0f, -1.0f, -1.0f ), XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),  XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),XMFLOAT2 ( 1.0f, 1.0f ) },
//{ XMFLOAT3 ( 1.0f, 1.0f, -1.0f ), XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),  XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),XMFLOAT2 ( 1.0f, 0.0f ) },
//{ XMFLOAT3 ( -1.0f, 1.0f, -1.0f ),XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),  XMFLOAT3 ( 0.0f, 1.0f, 1.0f ), XMFLOAT2 ( 0.0f, 0.0f ) },
//
//{ XMFLOAT3 ( -1.0f, -1.0f, 1.0f ), XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),  XMFLOAT3 ( 0.0f, 1.0f, 1.0f ), XMFLOAT2 ( 1.0f, 1.0f ) },
//{ XMFLOAT3 ( 1.0f, -1.0f, 1.0f ), XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),  XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),XMFLOAT2 ( 0.0f, 1.0f ) },
//{ XMFLOAT3 ( 1.0f, 1.0f, 1.0f ),XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),  XMFLOAT3 ( 0.0f, 1.0f, 1.0f ), XMFLOAT2 ( 0.0f, 0.0f ) },
//{ XMFLOAT3 ( -1.0f, 1.0f, 1.0f ),XMFLOAT3 ( 0.0f, 1.0f, 1.0f ),  XMFLOAT3 ( 0.0f, 1.0f, 1.0f ), XMFLOAT2 ( 1.0f, 0.0f ) },
//};

// *********************** remember to change this ********************
//void *						vertices;
//UINT						vertexStructSize;
//UINT						vertexNum;

//vertexStructSize = sizeof ( Vertex );
//vertexNum = ARRAYSIZE ( vct );
//vertices = vct;
// *********************** remember to change this ********************

// ************************************* end of defining vertex data ***********************************


// create index buffer

//WORD cubeIndices [] =
//{
//	3,1,0,
//	2,1,3,
//
//	6,4,5,
//	7,4,6,
//
//	11,9,8,
//	10,9,11,
//
//	14,12,13,
//	15,12,14,
//
//	19,17,16,
//	18,17,19,
//
//	22,20,21,
//	23,20,22
//};

// *********** change this ************
//WORD *indices = cubeIndices;
//vertexIndicesNum = ARRAYSIZE ( cubeIndices );
// *********** change this ************