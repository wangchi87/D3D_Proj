#include "BaseModel.h"



BaseModel::BaseModel ()
{
	vertexIndicesNum = 0; 
}

BaseModel::BaseModel ( ID3D11Device * d3dDevice , const DXGI_SURFACE_DESC * BackBufferSurfaceDesc , void * UserContext )
{
	InitDeviceHandle ( d3dDevice , BackBufferSurfaceDesc , UserContext );
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


BaseModel::~BaseModel ()
{
}
