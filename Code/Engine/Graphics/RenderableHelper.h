/*
This file contains the function declarations for MeshHelper
*/

#ifndef EAE6320_RENDERABLE_HELPER_H
#define EAE6320_RENDERABLE_HELPER_H

// Header Files
//=============
#include "Renderable.h"

// Interface
//==========

namespace eae6320
{
	namespace Graphics
	{
		struct RenderableHelper
		{			
			bool static CleanUp(Renderable& i_renderable);
			bool static LoadRenderableFromFile(Renderable& i_renderable, const char* const i_effectPath, const char* i_meshPath);
			bool static CreateLocalToWorldTransform(Renderable& i_renderable, const eae6320::Math::cQuaternion& i_rotation, const eae6320::Math::cVector& i_position);
			bool static CreateWorldToViewTransform(Renderable& i_renderable, const eae6320::Math::cQuaternion& i_cameraRotation, const eae6320::Math::cVector& i_cameraPosition);
			bool static CreateViewToScreenTransform(Renderable& i_renderable, const float i_fov, const float i_aspect, const float i_nearZ, const float i_farZ);
		};
	}
}

#endif	// EAE6320_GRAPHICS_H
