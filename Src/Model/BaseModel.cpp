#include "BaseModel.h"



BaseModel::BaseModel ()
{
	
	vertexIndicesNum = 0; 
}

BaseModel::BaseModel ( ID3D11Device * d3dDevice , const DXGI_SURFACE_DESC * BackBufferSurfaceDesc , void * UserContext )
{
	InitDeviceHandle ( d3dDevice , BackBufferSurfaceDesc , UserContext );
}

void BaseModel::SetEffectFileName ( LPCWSTR fileName )
{
	effectFileName = fileName;
}

void BaseModel::InitDeviceHandle ( ID3D11Device * d3dDevice , const DXGI_SURFACE_DESC * BackBufferSurfaceDesc , void * UserContext )
{
	vertexIndicesNum = 0;
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

void BaseModel::RenderScene ( double fTime , float fElapsedTime , void * pUserContext ) 
{	
	// write vertices and indices data to GPU
	UINT stride = sizeof ( Vertex );
	UINT offset = 0;

	pd3dImmediateContext->IASetVertexBuffers ( 0 , 1 , &g_pVertexBuffer , &stride , &offset );
	pd3dImmediateContext->IASetIndexBuffer ( g_pIndexBuffer , DXGI_FORMAT_R16_UINT , 0 );
}

void BaseModel::ApplyExtraWorldMatrix ( XMMATRIX extraWorldMatrix )
{
	XMMATRIX oldMatrix = GetWorldMatrix ();
	XMMATRIX newMatrix = oldMatrix * extraWorldMatrix;
	SetWorldMatrix ( newMatrix );
}

XMMATRIX BaseModel::GetWorldMatrix ()
{
	float data[ 16 ];
	worldVariable->GetMatrix ( data );
	return XMMATRIX ( data );
}

void BaseModel::SetWorldMatrix ( XMMATRIX worldMatrix )
{
	worldVariable->SetMatrix ( ( float* ) &worldMatrix );
}

void BaseModel::SetViewMatrix ( XMMATRIX viewMatrix )
{
	viewVariable->SetMatrix ( ( float* ) &viewMatrix );
}

void BaseModel::SetProjMatrix ( XMMATRIX projMatrix )
{
	projVariable->SetMatrix ( ( float* ) &projMatrix );
}

void BaseModel::InitGeometryData ( MeshData model)
{
	vertexNum = model.Vertices.size ();
	vertexIndicesNum = model.Indices.size ();

	vertices = new Vertex[ vertexNum ];

	for (WORD i = 0; i < vertexNum; i++)
		vertices[ i ] = model.Vertices[ i ];

	indices = new WORD[ vertexIndicesNum ];

	for (WORD i = 0; i < vertexIndicesNum; i++)
		indices[ i ] = model.Indices[ i ];
}

void BaseModel::SetTextureRV ( ID3D11ShaderResourceView * txRV )
{
	g_pTextureRV = txRV;
}

void BaseModel::InitTexture ( const wchar_t* fileName )
{
	if (fileName == nullptr)
	{
		g_pTextureRV = nullptr;
		return;
	}
	HRESULT hr = S_OK;

	// Load the Texture
	hr = CreateDDSTextureFromFile ( pd3dDevice , fileName , nullptr , &g_pTextureRV );
;	if (FAILED ( hr ))
		MessageBox ( nullptr ,L"Can not load texture." , L"Error" , MB_OK );
}



HRESULT BaseModel::InitVertexData ()
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

	return S_OK;
}

HRESULT BaseModel::InitIndexBuffer ()
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

	pd3dImmediateContext->IASetIndexBuffer ( g_pIndexBuffer , DXGI_FORMAT_R16_UINT , 0 );

	pd3dImmediateContext->IASetPrimitiveTopology ( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	return S_OK;
}


BaseModel::~BaseModel ()
{
}
