#include "D3DCreator.h"



void MyD3DCreator::InitCreator ( ID3D11Device * d3dDevice , const DXGI_SURFACE_DESC * BackBufferSurfaceDesc , void * UserContext )
{
	pd3dImmediateContext = DXUTGetD3D11DeviceContext ();
	dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	this->pd3dDevice = d3dDevice;
	this->pBackBufferSurfaceDesc = BackBufferSurfaceDesc;
	this->pUserContext = UserContext;

#ifdef DEBUG
	dwShaderFlags |= D3DCOMPILE_DEBUG;
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif // DEBUG
}

MyD3DCreator::MyD3DCreator ( ID3D11Device* pd3dDevice )
{
	pd3dImmediateContext = DXUTGetD3D11DeviceContext ();
	dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	this->pd3dDevice = pd3dDevice;

#ifdef DEBUG
	dwShaderFlags |= D3DCOMPILE_DEBUG;
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif // DEBUG

}


MyD3DCreator::MyD3DCreator ( ID3D11Device* pd3dDevice, 
						const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc,
						void* pUserContext )
{
	pd3dImmediateContext = DXUTGetD3D11DeviceContext ();
	dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	this->pd3dDevice = pd3dDevice;
	this->pBackBufferSurfaceDesc = pBackBufferSurfaceDesc;
	this->pUserContext = pUserContext;

#ifdef DEBUG
	dwShaderFlags |= D3DCOMPILE_DEBUG;
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif // DEBUG

}

HRESULT MyD3DCreator:: InitVertexShader ()
{
	HRESULT hr = S_OK;
	// Compile the vertex shader
	pVSBlob = nullptr;
	hr = DXUTCompileFromFile ( L"shader.fx" , nullptr , "VS" , "vs_4_0" , dwShaderFlags , 0 , &pVSBlob );
	if (FAILED ( hr ))
	{
		MessageBox ( nullptr ,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file." , L"Error" , MB_OK );
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
		return hr;
	}

	return S_OK;
}

HRESULT MyD3DCreator::InitVertexLayout ()
{
	HRESULT hr = S_OK;
	

	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout [] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE ( layout );

	// Create the input layout
	hr = pd3dDevice->CreateInputLayout (
		layout ,
		numElements ,
		pVSBlob->GetBufferPointer () ,
		pVSBlob->GetBufferSize () ,
		&g_pVertexLayout
	);
	SAFE_RELEASE ( pVSBlob );
	if (FAILED ( hr ))
		return hr;

	pd3dImmediateContext->IASetInputLayout ( g_pVertexLayout );
	return S_OK;
}


HRESULT MyD3DCreator::InitPixelShader ( )
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
		return hr;

	return S_OK;
}



HRESULT MyD3DCreator::InitVertexData ()
{
	HRESULT hr = S_OK;

	// ******************** define vertex data ********************
	VertexColorTex triangle [] =
	{
		{ XMFLOAT3 ( 0.0f, 0.50f, 0.20f ), XMFLOAT4 ( 1.0f, 0.0f, 0.0f, 1.0f ),XMFLOAT2 ( 1.0f, 0.0f ) },
	{ XMFLOAT3 ( 0.50f, -0.30f, 0.20f ), XMFLOAT4 ( 0.0f, 1.0f, 0.0f, 1.0f ),XMFLOAT2 ( 0.0f, 0.0f ) },
	{ XMFLOAT3 ( -0.50f, -0.30f, 0.20f ), XMFLOAT4 ( 0.0f, 0.50f, 1.0f, 1.0f ),XMFLOAT2 ( 0.0f, 1.0f ) },
	{ XMFLOAT3 ( -0.50f, 0.50f, 0.20f ), XMFLOAT4 ( 1.0f, 0.0f, 0.0f, 1.0f ),XMFLOAT2 ( 1.0f, 1.0f ) },
	};

	SimpleVertex cube [] =
	{
		{ XMFLOAT3 ( -1.0f, 1.0f, -1.0f ), XMFLOAT4 ( 0.0f, 0.0f, 1.0f, 1.0f ) },
	{ XMFLOAT3 ( 1.0f, 1.0f, -1.0f ), XMFLOAT4 ( 0.0f, 1.0f, 0.0f, 1.0f ) },
	{ XMFLOAT3 ( 1.0f, 1.0f, 1.0f ), XMFLOAT4 ( 0.0f, 1.0f, 1.0f, 1.0f ) },
	{ XMFLOAT3 ( -1.0f, 1.0f, 1.0f ), XMFLOAT4 ( 1.0f, 0.0f, 0.0f, 1.0f ) },
	{ XMFLOAT3 ( -1.0f, -1.0f, -1.0f ), XMFLOAT4 ( 1.0f, 0.0f, 1.0f, 1.0f ) },
	{ XMFLOAT3 ( 1.0f, -1.0f, -1.0f ), XMFLOAT4 ( 1.0f, 1.0f, 0.0f, 1.0f ) },
	{ XMFLOAT3 ( 1.0f, -1.0f, 1.0f ), XMFLOAT4 ( 1.0f, 1.0f, 1.0f, 1.0f ) },
	{ XMFLOAT3 ( -1.0f, -1.0f, 1.0f ), XMFLOAT4 ( 0.0f, 0.0f, 0.0f, 1.0f ) },
	};

	VertexTex vt [] =
	{
		{ XMFLOAT3 ( -1.0f, 1.0f, -1.0f ), XMFLOAT2 ( 1.0f, 0.0f ) },
		{ XMFLOAT3 ( 1.0f, 1.0f, -1.0f ), XMFLOAT2 ( 0.0f, 0.0f ) },
		{ XMFLOAT3 ( 1.0f, 1.0f, 1.0f ), XMFLOAT2 ( 0.0f, 1.0f ) },
	};

	VertexColorTex vct [] =
	{
		{ XMFLOAT3 ( -1.0f, 1.0f, -1.0f ), XMFLOAT4 ( 0.0f, 1.0f, 1.0f, 1.0f ), XMFLOAT2 ( 1.0f, 0.0f ) },
	{ XMFLOAT3 ( 1.0f, 1.0f, -1.0f ),XMFLOAT4 ( 0.0f, 0.0f, 1.0f, 1.0f ), XMFLOAT2 ( 0.0f, 0.0f ) },
	{ XMFLOAT3 ( 1.0f, 1.0f, 1.0f ),XMFLOAT4 ( 0.0f, 0.0f, 1.0f, 1.0f ), XMFLOAT2 ( 0.0f, 1.0f ) },
	{ XMFLOAT3 ( -1.0f, 1.0f, 1.0f ), XMFLOAT4 ( 1.0f, 0.0f, 1.0f, 1.0f ),XMFLOAT2 ( 1.0f, 1.0f ) },

	{ XMFLOAT3 ( -1.0f, -1.0f, -1.0f ),XMFLOAT4 ( 1.0f, 0.0f, 1.0f, 1.0f ), XMFLOAT2 ( 0.0f, 0.0f ) },
	{ XMFLOAT3 ( 1.0f, -1.0f, -1.0f ), XMFLOAT4 ( 0.0f, 0.0f, 1.0f, 1.0f ),XMFLOAT2 ( 1.0f, 0.0f ) },
	{ XMFLOAT3 ( 1.0f, -1.0f, 1.0f ),XMFLOAT4 ( 0.0f, 0.0f, 1.0f, 1.0f ), XMFLOAT2 ( 1.0f, 1.0f ) },
	{ XMFLOAT3 ( -1.0f, -1.0f, 1.0f ),XMFLOAT4 ( 0.0f, 0.0f, 1.0f, 1.0f ), XMFLOAT2 ( 0.0f, 1.0f ) },

	{ XMFLOAT3 ( -1.0f, -1.0f, 1.0f ),XMFLOAT4 ( 0.0f, 0.0f, 1.0f, 1.0f ), XMFLOAT2 ( 0.0f, 1.0f ) },
	{ XMFLOAT3 ( -1.0f, -1.0f, -1.0f ), XMFLOAT4 ( 0.0f, 0.0f, 1.0f, 1.0f ),XMFLOAT2 ( 1.0f, 1.0f ) },
	{ XMFLOAT3 ( -1.0f, 1.0f, -1.0f ),XMFLOAT4 ( 0.0f, 1.0f, 1.0f, 1.0f ), XMFLOAT2 ( 1.0f, 0.0f ) },
	{ XMFLOAT3 ( -1.0f, 1.0f, 1.0f ), XMFLOAT4 ( 0.0f, 0.0f, 1.0f, 1.0f ),XMFLOAT2 ( 0.0f, 0.0f ) },

	{ XMFLOAT3 ( 1.0f, -1.0f, 1.0f ), XMFLOAT4 ( 0.0f, 0.0f, 1.0f, 1.0f ),XMFLOAT2 ( 1.0f, 1.0f ) },
	{ XMFLOAT3 ( 1.0f, -1.0f, -1.0f ),XMFLOAT4 ( 0.0f, 1.0f, 1.0f, 1.0f ), XMFLOAT2 ( 0.0f, 1.0f ) },
	{ XMFLOAT3 ( 1.0f, 1.0f, -1.0f ),XMFLOAT4 ( 0.0f, 0.0f, 1.0f, 1.0f ), XMFLOAT2 ( 0.0f, 0.0f ) },
	{ XMFLOAT3 ( 1.0f, 1.0f, 1.0f ), XMFLOAT4 ( 0.0f, 0.0f, 1.0f, 1.0f ),XMFLOAT2 ( 1.0f, 0.0f ) },

	{ XMFLOAT3 ( -1.0f, -1.0f, -1.0f ), XMFLOAT4 ( 1.0f, 0.0f, 1.0f, 1.0f ),XMFLOAT2 ( 0.0f, 1.0f ) },
	{ XMFLOAT3 ( 1.0f, -1.0f, -1.0f ), XMFLOAT4 ( 0.0f, 0.0f, 1.0f, 1.0f ),XMFLOAT2 ( 1.0f, 1.0f ) },
	{ XMFLOAT3 ( 1.0f, 1.0f, -1.0f ), XMFLOAT4 ( 0.0f, 1.0f, 1.0f, 1.0f ),XMFLOAT2 ( 1.0f, 0.0f ) },
	{ XMFLOAT3 ( -1.0f, 1.0f, -1.0f ),XMFLOAT4 ( 0.0f, 0.0f, 1.0f, 1.0f ), XMFLOAT2 ( 0.0f, 0.0f ) },

	{ XMFLOAT3 ( -1.0f, -1.0f, 1.0f ), XMFLOAT4 ( 1.0f, 0.0f, 1.0f, 1.0f ),XMFLOAT2 ( 1.0f, 1.0f ) },
	{ XMFLOAT3 ( 1.0f, -1.0f, 1.0f ), XMFLOAT4 ( 0.0f, 0.0f, 1.0f, 1.0f ),XMFLOAT2 ( 0.0f, 1.0f ) },
	{ XMFLOAT3 ( 1.0f, 1.0f, 1.0f ),XMFLOAT4 ( 1.0f, 0.0f, 1.0f, 1.0f ), XMFLOAT2 ( 0.0f, 0.0f ) },
	{ XMFLOAT3 ( -1.0f, 1.0f, 1.0f ),XMFLOAT4 ( 0.0f, 1.0f, 1.0f, 1.0f ), XMFLOAT2 ( 1.0f, 0.0f ) },
	};

	// *********************** remember to change this ********************
	void *						vertices;
	UINT						vertexStructSize;
	UINT						vertexNum;

	vertexStructSize = sizeof ( VertexColorTex );
	vertexNum = ARRAYSIZE ( vct );
	vertices = vct;
	// *********************** remember to change this ********************

	// ************************************* end of defining vertex data ***********************************

	D3D11_BUFFER_DESC bd;
	ZeroMemory ( &bd , sizeof ( bd ) );
	bd.Usage = D3D11_USAGE_DEFAULT;

	bd.ByteWidth = vertexStructSize * vertexNum;

	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory ( &InitData , sizeof ( InitData ) );
	InitData.pSysMem = vertices;
	V_RETURN ( pd3dDevice->CreateBuffer ( &bd , &InitData , &g_pVertexBuffer ) );

	UINT stride = vertexStructSize;
	UINT offset = 0;

	pd3dImmediateContext->IASetVertexBuffers ( 0 , 1 , &g_pVertexBuffer , &stride , &offset );

	return S_OK;
}


HRESULT MyD3DCreator::InitIndexBuffer ()
{
	HRESULT hr = S_OK;
	// create index buffer
	WORD triIndices [] =
	{
		0,2,1,
		1,2,3,
	};

	WORD cubeIndices [] =
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

	// *********** change this ************
	WORD *indices = cubeIndices;
	indexNum = ARRAYSIZE ( cubeIndices );
	// *********** change this ************

	D3D11_BUFFER_DESC bd;
	ZeroMemory ( &bd , sizeof ( bd ) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof ( WORD ) * indexNum;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory ( &InitData , sizeof ( InitData ) );
	InitData.pSysMem = indices;

	hr = pd3dDevice->CreateBuffer ( &bd , &InitData , &g_pIndexBuffer );
	if (FAILED ( hr ))
	{
		return hr;
	}

	pd3dImmediateContext->IASetIndexBuffer ( g_pIndexBuffer , DXGI_FORMAT_R16_UINT , 0 );

	pd3dImmediateContext->IASetPrimitiveTopology ( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	return S_OK;
}

HRESULT MyD3DCreator::InitConstBufferColor ()
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bd;
	ZeroMemory ( &bd , sizeof ( bd ) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof ( ConstBufColor );
	bd.CPUAccessFlags = 0;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hr = pd3dDevice->CreateBuffer ( &bd , nullptr , &g_pConstantBuffer );
	if (FAILED ( hr ))
	{
		return hr;
	}

	ConstBufColor cbc;
	cbc.Color = XMFLOAT4 ( 0.9f , 0.07f , 0.2f , 1.0f );
	pd3dImmediateContext->UpdateSubresource ( g_pConstantBuffer , 0 , nullptr , &cbc , 0 , 0 );

	return S_OK;
}

HRESULT MyD3DCreator::InitConstBufferProjection ()
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bd;
	ZeroMemory ( &bd , sizeof ( bd ) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof ( ConstantBufferProjection );
	bd.CPUAccessFlags = 0;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hr = pd3dDevice->CreateBuffer ( &bd , nullptr , &g_pConstantBuffer );
	if (FAILED ( hr ))
	{
		return hr;
	}
	/*
	ConstantBufferProjection cbp;
	cbp.World = XMMatrixTranspose ( XMMatrixIdentity () );

	// Initialize the view matrix
	XMVECTOR Eye = XMVectorSet ( 0.0f , 1.0f , -5.0f , 0.0f );
	XMVECTOR At = XMVectorSet ( 0.0f , 1.0f , 0.0f , 0.0f );
	XMVECTOR Up = XMVectorSet ( 0.0f , 1.0f , 0.0f , 0.0f );
	cbp.View = XMMatrixTranspose ( XMMatrixLookAtLH ( Eye , At , Up ) );

	// Initialize the projection matrix
	cbp.Projection = XMMatrixTranspose ( XMMatrixPerspectiveFovLH ( XM_PIDIV2 , 800 / ( float ) 600 , 0.01f , 100.0f ) );

	pd3dImmediateContext->UpdateSubresource ( g_pConstantBuffer , 0 , nullptr , &cbp , 0 , 0 );
	*/

	return S_OK;
}

HRESULT MyD3DCreator::InitTexture ()
{
	HRESULT hr = S_OK;
	
	// Load the Texture
	hr = CreateDDSTextureFromFile ( pd3dDevice , L"seafloor.dds" , nullptr , &g_pTextureRV );
	if (FAILED ( hr ))
		return hr;

	// create Sampler
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory ( &sampDesc , sizeof ( sampDesc ) );
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = pd3dDevice->CreateSamplerState ( &sampDesc , &g_pSamplerState );
	if (FAILED ( hr ))
		return hr;

	return S_OK;
}

void MyD3DCreator::RenderScene ( double fTime , float fElapsedTime , void* pUserContext )
{
	// Clear render target and the depth stencil 
	auto pRTV = DXUTGetD3D11RenderTargetView ();
	pd3dImmediateContext->ClearRenderTargetView ( pRTV , Colors::MidnightBlue );

	auto pDSV = DXUTGetD3D11DepthStencilView ();
	pd3dImmediateContext->ClearDepthStencilView ( pDSV , D3D11_CLEAR_DEPTH , 1.0 , 0 );

	// Update our time
	static float t = 0.0f;
	{
		static ULONGLONG timeStart = 0;
		ULONGLONG timeCur = GetTickCount64 ();
		if (timeStart == 0)
			timeStart = timeCur;
		t = ( timeCur - timeStart )/500.0f ;
	}

	//t = sinf ( t );

	/*ConstBufColor cbc;
	cbc.Color = XMFLOAT4 ( 0.2f, 0.5f,t, 1.0f );
	pd3dImmediateContext->UpdateSubresource ( g_pConstantBuffer , 0 , nullptr , &cbc , 0 , 0 );*/

	ConstantBufferProjection cbp;
	cbp.World = XMMatrixTranspose ( XMMatrixRotationY (t) );

	// Initialize the view matrix
	XMVECTOR Eye = XMVectorSet ( 0.0f , 1.0f , -5.0f , 0.0f );
	XMVECTOR At = XMVectorSet ( 0.0f , 1.0f , 0.0f , 0.0f );
	XMVECTOR Up = XMVectorSet ( 0.0f , 1.0f , 0.0f , 0.0f );
	cbp.View = XMMatrixTranspose ( XMMatrixLookAtLH ( Eye , At , Up ) );

	// Initialize the projection matrix
	cbp.Projection = XMMatrixTranspose ( XMMatrixPerspectiveFovLH ( XM_PIDIV2 , 800 / ( float ) 600 , 0.01f , 100.0f ) );

	pd3dImmediateContext->UpdateSubresource ( g_pConstantBuffer , 0 , nullptr , &cbp , 0 , 0 );


	pd3dImmediateContext->VSSetShader ( g_pVertexShader , nullptr , 0 );
	pd3dImmediateContext->VSSetConstantBuffers ( 0 , 1 , &g_pConstantBuffer );	
	pd3dImmediateContext->PSSetShader ( g_pPixelShader , nullptr , 0 );
	//pd3dImmediateContext->PSSetConstantBuffers ( 1 , 1 , &g_pConstantBuffer );
	
	// set texture
	pd3dImmediateContext->PSSetShaderResources ( 0 , 1 , &g_pTextureRV );
	pd3dImmediateContext->PSSetSamplers ( 0 , 1 , &g_pSamplerState );

	pd3dImmediateContext->DrawIndexed ( indexNum , 0 , 0 );
}

MyD3DCreator::~MyD3DCreator ()
{
	SAFE_RELEASE (g_pVertexShader);
	SAFE_RELEASE ( g_pVertexLayout );
	SAFE_RELEASE ( g_pPixelShader );

	SAFE_RELEASE ( g_pVertexBuffer );
	SAFE_RELEASE ( g_pIndexBuffer );
	SAFE_RELEASE ( g_pConstantBuffer );

	SAFE_RELEASE ( g_pTextureRV );
	SAFE_RELEASE ( g_pSamplerState );
	
	//SAFE_RELEASE ( pd3dDevice );
	//SAFE_RELEASE ( pd3dImmediateContext );
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
