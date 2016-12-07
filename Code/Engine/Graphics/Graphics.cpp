// Header Files
//=============

#include "Graphics.h"

#include <cassert>
#include <gl/GL.h>
#include <string>
#include <sstream>
#include "../UserOutput/UserOutput.h"
#include "../Windows/Functions.h"
#include "Renderable.h"
#include "RenderableHelper.h"
#include "RenderableManager.h"
#include "MaterialHelper.h"

// Interface
//==========

void eae6320::Graphics::Core::Render()
{
	// Every frame an entirely new image will be created.
	// Before drawing anything, then, the previous image will be erased
	// by "clearing" the image buffer (filling it with a solid color)
	bool result;
	{
		//Color in format of RGBA
		sColor clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		float depth = 1.0f;
		result = Clear(clearColor, depth, Context::Get());
		assert(result);
	}

	result = OnSubmitRenderCommands_start(Context::Get());
	assert(result);

	// The actual function calls that draw geometry
	{
		// Set the vertex and fragment shaders
		{
			for (unsigned int i = 0; i < RenderableManager::Get().m_list.size(); i++)
			{
				Renderable& renderable = RenderableManager::Get().m_list[i];
				result = EffectHelper::Bind(*renderable.m_material->m_effect, Context::Get());
				assert(result);
				result = MaterialHelper::SetMaterialUniforms(*renderable.m_material, Context::Get());
				assert(result);
				result = MaterialHelper::SetMaterialTextures(*renderable.m_material, Context::Get());
				assert(result);
				result = MeshHelper::DrawMesh(*renderable.m_mesh, Context::Get());
				assert(result);
			}
			RenderableManager::Get().CleanUp();
		}
	}

	result = OnSubmitRenderCommands_end(Context::Get());
	assert(result);

	// Everything has been drawn to the "back buffer", which is just an image in memory.
	// In order to display it, the contents of the back buffer must be swapped with the "front buffer"
	// (which is what the user sees)
	result = DisplayRenderedBuffer(Context::Get());
	assert(result);
}