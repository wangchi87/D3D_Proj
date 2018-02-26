#include "FirstPersonCamera.h"



FirstPersonCamera::FirstPersonCamera ()
{
	upperBoarder = 350;
	bottomBoarder = 0;
	leftBoarder = -180;
	rightBoarder = 180;
	frontBoarder = 180;
	backBoarder = -180;

	Yaw = YAW;
	Pitch = PITCH;

	MovementSpeed = SPEED;
	MouseSensitivity = SENSITIVTY;
	Zoom = ZOOM;
}


FirstPersonCamera::~FirstPersonCamera ()
{
	printf ( "camera release" );
}

// Initialise with vectors

void FirstPersonCamera::InitCamera ( XMVECTOR position , XMVECTOR up )
{
	
	Position = position;
	WorldUp = up;
	Yaw = YAW;
	Pitch = PITCH;

	MovementSpeed = SPEED;
	MouseSensitivity = SENSITIVTY;
	Zoom = ZOOM;

	upperBoarder = 350;
	bottomBoarder = -10;
	leftBoarder = -180;
	rightBoarder = 180;
	frontBoarder = 180;
	backBoarder = -180;

	updateCameraVectors ();
}

// Constructor with vectors
FirstPersonCamera::FirstPersonCamera ( XMVECTOR position , XMVECTOR up , float yaw , float pitch ) : MovementSpeed ( SPEED ) , MouseSensitivity ( SENSITIVTY ) , Zoom ( ZOOM )
{
	Position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;
	updateCameraVectors ();
}

// Constructor with scalar values

FirstPersonCamera::FirstPersonCamera ( float posX , float posY , float posZ , float upX , float upY , float upZ , float yaw , float pitch ) 
	: MovementSpeed ( SPEED ) , MouseSensitivity ( SENSITIVTY ) , Zoom ( ZOOM )
{
	XMFLOAT3 pos = XMFLOAT3 ( posX , posY , posZ );
	XMFLOAT3 up = XMFLOAT3 ( upX , upY , upZ );
	
	Position = XMLoadFloat3 ( &pos );
	WorldUp = XMLoadFloat3 ( &up );

	Yaw = yaw;
	Pitch = pitch;
	updateCameraVectors ();
}

// Returns the view matrix calculated using Eular Angles and the LookAt Matrix

XMMATRIX FirstPersonCamera::GetTransposedViewMatrix ()
{
	XMVECTOR At = Position + Front;

	return XMMatrixTranspose ( XMMatrixLookAtLH ( Position ,At , Up ) );

}

XMMATRIX FirstPersonCamera::GeViewMatrix ()
{
	XMVECTOR At = Position + Front;

	return  ( XMMatrixLookAtLH ( Position , At , Up ) );
}

// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)

bool FirstPersonCamera::CheckCameraOutOfBoarder ()
{
	XMFLOAT3 cp;
	XMStoreFloat3 ( &cp , Position );

	if (cp.x < leftBoarder || cp.x > rightBoarder)
		return true;
	if (cp.z < backBoarder || cp.z > frontBoarder)
		return true;
	if (cp.y > upperBoarder || cp.y < bottomBoarder)
		return true;

	return false;
}


void FirstPersonCamera::ProcessKeyboard ( Camera_Movement direction , float deltaTime )
{
	float velocity = MovementSpeed * deltaTime;

	XMVECTOR oldCameraPos = Position;

	if (direction == FORWARD)
		Position += Front * velocity;
	if (direction == BACKWARD)
		Position -= Front * velocity;
	if (direction == LEFT)
		Position += Right * velocity;
	if (direction == RIGHT)
		Position -= Right * velocity;

	if (CheckCameraOutOfBoarder ())
		Position = oldCameraPos;
}

// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.

void FirstPersonCamera::ProcessMouseMovement ( float xoffset , float yoffset , BOOL constrainPitch )
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}

	// Update Front, Right and Up Vectors using the updated Eular angles
	updateCameraVectors ();
}

// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void FirstPersonCamera::ProcessMouseScroll ( float yoffset )
{
	if (Zoom >= 1.0f && Zoom <= 45.0f)
		Zoom -= yoffset;
	if (Zoom <= 1.0f)
		Zoom = 1.0f;
	if (Zoom >= 45.0f)
		Zoom = 45.0f;
}

// conver degree to radians
float FirstPersonCamera::ToRadians ( float angDeg )
{
	return angDeg * RAD_TO_DEG;
}

// Calculates the front vector from the Camera's (updated) Eular Angles

void FirstPersonCamera::updateCameraVectors ()
{
	// Calculate the new Front vector
	XMFLOAT3 front;
	front.x = cos ( ToRadians ( Yaw ) ) * cos ( ToRadians ( Pitch ) );
	front.y = sin ( ToRadians ( Pitch ) );
	front.z = sin ( ToRadians ( Yaw ) ) * cos ( ToRadians ( Pitch ) );

	Front = XMLoadFloat3 ( &front );
	Front = XMVector3Normalize ( Front );

	Right = XMVector3Normalize ( XMVector3Cross ( Front , WorldUp ) );
	Up = XMVector3Normalize ( XMVector3Cross ( Right , Front ) );

}
