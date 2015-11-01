/*
This file contains the function declarations for MeshHelper
*/

#ifndef EAE6320_ENTITYHELPER_H
#define EAE6320_ENTITYHELPER_H

// Header Files
//=============
#include "Renderable.h"
#include "../Math/cVector.h"

// Interface
//==========

namespace eae6320
{
	namespace Graphics
	{
		struct RenderableHelper
		{
			bool static LoadEntityFromFile(Renderable& i_entity, const char* const i_effectPath, const char* i_meshPath);
			void static OffsetPosition(Renderable& i_entity, const eae6320::Math::cVector& i_offset_position);
			bool static CleanUp(Renderable& i_entity);
		};
	}
}

#endif	// EAE6320_GRAPHICS_H
