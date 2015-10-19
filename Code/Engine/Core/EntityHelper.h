/*
This file contains the function declarations for MeshHelper
*/

#ifndef EAE6320_EFFECTHELPER_H
#define EAE6320_EFFECTHELPER_H

// Header Files
//=============
#include "Entity.h"
#include "CrossPlatformDefine.h"
#include "../../Math/cVector.h"

// Interface
//==========

namespace eae6320
{
	namespace Core
	{
		struct EntityHelper
		{
			bool static LoadEffectFromFile(Entity& i_effect, const char* const i_vertexPath, const char* const i_fragmentPath, const char* i_meshPath);
		};
	}
}

#endif	// EAE6320_GRAPHICS_H
