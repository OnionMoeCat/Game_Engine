// Header Files
//=============

#include "../Graphics.h"

#include <cassert>
#include <cstdint>
#include <d3d9.h>
#include <d3dx9shader.h>
#include <sstream>
#include "../../UserOutput/UserOutput.h"

// Static Data Initialization
//===========================
namespace
{
	HWND s_renderingWindow = NULL;
	IDirect3D9* s_direct3dInterface = NULL;
	IDirect3DDevice9* s_direct3dDevice = NULL;

	eae6320::Graphics::Mesh s_mesh;

	// The vertex shader is a program that operates on vertices.
	// Its input comes from a C/C++ "draw call" and is:
	//	* Position
	//	* Any other data we want
	// Its output is:
	//	* Position
	//		(So that the graphics hardware knows which pixels to fill in for the triangle)
	//	* Any other data we want
	IDirect3DVertexShader9* s_vertexShader = NULL;
	// The fragment shader is a program that operates on fragments
	// (or potential pixels).
	// Its input is:
	//	* The data that was output from the vertex shader,
	//		interpolated based on how close the fragment is
	//		to each vertex in the triangle.
	// Its output is:
	//	* The final color that the pixel should be
	IDirect3DPixelShader9* s_fragmentShader = NULL;
}

// Helper Function Declarations
//=============================

namespace
{
	bool CreateDevice();
	bool CreateInterface();
	bool CreateVertexDeclaration();
	bool LoadFragmentShader();
	bool LoadMeshes();
	bool LoadVertexShader();
}

// Interface
//==========

bool eae6320::Graphics::Initialize( const HWND i_renderingWindow )
{
	s_renderingWindow = i_renderingWindow;

	// Initialize the interface to the Direct3D9 library
	if ( !CreateInterface() )
	{
		return false;
	}
	// Create an interface to a Direct3D device
	if ( !CreateDevice() )
	{
		goto OnError;
	}
	if ( !LoadMeshes() )
	{
		goto OnError;
	}
	// Initialize vertex declaration
	if ( !CreateVertexDeclaration() )
	{
		goto OnError;
	}
	if ( !LoadVertexShader() )
	{
		goto OnError;
	}
	if ( !LoadFragmentShader() )
	{
		goto OnError;
	}

	return true;

OnError:

	ShutDown();
	return false;
}

void eae6320::Graphics::Render()
{
	// Every frame an entirely new image will be created.
	// Before drawing anything, then, the previous image will be erased
	// by "clearing" the image buffer (filling it with a solid color)
	{
		const D3DRECT* subRectanglesToClear = NULL;
		const DWORD subRectangleCount = 0;
		const DWORD clearTheRenderTarget = D3DCLEAR_TARGET;
		D3DCOLOR clearColor;
		{
			// Black is usually used:
			clearColor = D3DCOLOR_XRGB( 0, 0, 0 );
		}
		const float noZBuffer = 0.0f;
		const DWORD noStencilBuffer = 0;
		HRESULT result = s_direct3dDevice->Clear( subRectangleCount, subRectanglesToClear,
			clearTheRenderTarget, clearColor, noZBuffer, noStencilBuffer );
		assert( SUCCEEDED( result ) );
	}

	// The actual function calls that draw geometry must be made between paired calls to
	// BeginScene() and EndScene()
	{
		HRESULT result = s_direct3dDevice->BeginScene();
		assert( SUCCEEDED( result ) );
		{
			// Set the shaders
			{
				result = s_direct3dDevice->SetVertexShader( s_vertexShader );
				assert( SUCCEEDED( result ) );
				result = s_direct3dDevice->SetPixelShader( s_fragmentShader );
				assert( SUCCEEDED( result ) );
			}
			// Bind a specific vertex buffer to the device as a data source
			{
				// There can be multiple streams of data feeding the display adaptor at the same time
				const unsigned int streamIndex = 0;
				// It's possible to start streaming data in the middle of a vertex buffer
				const unsigned int bufferOffset = 0;
				// The "stride" defines how large a single vertex is in the stream of data
				const unsigned int bufferStride = sizeof( sVertex );
				result = s_direct3dDevice->SetStreamSource( streamIndex, s_mesh.m_vertexBuffer, bufferOffset, bufferStride );
				assert( SUCCEEDED( result ) );
			}
			// Bind a specific index buffer to the device as a data source
			{
				result = s_direct3dDevice->SetIndices( s_mesh.m_indexBuffer );
				assert( SUCCEEDED( result ) );
			}
			// Render objects from the current streams
			{
				const eae6320::Graphics::RenderContext renderContext =
				{ s_direct3dDevice };

				if (!MeshHelper::DrawMesh(s_mesh, renderContext))
				{
					assert( false );
				}
			}
		}
		result = s_direct3dDevice->EndScene();
		assert( SUCCEEDED( result ) );
	}

	// Everything has been drawn to the "back buffer", which is just an image in memory.
	// In order to display it, the contents of the back buffer must be "presented"
	// (to the front buffer)
	{
		const RECT* noSourceRectangle = NULL;
		const RECT* noDestinationRectangle = NULL;
		const HWND useDefaultWindow = NULL;
		const RGNDATA* noDirtyRegion = NULL;
		HRESULT result = s_direct3dDevice->Present( noSourceRectangle, noDestinationRectangle, useDefaultWindow, noDirtyRegion );
		assert( SUCCEEDED( result ) );
	}
}

bool eae6320::Graphics::ShutDown()
{
	bool wereThereErrors = false;

	if ( s_direct3dInterface )
	{
		if ( s_direct3dDevice )
		{
			if ( s_vertexShader )
			{
				s_vertexShader->Release();
				s_vertexShader = NULL;
			}
			if ( s_fragmentShader )
			{
				s_fragmentShader->Release();
				s_fragmentShader = NULL;
			}
			const eae6320::Graphics::CleanUpContext finalizeContext = { s_direct3dDevice };
			eae6320::Graphics::MeshHelper::CleanUp(s_mesh, finalizeContext);

			s_direct3dDevice->Release();
			s_direct3dDevice = NULL;
		}

		s_direct3dInterface->Release();
		s_direct3dInterface = NULL;
	}
	s_renderingWindow = NULL;

	return !wereThereErrors;
}

// Helper Function Definitions
//============================

namespace
{
	bool CreateDevice()
	{
		const UINT useDefaultDevice = D3DADAPTER_DEFAULT;
		const D3DDEVTYPE useHardwareRendering = D3DDEVTYPE_HAL;
		const DWORD useHardwareVertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
		D3DPRESENT_PARAMETERS presentationParameters = { 0 };
		{
			{
				const unsigned int useRenderingWindowDimensions = 0;
				presentationParameters.BackBufferWidth = useRenderingWindowDimensions;
				presentationParameters.BackBufferHeight = useRenderingWindowDimensions;
			}
			presentationParameters.BackBufferFormat = D3DFMT_X8R8G8B8;
			presentationParameters.BackBufferCount = 1;
			presentationParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
			presentationParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
			presentationParameters.hDeviceWindow = s_renderingWindow;
			presentationParameters.Windowed = TRUE;
			presentationParameters.EnableAutoDepthStencil = FALSE;
			presentationParameters.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
		}
		HRESULT result = s_direct3dInterface->CreateDevice( useDefaultDevice, useHardwareRendering,
			s_renderingWindow, useHardwareVertexProcessing, &presentationParameters, &s_direct3dDevice );
		if ( SUCCEEDED( result ) )
		{
			return true;
		}
		else
		{
			eae6320::UserOutput::Print( "Direct3D failed to create a Direct3D9 device" );
			return false;
		}
	}

	bool CreateInterface()
	{
		// D3D_SDK_VERSION is #defined by the Direct3D header files,
		// and is just a way to make sure that everything is up-to-date
		s_direct3dInterface = Direct3DCreate9( D3D_SDK_VERSION );
		if ( s_direct3dInterface )
		{
			return true;
		}
		else
		{
			eae6320::UserOutput::Print( "DirectX failed to create a Direct3D9 interface" );
			return false;
		}
	}

	bool CreateVertexDeclaration()
	{
		// Initialize the vertex format
		{
			// These elements must match the VertexFormat::sVertex layout struct exactly.
			// They instruct Direct3D how to match the binary data in the vertex buffer
			// to the input elements in a vertex shader
			// (by using D3DDECLUSAGE enums here and semantics in the shader,
			// so that, for example, D3DDECLUSAGE_POSITION here matches with POSITION in shader code).
			// Note that OpenGL uses arbitrarily assignable number IDs to do the same thing.
			eae6320::Graphics::VertexDeclarationSpec vertexElements[] =
			{
				// Stream 0

				// POSITION
				// 2 floats == 8 bytes
				// Offset = 0
				{ 0, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },

				// COLOR0
				// D3DCOLOR == 4 bytes
				// Offset = 8
				{ 0, 8, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },

				// The following marker signals the end of the vertex declaration
				D3DDECL_END()
			};
			const unsigned int vertexElementsCount = 2;
			const eae6320::Graphics::VertexDeclarationContext vertexDeclarationContext = { s_direct3dDevice , vertexElementsCount };
			if (!eae6320::Graphics::MeshHelper::SetVertexDeclaration(s_mesh, vertexElements, vertexDeclarationContext))
			{
				return false;
			}
		}

		return true;
	}

	bool LoadFragmentShader()
	{
		// Load the source code from file and compile it
		ID3DXBuffer* compiledShader;
		{
			const char* sourceCodeFileName = "data/fragment.shader";
			const D3DXMACRO defines[] =
			{
				{ "EAE6320_PLATFORM_D3D", "1" },
				{ NULL, NULL }
			};
			ID3DXInclude* noIncludes = NULL;
			const char* entryPoint = "main";
			const char* profile = "ps_3_0";
			const DWORD noFlags = 0;
			ID3DXBuffer* errorMessages = NULL;
			ID3DXConstantTable** noConstants = NULL;
			HRESULT result = D3DXCompileShaderFromFile( sourceCodeFileName, defines, noIncludes, entryPoint, profile, noFlags,
				&compiledShader, &errorMessages, noConstants );
			if ( SUCCEEDED( result ) )
			{
				if ( errorMessages )
				{
					errorMessages->Release();
				}
			}
			else
			{
				if ( errorMessages )
				{
					std::stringstream errorMessage;
					errorMessage << "Direct3D failed to compile the fragment shader from the file " << sourceCodeFileName
						<< ":\n" << reinterpret_cast<char*>( errorMessages->GetBufferPointer() );
					eae6320::UserOutput::Print( errorMessage.str() );
					errorMessages->Release();
				}
				else
				{
					std::stringstream errorMessage;
					errorMessage << "Direct3D failed to compile the fragment shader from the file " << sourceCodeFileName;
					eae6320::UserOutput::Print( errorMessage.str() );
				}
				return false;
			}
		}
		// Create the fragment shader object
		bool wereThereErrors = false;
		{
			HRESULT result = s_direct3dDevice->CreatePixelShader( reinterpret_cast<DWORD*>( compiledShader->GetBufferPointer() ),
				&s_fragmentShader );
			if ( FAILED( result ) )
			{
				eae6320::UserOutput::Print( "Direct3D failed to create the fragment shader" );
				wereThereErrors = true;
			}
			compiledShader->Release();
		}
		return !wereThereErrors;
	}
	
	bool LoadMeshes()
	{
		eae6320::Graphics::LoadMeshContext loadMeshContext = {s_direct3dDevice};
		eae6320::Graphics::MeshHelper::ReadMeshFromFile(s_mesh, "data/square.mesh.raw", loadMeshContext);
		return true;
	}

	bool LoadVertexShader()
	{
		// Load the source code from file and compile it
		ID3DXBuffer* compiledShader;
		{
			const char* sourceCodeFileName = "data/vertex.shader";
			const D3DXMACRO defines[] =
			{
				{ "EAE6320_PLATFORM_D3D", "1" },
				{ NULL, NULL }
			};
			ID3DXInclude* noIncludes = NULL;
			const char* entryPoint = "main";
			const char* profile = "vs_3_0";
			const DWORD noFlags = 0;
			ID3DXBuffer* errorMessages = NULL;
			ID3DXConstantTable** noConstants = NULL;
			HRESULT result = D3DXCompileShaderFromFile( sourceCodeFileName, defines, noIncludes, entryPoint, profile, noFlags,
				&compiledShader, &errorMessages, noConstants );
			if ( SUCCEEDED( result ) )
			{
				if ( errorMessages )
				{
					errorMessages->Release();
				}
			}
			else
			{
				if ( errorMessages )
				{
					std::stringstream errorMessage;
					errorMessage << "Direct3D failed to compile the vertex shader from the file " << sourceCodeFileName
						<< ":\n" << reinterpret_cast<char*>( errorMessages->GetBufferPointer() );
					eae6320::UserOutput::Print( errorMessage.str() );
					errorMessages->Release();
				}
				else
				{
					std::stringstream errorMessage;
					errorMessage << "Direct3D failed to compile the vertex shader from the file " << sourceCodeFileName;
					eae6320::UserOutput::Print( errorMessage.str() );
				}
				return false;
			}
		}
		// Create the vertex shader object
		bool wereThereErrors = false;
		{
			HRESULT result = s_direct3dDevice->CreateVertexShader( reinterpret_cast<DWORD*>( compiledShader->GetBufferPointer() ),
				&s_vertexShader );
			if ( FAILED( result ) )
			{
				eae6320::UserOutput::Print( "Direct3D failed to create the vertex shader" );
				wereThereErrors = true;
			}
			compiledShader->Release();
		}
		return !wereThereErrors;
	}
}
