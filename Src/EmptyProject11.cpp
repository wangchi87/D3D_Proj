//--------------------------------------------------------------------------------------
// File: EmptyProject11.cpp
//
// Empty starting point for new Direct3D 11 Win32 desktop applications
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

#include "D3DCreator.h"

#pragma warning( disable : 4100 )



// global variables
MyD3DCreator dc;

//ID3D11VertexShader*			g_pVertexShader = nullptr;
//ID3D11PixelShader*			g_pPixelShader = nullptr;
//ID3D11InputLayout*			g_pVertexLayout = nullptr;
//ID3D11Buffer*				g_pVertexBuffer = nullptr;
//ID3D11Buffer*				g_pIndexBuffer = nullptr;
//ID3D11Buffer*				g_pConstantBuffer = nullptr;
//XMMATRIX                g_World;
//XMMATRIX                g_View;
//XMMATRIX                g_Projection;

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
	
	dc.InitCreator ( pd3dDevice , pBackBufferSurfaceDesc , pUserContext );

	dc.AddResources ();

	/*
	D3D11_BUFFER_DESC bd;
	ZeroMemory ( &bd , sizeof ( bd ) );
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof ( ConstantBuffer );
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hr = pd3dDevice->CreateBuffer ( &bd , nullptr , &g_pConstantBuffer );
	if (FAILED ( hr ))
	{
		return hr;
	}

	g_World = XMMatrixIdentity ();

	XMVECTOR Eye = XMVectorSet ( 0.0f , 1.0f , -5.0f , 0.0f );
	XMVECTOR At = XMVectorSet ( 0.0f , 1.0f , 0.0f , 0.0f );
	XMVECTOR Up = XMVectorSet ( 0.0f , 1.0f , 0.0f , 0.0f );

	g_View = XMMatrixLookAtLH ( Eye , At , Up );

	g_Projection = XMMatrixPerspectiveFovLH ( XM_PIDIV2 , 8/(float)6 , 0.01f , 100.0f );

	D3D11_MAPPED_SUBRESOURCE MappedResource;
	V ( pd3dImmediateContext->Map ( g_pConstantBuffer , 0 , D3D11_MAP_WRITE_DISCARD , 0 , &MappedResource ) );
	auto pCB = reinterpret_cast<ConstantBuffer*>( MappedResource.pData );
	DirectX::XMStoreFloat4x4 ( &pCB->World , XMMatrixTranspose ( g_World ) );
	DirectX::XMStoreFloat4x4 ( &pCB->View , XMMatrixTranspose ( g_View ) );
	DirectX::XMStoreFloat4x4 ( &pCB->Projection , XMMatrixTranspose ( g_Projection ) );
	pd3dImmediateContext->Unmap ( g_pConstantBuffer , 0 );
	*/
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
	dc.RenderScene ( fTime , fElapsedTime , pUserContext );


	printf ( "time : %f" , fTime );
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
	dc.~MyD3DCreator ();
}


//--------------------------------------------------------------------------------------
// Handle messages to the application
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
                          bool* pbNoFurtherProcessing, void* pUserContext )
{
    return 0;
}


//--------------------------------------------------------------------------------------
// Handle key presses
//--------------------------------------------------------------------------------------
void CALLBACK OnKeyboard( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
}


//--------------------------------------------------------------------------------------
// Handle mouse button presses
//--------------------------------------------------------------------------------------
void CALLBACK OnMouse( bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown,
                       bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta,
                       int xPos, int yPos, void* pUserContext )
{
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
    DXUTCreateWindow( L"EmptyProject11" );

    // Only require 10-level hardware or later
    DXUTCreateDevice( D3D_FEATURE_LEVEL_10_0, true, 800, 600 );
    DXUTMainLoop(); // Enter into the DXUT ren  der loop

    // Perform any application-level cleanup here

    return DXUTGetExitCode();
}


