#ifndef EAE6320_RENDERABLE_H
#define EAE6320_RENDERABLE_H

// Header Files
//=============
#include "../Graphics/Mesh.h"
#include "../Graphics/Effect.h"
#include "../Graphics/Material.h"
#include "../Math/cVector.h"
#include "../Math/cQuaternion.h"

// Interface
//==========

namespace eae6320
{
	namespace Graphics
	{
		struct Renderable
		{
			eae6320::Graphics::Mesh* m_mesh = NULL;
			eae6320::Graphics::Material* m_material = NULL;
		};
	}
}

#endif	// EAE6320_MESH_H
