#include "Snowman.h"



Snowman::Snowman ()
{
}

Snowman::Snowman ( 
		ID3D11Device * pd3dDevice , 
		const DXGI_SURFACE_DESC * pBackBufferSurfaceDesc ,
		void * pUserContext 
)
{
	Initialise ( pd3dDevice , pBackBufferSurfaceDesc , pUserContext );
}

void Snowman::Initialise ( 
	ID3D11Device * pd3dDevice ,
	const DXGI_SURFACE_DESC * pBackBufferSurfaceDesc ,
	void * pUserContext 
)
{
	this->pd3dDevice = pd3dDevice;
	this->pBackBufferSurfaceDesc = pBackBufferSurfaceDesc;
	this->pUserContext = pUserContext;

	SetupSnowman ();
}


Snowman::~Snowman ()
{
	
}

void Snowman::SetupSnowman ()
{
	ConfigHead ();
	ConfigBody ();
	ConfigEye ();
	ConfigHand ();
	ConfigNose ();
}

void Snowman::ConfigHead ()
{
	MeshData model;
	GeometryGenerator geoGen;
	
	geoGen.CreateGeosphere ( 3 , 5 , model );

	BaseModel *Head = new BasicGeometry ( pd3dDevice , pBackBufferSurfaceDesc , pUserContext );

	Head->Initiallise ( L"basicTexEffect.fx" , model , L"desert_sky.dds" );

	XMMATRIX worldMatrix = XMMatrixTranslation ( 0 , 2 , 0 );
	Head->SetWorldMatrix ( worldMatrix );

	componentPos.push_back ( worldMatrix );

	models.push_back ( Head );
}

void Snowman::ConfigBody ()
{
	MeshData model;
	GeometryGenerator geoGen;

	geoGen.CreateGeosphere ( 5 , 5 , model );

	BaseModel *Body = new BasicGeometry ( pd3dDevice , pBackBufferSurfaceDesc , pUserContext );

	Body->Initiallise ( L"basicTexEffect.fx" , model , L"desert_sky.dds" );

	XMMATRIX worldMatrix = XMMatrixTranslation ( 0 , -4 , 0 );
	Body->SetWorldMatrix ( worldMatrix );
	componentPos.push_back ( worldMatrix );
	models.push_back ( Body );
}

void Snowman::ConfigEye ()
{
	MeshData model;
	GeometryGenerator geoGen;

	geoGen.CreateGeosphere ( 0.3 , 5 , model );

	// left eye
	BaseModel *leftEye = new BasicGeometry ( pd3dDevice , pBackBufferSurfaceDesc , pUserContext );

	leftEye->Initiallise ( L"basicTexEffect.fx" , model , nullptr );

	XMMATRIX worldMatrix = XMMatrixTranslation ( 1.2 , 2.9 , -2.3 );
	leftEye->SetWorldMatrix ( worldMatrix );
	componentPos.push_back ( worldMatrix );
	models.push_back ( leftEye );

	// right eye
	BaseModel *rightEye = new BasicGeometry ( pd3dDevice , pBackBufferSurfaceDesc , pUserContext );

	rightEye->Initiallise ( L"basicTexEffect.fx" , model , nullptr );

	worldMatrix = XMMatrixTranslation ( -1.2, 2.9 , -2.3 );
	rightEye->SetWorldMatrix ( worldMatrix );
	componentPos.push_back ( worldMatrix );
	models.push_back ( rightEye );
}

void Snowman::ConfigNose ()
{
	MeshData model;
	GeometryGenerator geoGen;

	geoGen.CreateCylinder ( 0.2 , 0.2 , 3 , 5 , 5 , model );

	// left eye
	BaseModel *nose = new BasicGeometry ( pd3dDevice , pBackBufferSurfaceDesc , pUserContext );

	nose->Initiallise ( L"basicTexEffect.fx" , model , nullptr );

	XMMATRIX worldMatrix = XMMatrixRotationX ( 70 * DEG_TO_RAD ) * XMMatrixTranslation ( 0 , 2 , -2.3 );
	nose->SetWorldMatrix ( worldMatrix );
	componentPos.push_back ( worldMatrix );
	models.push_back ( nose );
}

void Snowman::ConfigHand ()
{
	MeshData model;
	GeometryGenerator geoGen;

	geoGen.CreateCylinder ( 0.2 , 0.2 , 5 , 5 , 5 , model ); 

	// left eye
	BaseModel *leftHand = new BasicGeometry ( pd3dDevice , pBackBufferSurfaceDesc , pUserContext );

	leftHand->Initiallise ( L"basicTexEffect.fx" , model , nullptr );

	XMMATRIX worldMatrix = XMMatrixRotationZ ( 45 * DEG_TO_RAD ) * XMMatrixTranslation ( -6 , -3 , 0 );
	leftHand->SetWorldMatrix ( worldMatrix );
	componentPos.push_back ( worldMatrix );
	models.push_back ( leftHand );

	// right eye
	BaseModel *rightHand = new BasicGeometry ( pd3dDevice , pBackBufferSurfaceDesc , pUserContext );

	rightHand->Initiallise ( L"basicTexEffect.fx" , model , nullptr );

	worldMatrix = XMMatrixRotationZ ( -45 * DEG_TO_RAD ) * XMMatrixTranslation ( 6 , -3 , 0 );
	rightHand->SetWorldMatrix ( worldMatrix );
	componentPos.push_back ( worldMatrix );
	models.push_back ( rightHand );
}

void Snowman::ApplyExtraWorldMatrix ( XMMATRIX worldMatrix )
{
	
	for (auto i = 0; i < models.size (); i++)
	{
		XMMATRIX newPos = componentPos[ i ] * worldMatrix;
		models[ i ]->SetWorldMatrix( newPos );
	}
}

void Snowman::SetViewMatrix ( XMMATRIX viewMatrix )
{
	for (auto i = 0; i < models.size (); i++)
	{
		models[ i ]->SetViewMatrix ( viewMatrix );
	}
}

void Snowman::SetProjMatrix ( XMMATRIX projMatrix )
{
	for (auto i = 0; i < models.size (); i++)
	{
		models[ i ]->SetProjMatrix ( projMatrix );
	}
}

void Snowman::SetCameraPos ( XMVECTOR camPos )
{
	for (auto i = 0; i < models.size (); i++)
	{
		models[ i ]->SetCameraPos( camPos );
	}
}

void Snowman::SetLightDirection ( XMVECTOR lightDir )
{
	for (auto i = 0; i < models.size (); i++)
	{
		models[ i ]->SetLightDirection ( lightDir );
	}
}

void Snowman::RenderSnowman ( double fTime , float fElapsedTime , void * pUserContex )
{
	for (auto i = 0; i < models.size (); i++)
	{
		models[ i ]->RenderScene ( fTime , fElapsedTime , pUserContext );
	}
}

void Snowman::Release ()
{
	for (auto i = 0; i < models.size (); i++)
	{
		models[ i ]->Release ();
	}

	// empty vector
	for (vector<BaseModel *>::iterator it = models.begin (); it != models.end (); it++)
		if (NULL != *it)
		{
			delete *it;
			*it = NULL;
		}
	models.clear ();
}
