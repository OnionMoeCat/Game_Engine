/*
This file contains the function declarations for MeshHelper
*/

#ifndef EAE6320_EFFECTHELPER_H
#define EAE6320_EFFECTHELPER_H

// Header Files
//=============
#include "Context.h"
#include "Effect.h"
#include "Includes.h"
#include "../Math/cVector.h"

// Interface
//==========

namespace eae6320
{
	namespace Graphics
	{
		struct EffectHelper
		{
		private:
			bool static LoadEffectFromFile(Effect& i_effect, const char* const i_vertexPath, const char* const i_fragmentPath, const Context& i_context);
			bool static CleanUp(Effect& i_effect, const Context& i_context);
		public:
			bool static LoadEffectFromFile(Effect& i_effect, const char* const i_effectPath);
			bool static CleanUp(Effect& i_effect);
			bool static SetDrawCallUniforms(Effect& i_effect, const Context& i_Context);
			bool static Bind(Effect& i_effect, const Context& i_context);
		};
	}
}

#endif	// EAE6320_GRAPHICS_H
