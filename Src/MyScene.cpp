#include "MyScene.h"



MyScene::MyScene ()
{
	//_CrtSetBreakAlloc ( 1127 );
	firstMouseEntry = true;
	lastFrameTime = DXUTGetTime();
}


MyScene::~MyScene ()
{
	// release object
	for (auto i = 0; i < models.size (); i++)
	{
		models[ i ]->Release();
	}

	// empty vector
	for (vector<BaseModel *>::iterator it = models.begin (); it != models.end (); it++)
		if (NULL != *it)
		{
			delete *it;
			*it = NULL;
		}
	models.clear ();

	snowman.Release ();
	snowman.~Snowman ();

	snowmanOnBox.Release ();
	snowmanOnBox.~Snowman ();

	//SAFE_RELEASE ( constBufProj );
	//SAFE_RELEASE ( constBufView );
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

}

void MyScene::UpdateWorldMatrix ()
{
	// Update our time
	static float t = 0.0f;
	static float angle = 0.0f;
	{
		static ULONGLONG timeStart = 0;
		static ULONGLONG lastTime = 0;
		ULONGLONG timeCur = GetTickCount64 ();
		if (timeStart == 0)
			timeStart = timeCur;
		t = ( timeCur - timeStart ) / 500.0f;

		float deltaTime = timeCur - lastTime;
		lastTime = timeCur;

		if (angle <= 360)
			angle += deltaTime * 0.00005;
		else
			angle -= 360;
	}

	float xPos = 15 * sin ( angle * DEG_TO_RAD );
	float yPos = 15 * cos ( angle * DEG_TO_RAD );


	// NOTE : you really can NOT add Transpose to the matrix when using Effect shader!!  ( XMMatrixRotationY ( t ) );//
	XMMATRIX worldMatrix;

	// a rotation matrix around Y axis
	worldMatrix = ( XMMatrixRotationY ( t ) ) * XMMatrixTranslation ( xPos , -6 , yPos );

	BaseModel * box = models[ 0 ];
	box->SetWorldMatrix ( worldMatrix );

	worldMatrix = ( XMMatrixRotationY ( t ) ) * XMMatrixTranslation ( xPos , 5 , yPos );
	snowmanOnBox.ApplyExtraWorldMatrix ( worldMatrix );

	// a Identity matrix
	worldMatrix = XMMatrixRotationY ( 0 );
	snowman.ApplyExtraWorldMatrix ( worldMatrix );

}

void MyScene::UpdateViewProjMatrix ()
{
	XMMATRIX viewMatrix;
	XMMATRIX projMatrix;

	viewMatrix = camera.GeViewMatrix ();
	float fAspectRatio = pBackBufferSurfaceDesc->Width / ( FLOAT ) pBackBufferSurfaceDesc->Height;
	projMatrix =  XMMatrixPerspectiveFovLH ( XM_PIDIV4 , fAspectRatio , 0.01f , 100.0f ) ;

	/*ConstBufMatrix1 viewMat;
	viewMat.Mat = camera.GetTransposedViewMatrix ();
	pd3dImmediateContext->UpdateSubresource ( constBufView , 0 , nullptr , &viewMat , 0 , 0 );
	ConstBufMatrix1 projectionMat;
	projectionMat.Mat = XMMatrixTranspose ( projMatrix );
	pd3dImmediateContext->UpdateSubresource ( constBufProj , 0 , nullptr , &projectionMat , 0 , 0 );*/

	for (auto i = 0; i < models.size (); i++)
	{
		models[ i ]->SetViewMatrix ( viewMatrix );
		models[ i ]->SetProjMatrix ( projMatrix );
	}

	snowman.SetViewMatrix ( viewMatrix );
	snowman.SetProjMatrix ( projMatrix );

	snowmanOnBox.SetViewMatrix ( viewMatrix );
	snowmanOnBox.SetProjMatrix ( projMatrix );
}

void MyScene::AddModel ()
{
	MeshData model;
	GeometryGenerator geoGen;
	geoGen.CreateBox ( 4.0f , 4.0f , 4.0f , model );

	//geoGen.CreateGrid ( 5 , 10 , 10 , 15 , model );

	BaseModel *myCube = new BasicGeometry ( pd3dDevice , pBackBufferSurfaceDesc , pUserContext );

	myCube->Initiallise ( L"cubeEffect.fx" , model , L"seafloor.dds" );
	myCube->SetWorldMatrix ( XMMatrixIdentity() );
	models.push_back ( myCube );

	geoGen.CreateGeosphere ( 3 , 5 , model );

	//BaseModel *mySphere = new BasicGeometry ( pd3dDevice , pBackBufferSurfaceDesc , pUserContext );

	//mySphere->Initiallise ( L"cubeEffect.fx" , model , L"desert_sky.dds" );
	//mySphere->SetWorldMatrix ( XMMatrixIdentity () );
	//models.push_back ( mySphere );

	snowman.Initialise( pd3dDevice , pBackBufferSurfaceDesc , pUserContext );
	snowmanOnBox.Initialise ( pd3dDevice , pBackBufferSurfaceDesc , pUserContext );
}

void MyScene::RenderScene ( double fTime , float fElapsedTime , void* pUserContext )
{
	// Clear render target and the depth stencil 
	auto pRTV = DXUTGetD3D11RenderTargetView ();
	pd3dImmediateContext->ClearRenderTargetView ( pRTV , Colors::MidnightBlue );

	auto pDSV = DXUTGetD3D11DepthStencilView ();
	pd3dImmediateContext->ClearDepthStencilView ( pDSV , D3D11_CLEAR_DEPTH , 1.0 , 0 );

	UpdateWorldMatrix ();
	// set view and projection buffer
	UpdateViewProjMatrix ();

	snowman.RenderSnowman ( fTime , fElapsedTime , pUserContext );
	snowmanOnBox.RenderSnowman ( fTime , fElapsedTime , pUserContext );

	for (auto i = 0; i < models.size (); i++)
	{
		models[ i ]->RenderScene (fTime,fElapsedTime, pUserContext);
	}
}

void MyScene::MouseLeave ()
{

	firstMouseEntry = true;
}

void MyScene::UpdateCameraPos ( char c )
{

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


