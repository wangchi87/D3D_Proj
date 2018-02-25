#include "MyScene.h"



MyScene::MyScene ()
{
	//_CrtSetBreakAlloc ( 1127 );
	firstMouseEntry = true;
	deltaTime = 0;
	totalTime = 0;
	isCameraOnBoard = false;
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
inline float PointsDistance ( XMVECTOR a , XMVECTOR b )
{
	XMVECTOR distance = a - b;

	distance = XMVector3Length ( distance );

	float data; 
	XMStoreFloat (&data, distance );

	return data;
}

void MyScene::UpdateWorldMatrix ()
{
	// Update our time
	
	static float angle = 0.0f;
	{
		if (angle <= 360)
			angle += deltaTime * 0.01;
		else
			angle -= 360;
	}

	float xPos = 30 * sin ( angle * DEG_TO_RAD );
	float yPos = 30 * cos ( angle * DEG_TO_RAD );

	cameraPos = camera.GetCameraPos ();

	// NOTE : you really can NOT add Transpose to the matrix when using Effect shader!!  ( XMMatrixRotationY ( t ) );//
	XMMATRIX worldMatrix;

	// set rorating box position (a rotation matrix around Y axis)
	worldMatrix = ( XMMatrixRotationY ( totalTime ) ) * XMMatrixTranslation ( xPos , -10 , yPos );

	BaseModel * box = models[ 0 ];
	box->SetWorldMatrix ( worldMatrix );

	boxPos = XMVectorSet ( xPos , 0.0f , yPos , 0.0f );
	
	// if F is pressed and the camera is very close to the box, then get camera on board
	if(isCameraOnBoard)
		camera.Position = boxPos;

	// set snow man position
	worldMatrix = ( XMMatrixRotationY ( totalTime ) ) * XMMatrixTranslation ( xPos , 5 , yPos );
	snowmanOnBox.ApplyExtraWorldMatrix ( worldMatrix );

	worldMatrix = XMMatrixIdentity();
	snowman.ApplyExtraWorldMatrix ( worldMatrix );

	// sky sphere translates with camera

	BaseModel* skySphere = models[ 2 ];
	skySphere->SetWorldMatrix ( XMMatrixTranslationFromVector ( cameraPos ) );
}

void MyScene::UpdateViewProjMatrix ()
{
	XMMATRIX viewMatrix;
	XMMATRIX projMatrix;

	viewMatrix = camera.GeViewMatrix ();
	float fAspectRatio = pBackBufferSurfaceDesc->Width / ( FLOAT ) pBackBufferSurfaceDesc->Height;
	projMatrix =  XMMatrixPerspectiveFovLH ( XM_PIDIV4 , fAspectRatio , 0.01f , 1000.0f ) ;

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
	geoGen.CreateBox ( 8.0f , 8.0f , 8.0f , model );

	// models[0] is the BOX
	BaseModel *myCube = new BasicGeometry ( pd3dDevice , pBackBufferSurfaceDesc , pUserContext );

	myCube->Initiallise ( L"cubeEffect.fx" , model , L"seafloor.dds" );
	myCube->SetWorldMatrix ( XMMatrixIdentity() );
	models.push_back ( myCube );

	// models[1] is the GROUND
	geoGen.CreateGrid ( 200 , 200 , 100 , 100 , model );

	BaseModel *myGrid = new BasicGeometry ( pd3dDevice , pBackBufferSurfaceDesc , pUserContext );

	myGrid->Initiallise ( L"grassEffect.fx" , model , L"ground.dds" );
	myGrid->SetWorldMatrix ( XMMatrixTranslation ( 0 , -15 , 0 ) );
	models.push_back ( myGrid );

	// models[2] is the SKY-SPHERE
	geoGen.CreateGeosphere ( 200 , 5,  model );

	BaseModel *mySky = new BasicGeometry ( pd3dDevice , pBackBufferSurfaceDesc , pUserContext );

	mySky->Initiallise ( L"skyEffect.fx" , model , L"seafloor.dds" );
	mySky->SetWorldMatrix ( XMMatrixTranslation ( 0 , 0 , 0 ) );
	models.push_back ( mySky );

	snowman.Initialise( pd3dDevice , pBackBufferSurfaceDesc , pUserContext );
	snowmanOnBox.Initialise ( pd3dDevice , pBackBufferSurfaceDesc , pUserContext );
}

void MyScene::RenderScene ( double fTime , float fElapsedTime , void* pUserContext )
{
	totalTime = fTime;
	deltaTime = fElapsedTime;
	//printf ( "fTime %f  fElapsedTime %f \n " , fTime , fElapsedTime );

	// Clear render target and the depth stencil 
	auto pRTV = DXUTGetD3D11RenderTargetView ();
	pd3dImmediateContext->ClearRenderTargetView ( pRTV , Colors::MidnightBlue );

	auto pDSV = DXUTGetD3D11DepthStencilView ();
	pd3dImmediateContext->ClearDepthStencilView ( pDSV , D3D11_CLEAR_DEPTH , 1.0 , 0 );

	UpdateWorldMatrix ();
	// set view and projection buffer
	UpdateViewProjMatrix ();

	//snowman.RenderSnowman ( fTime , fElapsedTime , pUserContext );
	//snowmanOnBox.RenderSnowman ( fTime , fElapsedTime , pUserContext );

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

	int xOffSet = -xPos + lastMousePosX;
	int yOffSet = -yPos + lastMousePosY;

	camera.ProcessMouseMovement ( xOffSet , yOffSet , true );

	lastMousePosX = xPos;
	lastMousePosY = yPos;
}

void MyScene::CameraTryOnBoard ()
{
	if (isCameraOnBoard)
	{
		// deattach the camera from box
		isCameraOnBoard = false;
	}
	else
	{
		cameraPos = camera.GetCameraPos ();
		float dis = PointsDistance ( cameraPos , boxPos );
		//printf ( "dis: %f \n" , dis );
		if (dis < 15)
		{
			isCameraOnBoard = true;
		}
		else
		{
			printf ( "camera is too far from the box, distance is : %f \n" , dis );
		}
	}
}


