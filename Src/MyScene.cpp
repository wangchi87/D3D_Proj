#include "MyScene.h"



MyScene::MyScene ()
{
	firstMouseEntry = true;
	lastFrameTime = 0;

}


MyScene::~MyScene ()
{
	// release object
	for (auto i = 0; i < models.size (); i++)
	{
		models[ i ]->~Cube ();
	}

	// empty vector
	for (vector<Cube *>::iterator it = models.begin (); it != models.end (); it++)
		if (NULL != *it)
		{
			delete *it;
			*it = NULL;
		}
	models.clear ();

	SAFE_RELEASE ( constBufProj );
	SAFE_RELEASE ( constBufView );
}

void MyScene::InitScene ( 
		ID3D11Device * d3dDevice , 
		const DXGI_SURFACE_DESC * BackBufferSurfaceDesc , 
		void * UserContext )
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

	InitCamera ();
}

void MyScene::InitCamera ()
{
	XMVECTOR Eye = XMVectorSet ( 0.0f , 0.0f , -30.0f , 0.0f );
	XMVECTOR At = XMVectorSet ( -0.0f , -0.0f , 0.0f , 0.0f );
	XMVECTOR Up = XMVectorSet ( 0.0f , 1.0f , 0.0f , 0.0f );

	camera.InitCamera ( Eye , Up );
	
	HRESULT hr = S_OK;
	D3D11_BUFFER_DESC bd;
	ZeroMemory ( &bd , sizeof ( bd ) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof ( ConstBufMatrix1 );
	bd.CPUAccessFlags = 0;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	hr = pd3dDevice->CreateBuffer ( &bd , nullptr , &constBufProj );
	if (FAILED ( hr ))
	{
		MessageBox ( nullptr , L"Failed to create Constant Projection Buffer." , L"Error" , MB_OK );
	}

	hr = pd3dDevice->CreateBuffer ( &bd , nullptr , &constBufView );
	if (FAILED ( hr ))
	{
		MessageBox ( nullptr , L"Failed to create Constant View Buffer." , L"Error" , MB_OK );
	}

}

void MyScene::UpdateViewProjBuffer ()
{
	ConstBufMatrix1 viewMatrix;
	viewMatrix.Mat = camera.GetTransposedViewMatrix ();
	pd3dImmediateContext->UpdateSubresource ( constBufView , 0 , nullptr , &viewMatrix , 0 , 0 );

	ConstBufMatrix1 projectionMatrix;
	float fAspectRatio = pBackBufferSurfaceDesc->Width / ( FLOAT ) pBackBufferSurfaceDesc->Height;
	projectionMatrix.Mat = XMMatrixTranspose ( XMMatrixPerspectiveFovLH ( XM_PIDIV4 , fAspectRatio , 0.01f , 100.0f ) );
	pd3dImmediateContext->UpdateSubresource ( constBufProj , 0 , nullptr , &projectionMatrix , 0 , 0 );
}

void MyScene::AddModel ()
{
	Cube *myCube = new Cube( pd3dDevice , pBackBufferSurfaceDesc , pUserContext );
	myCube->AddResources ();
	models.push_back ( myCube );
}

void MyScene::RenderScene ( double fTime , float fElapsedTime , void* pUserContext )
{
	// Clear render target and the depth stencil 
	auto pRTV = DXUTGetD3D11RenderTargetView ();
	pd3dImmediateContext->ClearRenderTargetView ( pRTV , Colors::MidnightBlue );

	auto pDSV = DXUTGetD3D11DepthStencilView ();
	pd3dImmediateContext->ClearDepthStencilView ( pDSV , D3D11_CLEAR_DEPTH , 1.0 , 0 );

	// set view and projection buffer
	UpdateViewProjBuffer ();

	for (auto i = 0; i < models.size (); i++)
	{
		models[ i ]->RenderScene (fTime,fElapsedTime, pUserContext, constBufView, constBufProj );
	}
}

void MyScene::MouseLeave ()
{
	for (auto i = 0; i < models.size (); i++)
	{
		models[ i ]->MouseLeave();
	}

	firstMouseEntry = true;
}

void MyScene::UpdateCameraPos ( char c )
{

	for (auto i = 0; i < models.size (); i++)
	{
		models[ i ]->UpdateCameraPos (c);
	}

	float currentFrame = DXUTGetTime ();
	float deltaTime = currentFrame - lastFrameTime;
	lastFrameTime = currentFrame;

	if (c == 'W')
		camera.ProcessKeyboard ( FORWARD , deltaTime );
	if (c == 'S')
		camera.ProcessKeyboard ( BACKWARD , deltaTime );
	if (c == 'A')
		camera.ProcessKeyboard ( LEFT , deltaTime );
	if (c == 'D')
		camera.ProcessKeyboard ( RIGHT , deltaTime );
}

void MyScene::UpdateMousePos ( int xPos , int yPos )
{
	for (auto i = 0; i < models.size (); i++)
	{
		models[ i ]->UpdateMousePos (xPos,yPos);
	}

	if (firstMouseEntry)
	{
		lastMousePosX = xPos;
		lastMousePosY = yPos;
		firstMouseEntry = false;
	}

	int xOffSet = xPos - lastMousePosX;
	int yOffSet = -yPos + lastMousePosY;

	camera.ProcessMouseMovement ( xOffSet , yOffSet , true );

	lastMousePosX = xPos;
	lastMousePosY = yPos;
}


