#include "D3DCreator.h"



void D3DCreator::InitCreator ( ID3D11Device * d3dDevice , const DXGI_SURFACE_DESC * BackBufferSurfaceDesc , void * UserContext )
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

D3DCreator::D3DCreator ( ID3D11Device* pd3dDevice )
{
	pd3dImmediateContext = DXUTGetD3D11DeviceContext ();
	dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	this->pd3dDevice = pd3dDevice;

#ifdef DEBUG
	dwShaderFlags |= D3DCOMPILE_DEBUG;
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif // DEBUG

}


D3DCreator::D3DCreator ( ID3D11Device* pd3dDevice, 
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

HRESULT D3DCreator:: InitVertexShader ()
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

HRESULT D3DCreator::InitVertexLayout ()
{
	HRESULT hr = S_OK;
	

	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout [] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
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


HRESULT D3DCreator::InitPixelShader ( )
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

HRESULT D3DCreator::InitVertexData ()
{
	HRESULT hr = S_OK;

	SimpleVertex triangle [] =
	{
		{ XMFLOAT3 ( 0.0f, 0.50f, 0.50f ), XMFLOAT4 ( 1.0f, 0.0f, 0.0f, 1.0f ) },
		{ XMFLOAT3 ( 0.50f, -0.30f, 0.50f ), XMFLOAT4 ( 0.0f, 1.0f, 0.0f, 1.0f ) },
		{ XMFLOAT3 ( -0.50f, -0.30f, 0.50f ), XMFLOAT4 ( 0.0f, 0.50f, 1.0f, 1.0f ) },
		{ XMFLOAT3 ( -0.50f, 0.50f, 0.50f ), XMFLOAT4 ( 1.0f, 0.0f, 0.0f, 1.0f ) },
	};

	// define vertex data and send it to GPU

	//SimpleVertex cube [] =
	//{
	//	{ XMFLOAT3 ( -1.0f, 1.0f, -1.0f ), XMFLOAT4 ( 0.0f, 0.0f, 1.0f, 1.0f ) },
	//	{ XMFLOAT3 ( 1.0f, 1.0f, -1.0f ), XMFLOAT4 ( 0.0f, 1.0f, 0.0f, 1.0f ) },
	//	{ XMFLOAT3 ( 1.0f, 1.0f, 1.0f ), XMFLOAT4 ( 0.0f, 1.0f, 1.0f, 1.0f ) },
	//	{ XMFLOAT3 ( -1.0f, 1.0f, 1.0f ), XMFLOAT4 ( 1.0f, 0.0f, 0.0f, 1.0f ) },
	//	{ XMFLOAT3 ( -1.0f, -1.0f, -1.0f ), XMFLOAT4 ( 1.0f, 0.0f, 1.0f, 1.0f ) },
	//	{ XMFLOAT3 ( 1.0f, -1.0f, -1.0f ), XMFLOAT4 ( 1.0f, 1.0f, 0.0f, 1.0f ) },
	//	{ XMFLOAT3 ( 1.0f, -1.0f, 1.0f ), XMFLOAT4 ( 1.0f, 1.0f, 1.0f, 1.0f ) },
	//	{ XMFLOAT3 ( -1.0f, -1.0f, 1.0f ), XMFLOAT4 ( 0.0f, 0.0f, 0.0f, 1.0f ) },
	//};

	SimpleVertex *vertices = triangle;

	D3D11_BUFFER_DESC bd;
	ZeroMemory ( &bd , sizeof ( bd ) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof ( SimpleVertex ) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory ( &InitData , sizeof ( InitData ) );
	InitData.pSysMem = vertices;
	V_RETURN ( pd3dDevice->CreateBuffer ( &bd , &InitData , &g_pVertexBuffer ) );

	UINT stride = sizeof ( SimpleVertex );
	UINT offset = 0;

	pd3dImmediateContext->IASetVertexBuffers ( 0 , 1 , &g_pVertexBuffer , &stride , &offset );

	return S_OK;
}


HRESULT D3DCreator::InitIndexBuffer ()
{
	HRESULT hr = S_OK;
	// create index buffer
	WORD triIndices [] =
	{
		0,1,2,
		0,2,3
	};

	WORD cubeIndices [] =
	{
		3,1,0,
		2,1,3,

		0,5,4,
		1,5,0,

		3,4,7,
		0,4,3,

		1,6,5,
		2,6,1,

		2,7,6,
		3,7,2,

		6,4,5,
		7,4,6,
	};

	WORD *indices = triIndices;

	D3D11_BUFFER_DESC bd;
	ZeroMemory ( &bd , sizeof ( bd ) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof ( WORD ) * 6;
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

HRESULT D3DCreator::InitConstBuffer ()
{
	/*HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bd;
	ZeroMemory ( &bd , sizeof ( bd ) );
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof ( ConstantBuffer );
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hr = pd3dDevice->CreateBuffer ( &bd , nullptr , &g_pConstantBuffer );
	if (FAILED ( hr ))
	{
		return hr;
	}
*/
	return S_OK;
}

void D3DCreator::RenderScene ( double fTime , float fElapsedTime , void* pUserContext )
{
	// Clear render target and the depth stencil 
	auto pRTV = DXUTGetD3D11RenderTargetView ();
	pd3dImmediateContext->ClearRenderTargetView ( pRTV , Colors::MidnightBlue );

	auto pDSV = DXUTGetD3D11DepthStencilView ();
	pd3dImmediateContext->ClearDepthStencilView ( pDSV , D3D11_CLEAR_DEPTH , 1.0 , 0 );


	pd3dImmediateContext->VSSetShader ( g_pVertexShader , nullptr , 0 );
	//pd3dImmediateContext->VSGetConstantBuffers ( 0 , 1 , &g_pConstantBuffer );
	pd3dImmediateContext->PSSetShader ( g_pPixelShader , nullptr , 0 );
	pd3dImmediateContext->DrawIndexed ( 6 , 0 , 0 );
}

D3DCreator::~D3DCreator ()
{
	SAFE_RELEASE (g_pVertexShader);
	SAFE_RELEASE ( g_pVertexLayout );
	SAFE_RELEASE ( g_pPixelShader );

	SAFE_RELEASE ( g_pVertexBuffer );
	SAFE_RELEASE ( g_pIndexBuffer );
	SAFE_RELEASE ( g_pConstantBuffer );


	
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
