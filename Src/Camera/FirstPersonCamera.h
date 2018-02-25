#pragma once

#ifndef FIRST_PERSON_CAMERA_H
#define FIRST_PERSON_CAMERA_H

#include "ProjectHeader.h"

#define DEG_TO_RAD 180/XM_PI
#define RAD_TO_DEG XM_PI/180

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD ,
	BACKWARD ,
	LEFT ,
	RIGHT
};

// Default camera values
const float YAW = 90.0f;
const float PITCH = 0.0f;
const float SPEED = 1000.0f;
const float SENSITIVTY = 0.1f;
const float ZOOM = 45.0f;

class FirstPersonCamera
{
public:
	// Camera Attributes
	XMVECTOR Position;
	XMVECTOR Front;
	XMVECTOR Up;
	XMVECTOR Right;
	XMVECTOR WorldUp;

	// Eular Angles
	float Yaw;
	float Pitch;

	// Camera options

	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	FirstPersonCamera ();
	~FirstPersonCamera ();

	// Constructor with vectors
	FirstPersonCamera ( XMVECTOR position , XMVECTOR up , float yaw = YAW , float pitch = PITCH );
	// Constructor with scalar values
	FirstPersonCamera ( float posX , float posY , float posZ , float upX , float upY , float upZ , float yaw , float pitch );

	void InitCamera ( XMVECTOR position , XMVECTOR up );

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	XMMATRIX GetTransposedViewMatrix ();

	// Returns the view matrix 
	XMMATRIX GeViewMatrix ();

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard ( Camera_Movement direction , float deltaTime );

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement ( float xoffset , float yoffset , BOOL constrainPitch = true );

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll ( float yoffset );

	XMVECTOR GetCameraPos () { return Position; }

private:

	float ToRadians ( float angDeg );

	// Calculates the front vector from the Camera's (updated) Eular Angles
	void updateCameraVectors ();
};

#endif