/*
This file contains the function declarations for effect
*/

#ifndef EAE6320_TEXTURE_H
#define EAE6320_TEXTURE_H

// Header Files
//=============

#include "Includes.h"

// Interface
//==========

namespace eae6320
{
	namespace Graphics
	{
		struct Texture
		{
			SamplerID samplerID = 0;
			TextureData textureData = 0;
		};
	}
}

#endif	// EAE6320_MESH_H
