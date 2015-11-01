/*
This file contains the function declarations for graphics
*/

#ifndef EAE6320_CONTEXT_H
#define EAE6320_CONTEXT_H

#include "../Windows/Includes.h"

// Header Files
//=============

#if defined( EAE6320_PLATFORM_D3D )
#include <d3d9.h>
#elif defined( EAE6320_PLATFORM_GL )
#include <gl/GL.h>
#include <gl/GLU.h>
#include "../../External/OpenGlExtensions/OpenGlExtensions.h"
#endif

namespace eae6320
{
	namespace Graphics
	{
		struct Context
		{
#if defined( EAE6320_PLATFORM_D3D )
			IDirect3DDevice9* device = NULL;
#elif defined( EAE6320_PLATFORM_GL )
			HDC device = NULL;
#endif
			static Context& Get();
		private:
			static Context m_Instance;
		};
	}
}

#endif
