/*
	This file contains the function declarations for graphics
*/

#ifndef EAE6320_GRAPHICS_H
#define EAE6320_GRAPHICS_H

// Header Files
//=============

#include "../Windows/Includes.h"
#include "Mesh.h"
#include "MeshHelper.h"
#include "Includes.h"
#include "Effect.h"
#include "EffectHelper.h"
#include "Context.h"

// Interface
//==========

namespace eae6320
{
	namespace Graphics
	{
		struct Core
		{
		public:
			bool static Initialize(const HWND i_renderingWindow);
			void static Render();
			bool static ShutDown();
		private:
			bool static Clear(sColor color, float depth, Context context);
			bool static OnSubmitRenderCommands_start(Context context);
			bool static OnSubmitRenderCommands_end(Context context);
			bool static DisplayRenderedBuffer(Context context);
		};

	}
}

#endif	// EAE6320_GRAPHICS_H
