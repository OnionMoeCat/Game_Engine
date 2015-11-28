/*
This file contains the function declarations for effect
*/

#ifndef EAE6320_MATERIAL_H
#define EAE6320_MATERIAL_H

// Header Files
//=============

#include "Effect.h"
#include "Uniform.h"
#include "Texture.h"

// Interface
//==========

namespace eae6320
{
	namespace Graphics
	{
		struct Material
		{
			Effect* m_effect = NULL;
			Uniform* m_uniform = NULL;			
			uint32_t m_uniformCount = 0;
			Texture* m_texture = NULL;
			uint32_t m_textureCount = 0;
		};
	}
}

#endif	// EAE6320_MESH_H
