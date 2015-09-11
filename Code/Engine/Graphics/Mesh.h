/*
This file contains the function declarations for mesh
*/

#ifndef EAE6320_MESH_H
#define EAE6320_MESH_H

// Header Files
//=============
#include "../Windows/Includes.h"

#if defined( EAE6320_PLATFORM_D3D )
#include <d3d9.h>
//QUESTION: why d3d9xshader.h doesn't work here
#elif defined( EAE6320_PLATFORM_GL )
#include <gl/GL.h>
#include <gl/GLU.h>
#endif
// Interface
//==========

namespace eae6320
{
	namespace Graphics
	{
		struct Mesh
		{
#if defined( EAE6320_PLATFORM_D3D )
			IDirect3DVertexDeclaration9* m_vertexDeclaration = NULL;
			IDirect3DVertexBuffer9* m_vertexBuffer = NULL;
			IDirect3DIndexBuffer9* m_indexBuffer = NULL;
#elif defined( EAE6320_PLATFORM_GL )
			GLuint m_vertexArrayId = 0;
			GLuint m_vertexBufferId = 0;
			GLuint m_indexBufferId = 0;
#endif
		};
	}
}

#endif	// EAE6320_MESH_H
