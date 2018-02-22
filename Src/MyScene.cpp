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

	for (auto i = 0; i < models.size (); i++)
	{
		models[ i ]->RenderScene (fTime,fElapsedTime, pUserContext );
	}
}

void MyScene::MouseLeave ()
{
	for (auto i = 0; i < models.size (); i++)
	{
		models[ i ]->MouseLeave();
	}

	//firstMouseEntry = true;
}

void MyScene::UpdateCameraPos ( char c )
{

	for (auto i = 0; i < models.size (); i++)
	{
		models[ i ]->UpdateCameraPos (c);
	}

	/*float currentFrame = DXUTGetTime ();
	float deltaTime = currentFrame - lastFrameTime;
	lastFrameTime = currentFrame;

	if (c == 'W')
		camera.ProcessKeyboard ( FORWARD , deltaTime );
	if (c == 'S')
		camera.ProcessKeyboard ( BACKWARD , deltaTime );
	if (c == 'A')
		camera.ProcessKeyboard ( LEFT , deltaTime );
	if (c == 'D')
		camera.ProcessKeyboard ( RIGHT , deltaTime );*/
}

void MyScene::UpdateMousePos ( int xPos , int yPos )
{
	for (auto i = 0; i < models.size (); i++)
	{
		models[ i ]->UpdateMousePos (xPos,yPos);
	}

	//if (firstMouseEntry)
	//{
	//	lastMousePosX = xPos;
	//	lastMousePosY = yPos;
	//	firstMouseEntry = false;
	//}
	///*else
	//{
	//printf ( "not first mouse" );
	//}*/

	//int xOffSet = xPos - lastMousePosX;
	//int yOffSet = -yPos + lastMousePosY;

	//camera.ProcessMouseMovement ( xOffSet , yOffSet , true );

	//lastMousePosX = xPos;
	//lastMousePosY = yPos;
}


