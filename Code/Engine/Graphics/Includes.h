/*
This file contains the function declarations for graphics
*/

#ifndef EAE6320_INCLUDES_H
#define EAE6320_INCLUDES_H

#include "../Windows/Includes.h"

#include <cstdint>
#include <cmath>

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
		
#ifdef _DEBUG
#define EAE6320_GRAPHICS_AREDEBUGSHADERSENABLED
#endif

#if defined( EAE6320_PLATFORM_D3D )
		typedef void* BufferDataPtr;
#elif defined( EAE6320_PLATFORM_GL )
		typedef GLvoid* BufferDataPtr;
#endif

		typedef
#if defined( EAE6320_PLATFORM_D3D )
			DWORD
#elif defined( EAE6320_PLATFORM_GL )
			GLint
#endif
			SamplerID;

		typedef
#if defined( EAE6320_PLATFORM_D3D )
			IDirect3DTexture9*
#elif defined( EAE6320_PLATFORM_GL )
			GLuint
#endif
			TextureData;

		typedef
#if defined( EAE6320_PLATFORM_D3D )
			// This is conceptually a D3DXHANDLE but is defined this way
			// so that external files aren't required to specify the #include path to the DirectX SDK
			const char*
#elif defined( EAE6320_PLATFORM_GL )
			GLint
#endif
			tUniformHandle;

#if defined( EAE6320_PLATFORM_GL )
#define EAE6320_GRAPHICS_ISTEXTUREINDEXREQUIREDTOSETTEXTURE
#endif

		// This struct determines the layout of the data that the CPU will send to the GPU
		struct sVertex
		{
			// POSITION
			// 3 floats == 12 bytes
			// Offset = 0
			float x, y, z;
			// COLOR0
			// 4 uint8_ts == 4 bytes
			// Offset = 12
#if defined( EAE6320_PLATFORM_D3D )
			uint8_t b, g, r, a;	// Direct3D expects the byte layout of a color to be different from what you might expect
#elif defined( EAE6320_PLATFORM_GL )
			uint8_t r, g, b, a;	// Direct3D expects the byte layout of a color to be different from what you might expect
#endif
			// NORMAL
			// 3 floats == 12 bits
			// Offset = 16
			float nx, ny, nz;
			// TANGENT
			// 3 floats == 12 bits
			// Offset = 28
			float tx, ty, tz;
			// BITANGENT
			// 3 floats == 12 bits
			// Offset = 40
			float bx, by, bz;
			// TEXTURE COORDINATE
			// 2 floats == 8 bytes
			// Offset = 52
			float u, v;
		};

		namespace ShaderTypes
		{
			enum eShaderType: uint8_t { Unknown, Vertex, Fragment };
		}

		struct sColor
		{
			float r, g, b, a;
		};

		struct ColorHelper
		{
			static uint8_t ColorFloatToUint8(float i_float)
			{
				return static_cast<uint8_t>(fmin(floor(i_float * 256), 255));
			}
		};

	}
}
#endif	// EAE6320_INCLUDES_H