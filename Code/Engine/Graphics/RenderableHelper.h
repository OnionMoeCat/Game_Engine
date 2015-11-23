/*
This file contains the function declarations for MeshHelper
*/

#ifndef EAE6320_RENDERABLE_HELPER_H
#define EAE6320_RENDERABLE_HELPER_H

// Header Files
//=============
#include "Renderable.h"

// Interface
//==========

namespace eae6320
{
	namespace Graphics
	{
		struct RenderableHelper
		{			
			bool static CleanUp(Renderable& i_renderable);
			bool static LoadRenderableFromFile(Renderable& i_renderable, const char* const i_effectPath, const char* i_meshPath);
		};
	}
}

#endif	// EAE6320_GRAPHICS_H
