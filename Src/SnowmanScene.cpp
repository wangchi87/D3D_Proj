//--------------------------------------------------------------------------------------
// File: SnowmanScene.cpp
//
// Starting point for new Direct3D 11 Win32 desktop applications of a snowman scene
//
// Chi Wang for NetEase -1 homework
//--------------------------------------------------------------------------------------


#include "MyScene.h"

#include <Windowsx.h>
#include <fcntl.h>
#include <io.h>

#include <iostream>
using namespace std;
#pragma warning( disable : 4100 )


// global variables
static BOOL			bTrackLeave = FALSE;
MyScene				mScene;
TRACKMOUSEEVENT		tEventTrack;


void RequestMouseLeaveMsg ()
{
	if (!bTrackLeave)
	{
		// request a mouse leave msg
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof ( tme );
		tme.hwndTrack = DXUTGetHWND ();
		tme.dwFlags = TME_LEAVE;
		_TrackMouseEvent ( &tme );
		bTrackLeave = true;
	}
}


void CallConcoleOutput ( void )
{
	AllocConsole ();
	freopen ( "CONOUT$" , "w+t" , stdout );
	// test code
	printf ( "Snow man scene\n\n" );

	printf ( "USAGE:\n\n1. use key W A S D and mouse to move around \n" );
	printf (		   "2. press key F to get on aboard on the moving box, and watch the scene in the view of moving snowman." );
	printf (				"(This is only available when you are close enough to the moving box) \n" );
	printf (		   "3. press key Z and C to increse and decrease the roughness parameters of the shinning balls.\n" );
}

//--------------------------------------------------------------------------------------
// Reject any D3D11 devices that aren't acceptable by returning false
//--------------------------------------------------------------------------------------
bool CALLBACK IsD3D11DeviceAcceptable ( const CD3D11EnumAdapterInfo *AdapterInfo , UINT Output , const CD3D11EnumDeviceInfo *DeviceInfo ,
	DXGI_FORMAT BackBufferFormat , bool bWindowed , void* pUserContext )
{
	return true;
}


//--------------------------------------------------------------------------------------
// Called right before creating a D3D device, allowing the app to modify the device settings as needed
//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings ( DXUTDeviceSettings* pDeviceSettings , void* pUserContext )
{
	return true;
}


//--------------------------------------------------------------------------------------
// Create any D3D11 resources that aren't dependant on the back buffer
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D11CreateDevice ( ID3D11Device* pd3dDevice , const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc ,
	void* pUserContext )
{
	HRESULT hr = S_OK;
	
	mScene.InitScene ( pd3dDevice , pBackBufferSurfaceDesc , pUserContext );
	mScene.AddModel ();

	return S_OK;
}


//--------------------------------------------------------------------------------------
// Create any D3D11 resources that depend on the back buffer
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D11ResizedSwapChain( ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
                                          const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
    return S_OK;
}


//--------------------------------------------------------------------------------------
// Handle updates to the scene.  This is called regardless of which D3D API is used
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
}


//--------------------------------------------------------------------------------------
// Render the scene using the D3D11 device
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D11FrameRender( ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext,
                                  double fTime, float fElapsedTime, void* pUserContext )
{
	mScene.RenderScene ( fTime , fElapsedTime , pUserContext );
}


//--------------------------------------------------------------------------------------
// Release D3D11 resources created in OnD3D11ResizedSwapChain 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D11ReleasingSwapChain( void* pUserContext )
{
}


//--------------------------------------------------------------------------------------
// Release D3D11 resources created in OnD3D11CreateDevice 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D11DestroyDevice( void* pUserContext )
{
	mScene.~MyScene ();
}


//--------------------------------------------------------------------------------------
// Handle messages to the application
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
                          bool* pbNoFurtherProcessing, void* pUserContext )
{

	if (uMsg == WM_MOUSEMOVE)
	{
		RequestMouseLeaveMsg ();

		int xPos = GET_X_LPARAM ( lParam );
		int yPos = GET_Y_LPARAM ( lParam );

		mScene.UpdateMousePos ( xPos , yPos );
	}
	if (uMsg == WM_MOUSELEAVE)
	{
		bTrackLeave = false;
		mScene.MouseLeave ();
	}

    return 0;
}



//--------------------------------------------------------------------------------------
// Handle key presses
//--------------------------------------------------------------------------------------
void CALLBACK OnKeyboard( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
	if (bKeyDown)
	{
		
		switch (nChar)
		{
		case VK_F1: // Change as needed                
			break;
		case 0x41:            //A
			mScene.UpdateCameraPos ( 'A' );
			break;
		case 0x53:            //S
			mScene.UpdateCameraPos ( 'S' );
			break;
		case 0x44:            //D
			mScene.UpdateCameraPos ( 'D' );
			break; 
		case 0x57:            //W
			mScene.UpdateCameraPos ( 'W' );
			break; 
		case 0x46:            //F
			mScene.CameraTryOnBoard ( );
			break;
		case 0x5A:			  //Z
			mScene.IncreaseMaterialRoughness ();
			break;
		case 0x43:			  //C
			mScene.DecreaseMaterialRoughness ();
			break;
		}
	}
}


//--------------------------------------------------------------------------------------
// Handle mouse button presses
//--------------------------------------------------------------------------------------
void CALLBACK OnMouse( bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown,
                       bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta,
                       int xPos, int yPos, void* pUserContext )
{
	mScene.UpdateMousePos ( xPos , yPos );
}


//--------------------------------------------------------------------------------------
// Call if device was removed.  Return true to find a new device, false to quit
//--------------------------------------------------------------------------------------
bool CALLBACK OnDeviceRemoved( void* pUserContext )
{
    return true;
}

//--------------------------------------------------------------------------------------
// Initialize everything and go into a render loop
//--------------------------------------------------------------------------------------
int WINAPI wWinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow )
{
	// create an extra cmd line window
	CallConcoleOutput ();
	
    // Enable run-time memory check for debug builds.
#ifdef _DEBUG
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    // DXUT will create and use the best device
    // that is available on the system depending on which D3D callbacks are set below

    // Set general DXUT callbacks
    DXUTSetCallbackFrameMove( OnFrameMove );
    DXUTSetCallbackKeyboard( OnKeyboard );
    DXUTSetCallbackMouse( OnMouse );
    DXUTSetCallbackMsgProc( MsgProc );
    DXUTSetCallbackDeviceChanging( ModifyDeviceSettings );
    DXUTSetCallbackDeviceRemoved( OnDeviceRemoved );

    // Set the D3D11 DXUT callbacks. Remove these sets if the app doesn't need to support D3D11
    DXUTSetCallbackD3D11DeviceAcceptable( IsD3D11DeviceAcceptable );
    DXUTSetCallbackD3D11DeviceCreated( OnD3D11CreateDevice );
    DXUTSetCallbackD3D11SwapChainResized( OnD3D11ResizedSwapChain );
    DXUTSetCallbackD3D11FrameRender( OnD3D11FrameRender );
    DXUTSetCallbackD3D11SwapChainReleasing( OnD3D11ReleasingSwapChain );
    DXUTSetCallbackD3D11DeviceDestroyed( OnD3D11DestroyDevice );
	
	
    // Perform any application-level initialization here
    DXUTInit( true, true, nullptr ); // Parse the command line, show msgboxes on error, no extra command line params
    DXUTSetCursorSettings( true, true ); // Show the cursor and clip it when in full screen
    DXUTCreateWindow( L"SnowmanScene" );

    // Only require 10-level hardware or later
    DXUTCreateDevice( D3D_FEATURE_LEVEL_10_0, true, 1000, 800 );

	// Enter into the DXUT render loop
    DXUTMainLoop(); 

    // Perform any application-level cleanup here

    return DXUTGetExitCode();
}


