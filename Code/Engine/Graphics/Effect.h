/*
This file contains the function declarations for effect
*/

#ifndef EAE6320_EFFECT_H
#define EAE6320_EFFECT_H

// Header Files
//=============

#include "../Windows/Includes.h"

#include <cstdint>

#if defined( EAE6320_PLATFORM_D3D )
#include <d3d9.h>
#include <d3dx9shader.h>
#elif defined( EAE6320_PLATFORM_GL )
#include <gl/GL.h>
#include <gl/GLU.h>
#endif

#include "../Math/cMatrix_transformation.h"

// Interface
//==========

namespace eae6320
{
	namespace Graphics
	{
		struct Effect
		{

#if defined( EAE6320_PLATFORM_D3D )
			IDirect3DVertexShader9* m_vertexShader = NULL;
			IDirect3DPixelShader9* m_fragmentShader = NULL;
			ID3DXConstantTable* m_vertexShaderConstantTable = NULL;
			ID3DXConstantTable* m_fragmentShaderConstantTable = NULL;
#elif defined( EAE6320_PLATFORM_GL )
			GLuint m_programID = 0;
#endif
			uint8_t m_renderStates = 0;	
		};
	}
}

#endif	// EAE6320_MESH_H
