/*
This file contains the function declarations for MeshHelper
*/

#ifndef EAE6320_EFFECTHELPER_H
#define EAE6320_EFFECTHELPER_H

// Header Files
//=============
#include "Effect.h"
#include "CrossPlatformDefine.h"

// Interface
//==========

namespace eae6320
{
	namespace Graphics
	{
		struct EffectHelper
		{
			bool static LoadEffectFromFile(Effect& i_effect, const char* const i_vertexPath, const char* const i_fragmentPath, const LoadEffectContext& i_loadEffectContext);
			bool static Bind(Effect& i_effect, const BindEffectContext& i_bindEffectContext);
			bool static CleanUp(Effect& i_mesh, const CleanUpEffectContext& i_cleanUpEffectContext);
		};
	}
}

#endif	// EAE6320_GRAPHICS_H
