/*
This file contains the function declarations for graphics
*/

#ifndef EAE6320_CROSSPLATFORMDEFINE_H
#define EAE6320_CROSSPLATFORMDEFINE_H

// Header Files
//=============
#include "../Windows/Includes.h"

#include <cstdint>

#if defined( EAE6320_PLATFORM_D3D )
#include <d3d9.h>
#elif defined( EAE6320_PLATFORM_GL )
#include <gl/GL.h>
#include <gl/GLU.h>
#include "../../External/OpenGlExtensions/OpenGlExtensions.h"
#endif

// Interface
//==========

namespace eae6320
{
	namespace Graphics
	{
		struct Context
		{
#if defined( EAE6320_PLATFORM_D3D )
			IDirect3DDevice9* device;
#endif
		};

#if defined( EAE6320_PLATFORM_D3D )
		typedef void* BufferDataPtr;
#elif defined( EAE6320_PLATFORM_GL )
		typedef GLvoid* BufferDataPtr;
#endif

		// This struct determines the layout of the data that the CPU will send to the GPU
		struct sVertex
		{
			// POSITION
			// 2 floats == 8 bytes
			// Offset = 0
			float x, y;
			// COLOR0
			// 4 uint8_ts == 4 bytes
			// Offset = 8
#if defined( EAE6320_PLATFORM_D3D )
			uint8_t b, g, r, a;	// Direct3D expects the byte layout of a color to be different from what you might expect
#elif defined( EAE6320_PLATFORM_GL )
			uint8_t r, g, b, a;	// Direct3D expects the byte layout of a color to be different from what you might expect
#endif
		};
	}
}

#endif	// EAE6320_DEFINE_H
