// Header Files
//=============

#include "Graphics.h"

#include <cassert>
#include <gl/GL.h>
#include <string>
#include <sstream>
#include "../UserOutput/UserOutput.h"
#include "../Windows/Functions.h"
#include "../../External/OpenGlExtensions/OpenGlExtensions.h"
#include "../Math/cVector.h"
#include "Renderable.h"
#include "RenderableHelper.h"
#include "RenderableManager.h"

// Helper Function Declarations
//=============================

namespace
{

}

// Interface
//==========

void eae6320::Graphics::Render()
{
	// Every frame an entirely new image will be created.
	// Before drawing anything, then, the previous image will be erased
	// by "clearing" the image buffer (filling it with a solid color)
	{
		sColor clearColor = { 0.0f, 0.0f, 0.0f, 0.0f };
		if (!Clear(clearColor, Context::Get()))
		{
			assert(false);
		}
	}

	if (!OnSubmitRenderCommands_start(Context::Get()))
	{
		assert(false);
	}

	// The actual function calls that draw geometry
	{
		// Set the vertex and fragment shaders
		{
			for (unsigned int i = 0; i < RenderableManager::Get().m_list.size(); i++)
			{
				Renderable& renderable = RenderableManager::Get().m_list[i];
				if (!EffectHelper::Bind(*renderable.m_effect, Context::Get()))
				{
					assert(false);
				}
				if (!EffectHelper::SetDrawCallUniforms(*renderable.m_effect, renderable.cVector, Context::Get()))
				{
					assert(false);
				}
				if (!MeshHelper::DrawMesh(*renderable.m_mesh, Context::Get()))
				{
					assert(false);
				}
			}
			RenderableManager::Get().CleanUp();
		}
	}

	if (!OnSubmitRenderCommands_end(Context::Get()))
	{
		assert(false);
	}

	// Everything has been drawn to the "back buffer", which is just an image in memory.
	// In order to display it, the contents of the back buffer must be swapped with the "front buffer"
	// (which is what the user sees)
	if (!DisplayRenderedBuffer(Context::Get()))
	{
		assert(false);
	}
}