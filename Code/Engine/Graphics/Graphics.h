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
		bool Initialize( const HWND i_renderingWindow );
		void Render();
		bool ShutDown();
		bool Clear(sColor color, Context context);
		bool OnSubmitRenderCommands_start(Context context);
		bool OnSubmitRenderCommands_end(Context context);
		bool DisplayRenderedBuffer(Context context);
	}
}

#endif	// EAE6320_GRAPHICS_H
