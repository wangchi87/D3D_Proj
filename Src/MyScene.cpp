#include "MyScene.h"



MyScene::MyScene ()
{
	//_CrtSetBreakAlloc ( 1127 );
	firstMouseEntry = true;
	deltaTime = 0;
	totalTime = 0;
	isCameraOnBoard = false;
	materialRoughness = 0.3;

}


MyScene::~MyScene ()
{
	// release object
	for (size_t i = 0; i < models.size (); i++)
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
	XMVECTOR Eye = XMVectorSet ( -30.0f , 20.0f , -120.0f , 0.0f );
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

	float xPos = 30 * sin ( angle * RAD_TO_DEG );
	float zPos = 30 * cos ( angle * RAD_TO_DEG );

	cameraPos = camera.GetCameraPos ();

	XMMATRIX worldMatrix;

	// set rorating box position (a rotation matrix around Y axis)
	// NOTE : you really can NOT add Transpose operation to the matrix when using Effect shader!!
	worldMatrix = ( XMMatrixRotationY ( totalTime ) ) * XMMatrixTranslation ( xPos , -10 , zPos );

	BaseModel * box = models[ 0 ];
	box->SetWorldMatrix ( worldMatrix );

	boxPos = XMVectorSet ( xPos , 0.0f , zPos , 0.0f );
	
	// if F is pressed and the camera is very close to the box, then get camera on board
	if(isCameraOnBoard)
		camera.Position = boxPos;

	// set snow man position
	worldMatrix = ( XMMatrixRotationY ( totalTime ) ) * XMMatrixTranslation ( xPos , 3 , zPos );
	snowmanOnBox.ApplyExtraWorldMatrix ( worldMatrix );

	worldMatrix = XMMatrixIdentity();
	snowman.ApplyExtraWorldMatrix ( worldMatrix );

	// sky sphere translates with camera
	BaseModel* skySphere = models[ 2 ];
	skySphere->SetWorldMatrix ( XMMatrixTranslationFromVector ( cameraPos ) );

	// UPDATE cameraPos for every object
	for (size_t i = 0; i < models.size (); i++)
	{
		models[ i ]->SetCameraPos ( cameraPos );
	}

	snowman.SetCameraPos ( cameraPos );
	snowmanOnBox.SetCameraPos ( cameraPos );

	SetPointLightSourcePos ();
}

void MyScene::UpdateViewProjMatrix ()
{
	XMMATRIX viewMatrix;
	XMMATRIX projMatrix;

	viewMatrix = camera.GeViewMatrix ();
	float fAspectRatio = pBackBufferSurfaceDesc->Width / ( FLOAT ) pBackBufferSurfaceDesc->Height;
	projMatrix =  XMMatrixPerspectiveFovLH ( XM_PIDIV4 , fAspectRatio , 0.01f , 1000.0f ) ;

	for (size_t i = 0; i < models.size (); i++)
	{
		models[ i ]->SetViewMatrix ( viewMatrix );
		models[ i ]->SetProjMatrix ( projMatrix );
	}

	snowman.SetViewMatrix ( viewMatrix );
	snowman.SetProjMatrix ( projMatrix );

	snowmanOnBox.SetViewMatrix ( viewMatrix );
	snowmanOnBox.SetProjMatrix ( projMatrix );
}

void MyScene::SetDirectionalLightDirection ()
{
	XMVECTOR lightDirection = XMVectorSet ( 3.0f , -4.0 , 0 , 0 );

	for (size_t i = 0; i < models.size (); i++)
	{
		models[ i ]->SetDirectionalLightDirection ( lightDirection );
	}

	snowman.SetDirectionalLightDirection ( lightDirection );
	snowmanOnBox.SetDirectionalLightDirection ( lightDirection );
}

void MyScene::SetPointLightSourcePos ()
{
	XMVECTOR pointLightPos = boxPos + XMVectorSet(20,10,0,0);

	XMMATRIX pointLightPosWorldMat = XMMatrixTranslationFromVector ( pointLightPos );

	// models[8] is the point light 
	models[ 8 ]->SetWorldMatrix ( pointLightPosWorldMat );

	for (size_t i = 0; i < models.size (); i++)
	{
		models[ i ]->SetPointLightSourcePos ( pointLightPos );
	}

	snowman.SetPointLightSourcePos ( pointLightPos );
	snowmanOnBox.SetPointLightSourcePos ( pointLightPos );
}

void MyScene::AddModel ()
{
	MeshData model;
	GeometryGenerator geoGen;
	geoGen.CreateBox ( 8.0f , 8.0f , 8.0f , model );

	// models[0] is the BOX
	BaseModel *myCube = new BasicGeometry ( pd3dDevice , pBackBufferSurfaceDesc , pUserContext );

	myCube->Initiallise ( L"Resources\\lightingTexEffect.fx" , model , L"Resources\\container.dds" );
	myCube->SetWorldMatrix ( XMMatrixIdentity() );
	models.push_back ( myCube );

	// models[1] is the GROUND
	geoGen.CreateGrid ( 400 , 400 , 50 , 50 , model );

	BaseModel *myGrid = new BasicGeometry ( pd3dDevice , pBackBufferSurfaceDesc , pUserContext );

	myGrid->Initiallise ( L"Resources\\groundEffect.fx" , model , L"Resources\\ground.dds" );
	myGrid->SetWorldMatrix ( XMMatrixTranslation ( 0 , -15 , 0 ) );
	models.push_back ( myGrid );

	// models[2] is the SKY-SPHERE
	geoGen.CreateGeosphere ( 500 , 3,  model );

	BaseModel *mySky = new BasicGeometry ( pd3dDevice , pBackBufferSurfaceDesc , pUserContext );

	mySky->Initiallise ( L"Resources\\skyEffect.fx" , model , nullptr );
	mySky->SetWorldMatrix ( XMMatrixTranslation ( 0 , 0 , 0 ) );
	models.push_back ( mySky );

	// models[3] is the SUN
	geoGen.CreateGeosphere ( 5 , 3 , model );
	BaseModel *sun = new BasicGeometry ( pd3dDevice , pBackBufferSurfaceDesc , pUserContext );

	sun->Initiallise ( L"Resources\\basicTexEffect.fx" , model , L"Resources\\sun.dds" );
	sun->SetWorldMatrix ( XMMatrixTranslation ( -90 , 120 , 0 ) );
	models.push_back ( sun );

	// models[4] [5] [6] [7] is a shinning SPHERE
	BaseModel *shinningSphere = new BasicGeometry ( pd3dDevice , pBackBufferSurfaceDesc , pUserContext );

	shinningSphere->Initiallise ( L"Resources\\lightingTexEffect.fx" , model , L"Resources\\aluminum.dds" );
	shinningSphere->SetWorldMatrix ( XMMatrixTranslation ( 30 , 30 , 0 ) );
	models.push_back ( shinningSphere );

	BaseModel *shinningSphere2 = new BasicGeometry ( pd3dDevice , pBackBufferSurfaceDesc , pUserContext );

	shinningSphere2->Initiallise ( L"Resources\\lightingTexEffect.fx" , model , L"Resources\\aluminum.dds" );
	shinningSphere2->SetWorldMatrix ( XMMatrixTranslation ( -30 , 30 , 0 ) );
	models.push_back ( shinningSphere2 );

	BaseModel *shinningSphere3 = new BasicGeometry ( pd3dDevice , pBackBufferSurfaceDesc , pUserContext );

	shinningSphere3->Initiallise ( L"Resources\\lightingTexEffect.fx" , model , L"Resources\\aluminum.dds" );
	shinningSphere3->SetWorldMatrix ( XMMatrixTranslation ( 30 , 30 , 60 ) );
	models.push_back ( shinningSphere3 );

	BaseModel *shinningSphere4 = new BasicGeometry ( pd3dDevice , pBackBufferSurfaceDesc , pUserContext );

	shinningSphere4->Initiallise ( L"Resources\\lightingTexEffect.fx" , model , L"Resources\\aluminum.dds" );
	shinningSphere4->SetWorldMatrix ( XMMatrixTranslation ( -30 , 30 , 60 ) );
	models.push_back ( shinningSphere4 );

	// models[8] is a point light souce SPHERE
	geoGen.CreateGeosphere ( 2 , 3 , model );
	BaseModel *pointLightSrc = new BasicGeometry ( pd3dDevice , pBackBufferSurfaceDesc , pUserContext );

	pointLightSrc->Initiallise ( L"Resources\\pointLightEffect.fx" , model , nullptr );
	pointLightSrc->SetWorldMatrix ( XMMatrixTranslation ( -30 , 30 , 0 ) );
	models.push_back ( pointLightSrc );

	// models[9] is external mesh - bunny
	geoGen.CreateMeshFromPLY ( L"Resources\\bun_res2_norm.ply" , model );
	ScaleMesh ( model , 100 );
	BaseModel *bunny = new BasicGeometry ( pd3dDevice , pBackBufferSurfaceDesc , pUserContext );

	bunny->Initiallise ( L"Resources\\meshEffect.fx" , model , nullptr );
	bunny->SetWorldMatrix ( XMMatrixRotationY( 80 * RAD_TO_DEG) *XMMatrixTranslation ( -35 , -17 , -40 ) );
	models.push_back ( bunny );

	// snowman 
	snowman.Initialise( pd3dDevice , pBackBufferSurfaceDesc , pUserContext );
	snowmanOnBox.Initialise ( pd3dDevice , pBackBufferSurfaceDesc , pUserContext );

	SetDirectionalLightDirection ();
}

void MyScene::RenderScene ( double fTime , float fElapsedTime , void* pUserContext )
{
	totalTime = fTime;
	deltaTime = fElapsedTime;

	// clear render target and the depth stencil 
	auto pRTV = DXUTGetD3D11RenderTargetView ();
	pd3dImmediateContext->ClearRenderTargetView ( pRTV , Colors::MidnightBlue );

	auto pDSV = DXUTGetD3D11DepthStencilView ();
	pd3dImmediateContext->ClearDepthStencilView ( pDSV , D3D11_CLEAR_DEPTH , 1.0 , 0 );

	// update matrix of all models
	UpdateWorldMatrix ();
	UpdateViewProjMatrix ();

	// do the rendering
	snowman.RenderSnowman ( fTime , fElapsedTime , pUserContext );

	if (!isCameraOnBoard)
		snowmanOnBox.RenderSnowman ( fTime , fElapsedTime , pUserContext );

	for (size_t i = 0; i < models.size (); i++)
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
		if (dis < 15)
		{
			isCameraOnBoard = true;
		}
		else
		{
			printf ( "\nToo far from the box, please move closer, distance: %f, \n" , dis );
		}
	}
}

void MyScene::IncreaseMaterialRoughness ()
{
	materialRoughness += 0.03;
	if (materialRoughness > 1)
		materialRoughness = 1;

	printf ( "\nRoughness of shinning spheres: %f \n" , materialRoughness );

	for (size_t i = 0; i < models.size (); i++)
	{
		models[ i ]->SetMaterialRoughness( materialRoughness );
	}
}

void MyScene::DecreaseMaterialRoughness ()
{
	materialRoughness -= 0.03;
	if (materialRoughness <= 0.05)
		materialRoughness = 0.05;

	printf ( "\nRoughness of shinning spheres: %f \n" , materialRoughness );

	for (size_t i = 0; i < models.size (); i++)
	{
		models[ i ]->SetMaterialRoughness ( materialRoughness );
	}
}


