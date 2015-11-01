/*
This file contains the function declarations for graphics
*/

#ifndef EAE6320_INCLUDES_H
#define EAE6320_INCLUDES_H



namespace eae6320
{
	namespace Graphics
	{
		namespace ShaderTypes
		{
#ifdef _DEBUG
			#define EAE6320_GRAPHICS_AREDEBUGSHADERSENABLED
#endif

#if defined( EAE6320_PLATFORM_D3D )
			enum eShaderType { Unknown, Vertex, Fragment };
#endif
		}
	}
}

#endif	// EAE6320_INCLUDES_H