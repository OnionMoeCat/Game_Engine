/*
This file contains the function declarations for MeshHelper
*/

#ifndef EAE6320_MATERIALHELPER_H
#define EAE6320_MATERIALHELPER_H

// Header Files
//=============
#include "Material.h"

#include "Context.h"

// Interface
//==========

namespace eae6320
{
	namespace Graphics
	{
		struct MaterialHelper
		{
		public:
			bool static LoadMaterialFromFile(Material& i_material, const char* const i_path);
			bool static SetMaterialUniforms(Material& i_material, const Context& i_context);
			bool static SetMaterialTextures(Material& i_material, const Context& i_context);
			bool static CleanUp(Material& i_material);		
		};
	}
}

#endif	// EAE6320_GRAPHICS_H
