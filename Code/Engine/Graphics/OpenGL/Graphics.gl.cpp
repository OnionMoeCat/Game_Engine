// Header Files
//=============

#include "../Graphics.h"

#include <cassert>
#include <gl/GL.h>
#include <string>
#include <sstream>
#include "../../UserOutput/UserOutput.h"
#include "../../Windows/Functions.h"
#include "../../../External/OpenGlExtensions/OpenGlExtensions.h"
#include "../../Math/cVector.h"

// Static Data Initialization
//===========================

namespace
{
	HWND s_renderingWindow = NULL;
	HDC s_deviceContext = NULL;
	HGLRC s_openGlRenderingContext = NULL;

	eae6320::Graphics::Mesh s_squareMesh;
	eae6320::Graphics::Mesh s_triangleMesh1;
	eae6320::Graphics::Mesh s_triangleMesh2;
	eae6320::Graphics::Effect s_effect;
	eae6320::Graphics::Context s_context;
	eae6320::Math::cVector s_position_offset_triangleMesh1(0.1f, 0.1f);
	eae6320::Math::cVector s_position_offset_triangleMesh2(-0.1f, 0.1f);
	eae6320::Math::cVector s_position_offset_squareMesh(0.1f, 0.1f);
}

// Helper Function Declarations
//=============================

namespace
{
	bool CreateContext();
	bool CreateRenderingContext();
	bool LoadEffect();
	bool LoadMeshes();
}

// Interface
//==========

bool eae6320::Graphics::Initialize( const HWND i_renderingWindow )
{
	s_renderingWindow = i_renderingWindow;

	// Create an OpenGL rendering context
	if ( !CreateRenderingContext() )
	{
		goto OnError;
	}

	// Load any required OpenGL extensions
	{
		std::string errorMessage;
		if ( !OpenGlExtensions::Load( &errorMessage ) )
		{
			UserOutput::Print( errorMessage );
			goto OnError;
		}
	}

	if (!CreateContext())
	{
		goto OnError;
	}

	if (!LoadMeshes())
	{
		goto OnError;
	}

	if ( !LoadEffect() )
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
		// Black is usually used
		glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
		assert( glGetError() == GL_NO_ERROR );
		// In addition to the color, "depth" and "stencil" can also be cleared,
		// but for now we only care about color
		const GLbitfield clearColor = GL_COLOR_BUFFER_BIT;
		glClear( clearColor );
		assert( glGetError() == GL_NO_ERROR );
	}

	// The actual function calls that draw geometry
	{
		// Set the vertex and fragment shaders
		{
			if (!EffectHelper::Bind(s_effect, s_context))
			{
				assert(false);
			}
		}
		// Render objects from the current streams
		{
			if (!EffectHelper::SetDrawCallUniforms(s_effect, s_position_offset_squareMesh, s_context))
			{
				assert(false);
			}
			if (!MeshHelper::DrawMesh(s_squareMesh, s_context))
			{
				assert(false);
			}
			if (!EffectHelper::SetDrawCallUniforms(s_effect, s_position_offset_triangleMesh1, s_context))
			{
				assert(false);
			}
			if (!MeshHelper::DrawMesh(s_triangleMesh1, s_context))
			{
				assert(false);
			}
			if (!EffectHelper::SetDrawCallUniforms(s_effect, s_position_offset_triangleMesh2, s_context))
			{
				assert(false);
			}
			if (!MeshHelper::DrawMesh(s_triangleMesh2, s_context))
			{
				assert(false);
			}
		}
	}

	// Everything has been drawn to the "back buffer", which is just an image in memory.
	// In order to display it, the contents of the back buffer must be swapped with the "front buffer"
	// (which is what the user sees)
	{
		BOOL result = SwapBuffers( s_deviceContext );
		assert( result != FALSE );
	}
}

bool eae6320::Graphics::ShutDown()
{
	bool wereThereErrors = false;

	if ( s_openGlRenderingContext != NULL )
	{
		{
			if (!EffectHelper::CleanUp(s_effect, s_context))
			{
				wereThereErrors = true;
			}
		}

		{
			if (!MeshHelper::CleanUp(s_squareMesh, s_context))
			{
				wereThereErrors = true;
			}
			if (!MeshHelper::CleanUp(s_triangleMesh1, s_context))
			{
				wereThereErrors = true;
			}
		}

		if ( wglMakeCurrent( s_deviceContext, NULL ) != FALSE )
		{
			if ( wglDeleteContext( s_openGlRenderingContext ) == FALSE )
			{
				std::stringstream errorMessage;
				errorMessage << "Windows failed to delete the OpenGL rendering context: " << GetLastWindowsError();
				UserOutput::Print( errorMessage.str() );
				wereThereErrors = true;
			}
		}
		else
		{
			std::stringstream errorMessage;
			errorMessage << "Windows failed to unset the current OpenGL rendering context: " << GetLastWindowsError();
			UserOutput::Print( errorMessage.str() );
			wereThereErrors = true;
		}
		s_openGlRenderingContext = NULL;
	}

	if ( s_deviceContext != NULL )
	{
		// The documentation says that this call isn't necessary when CS_OWNDC is used
		ReleaseDC( s_renderingWindow, s_deviceContext );
		s_deviceContext = NULL;
	}

	s_renderingWindow = NULL;

	return !wereThereErrors;
}

// Helper Function Declarations
//=============================

namespace
{
	bool CreateContext()
	{
		s_context = {};
		return true;
	}

	bool CreateRenderingContext()
	{
		// A "device context" can be thought of an abstraction that Windows uses
		// to represent the graphics adaptor used to display a given window
		s_deviceContext = GetDC( s_renderingWindow );
		if ( s_deviceContext == NULL )
		{
			eae6320::UserOutput::Print( "Windows failed to get the device context" );
			return false;
		}
		// Windows requires that an OpenGL "render context" is made for the window we want to use to render into
		{
			// Set the pixel format of the rendering window
			{
				PIXELFORMATDESCRIPTOR desiredPixelFormat = { 0 };
				{
					desiredPixelFormat.nSize = sizeof( PIXELFORMATDESCRIPTOR );
					desiredPixelFormat.nVersion = 1;

					desiredPixelFormat.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
					desiredPixelFormat.iPixelType = PFD_TYPE_RGBA;
					desiredPixelFormat.cColorBits = 32;
					desiredPixelFormat.iLayerType = PFD_MAIN_PLANE ;
				}
				// Get the ID of the desired pixel format
				int pixelFormatId;
				{
					pixelFormatId = ChoosePixelFormat( s_deviceContext, &desiredPixelFormat );
					if ( pixelFormatId == 0 )
					{
						std::stringstream errorMessage;
						errorMessage << "Windows couldn't choose the closest pixel format: " << eae6320::GetLastWindowsError();
						eae6320::UserOutput::Print( errorMessage.str() );
						return false;
					}
				}
				// Set it
				if ( SetPixelFormat( s_deviceContext, pixelFormatId, &desiredPixelFormat ) == FALSE )
				{
					std::stringstream errorMessage;
					errorMessage << "Windows couldn't set the desired pixel format: " << eae6320::GetLastWindowsError();
					eae6320::UserOutput::Print( errorMessage.str() );
					return false;
				}
			}
			// Create the OpenGL rendering context
			s_openGlRenderingContext = wglCreateContext( s_deviceContext );
			if ( s_openGlRenderingContext == NULL )
			{
				std::stringstream errorMessage;
				errorMessage << "Windows failed to create an OpenGL rendering context: " << eae6320::GetLastWindowsError();
				eae6320::UserOutput::Print( errorMessage.str() );
				return false;
			}
			// Set it as the rendering context of this thread
			if ( wglMakeCurrent( s_deviceContext, s_openGlRenderingContext ) == FALSE )
			{
				std::stringstream errorMessage;
				errorMessage << "Windows failed to set the current OpenGL rendering context: " << eae6320::GetLastWindowsError();
				eae6320::UserOutput::Print( errorMessage.str() );
				return false;
			}
		}

		return true;
	}

	bool LoadEffect()
	{
		eae6320::Graphics::EffectHelper::LoadEffectFromFile(s_effect, "data/vertex.shader", "data/fragment.shader", s_context);
		return true;
	}

	bool LoadMeshes()
	{
		eae6320::Graphics::MeshHelper::LoadMeshFromFile(s_squareMesh, "data/square.mesh", s_context);
		eae6320::Graphics::MeshHelper::LoadMeshFromFile(s_triangleMesh1, "data/triangle.mesh", s_context);
		eae6320::Graphics::MeshHelper::LoadMeshFromFile(s_triangleMesh2, "data/triangle.mesh", s_context);
		return true;
	}

}
