/*
This file contains the function declarations for MeshHelper
*/

#ifndef EAE6320_EFFECTHELPER_H
#define EAE6320_EFFECTHELPER_H

// Header Files
//=============
#include "Effect.h"
#include "CrossPlatformDefine.h"
#include "../Math/cVector.h"

// Interface
//==========

namespace eae6320
{
	namespace Graphics
	{
		struct EffectHelper
		{
			bool static LoadEffectFromFile(Effect& i_effect, const char* const i_vertexPath, const char* const i_fragmentPath, const Context& i_context);
			bool static Bind(Effect& i_effect, const Context& i_context);
			bool static CleanUp(Effect& i_effect, const Context& i_context);
			bool static SetDrawCallUniforms(Effect& i_effect, const eae6320::Math::cVector& i_vector, const Context& i_Context);
		};
	}
}

#endif	// EAE6320_GRAPHICS_H
