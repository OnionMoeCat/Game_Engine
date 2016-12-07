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

#include <map>

// Interface
//==========

namespace eae6320
{
	namespace Graphics
	{
		struct Material
		{
			Effect* m_effect = NULL;
			UniformVector* m_uniformVector = NULL;			
			uint32_t m_uniformVectorCount = 0;
			UniformMatrix* m_uniformMatrixAdded = NULL;
			uint32_t m_uniformMatrixAddedCount = 0;
			uint32_t m_uniformMatrixAddedCapacity = 0;
			UniformVector* m_uniformVectorAdded = NULL;
			uint32_t m_uniformVectorAddedCount = 0;
			uint32_t m_uniformVectorAddedCapacity = 0;
			Texture* m_texture = NULL;
			uint32_t m_textureCount = 0;
			std::map<std::string, uint32_t> m_uniformMatrixMap;
			std::map<std::string, uint32_t> m_uniformVectorMap;
		};
	}
}

#endif	// EAE6320_MESH_H
