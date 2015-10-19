/*
This file contains the function declarations for MeshHelper
*/

#ifndef EAE6320_ENTITYHELPER_H
#define EAE6320_ENTITYHELPER_H

// Header Files
//=============
#include "Entity.h"
#include "../Math/cVector.h"

// Interface
//==========

namespace eae6320
{
	namespace Core
	{
		struct EntityHelper
		{
			bool static LoadEntityFromFile(Entity& i_effect, const char* const i_vertexPath, const char* const i_fragmentPath, const char* i_meshPath);
			bool static OffsetPosition(Entity& i_entity, const eae6320::Math::cVector& i_offset_position);
		};
	}
}

#endif	// EAE6320_GRAPHICS_H
