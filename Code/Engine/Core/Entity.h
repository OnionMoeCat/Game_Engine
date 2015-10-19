/*
This file contains the function declarations for mesh
*/

#ifndef EAE6320_ENTITY_H
#define EAE6320_ENTITY_H

// Header Files
//=============
#include "../Graphics/Mesh.h"
#include "../Graphics/Effect.h"
#include "../Math/cVector.h"

// Interface
//==========

namespace eae6320
{
	namespace Core
	{
		struct Entity
		{
			eae6320::Graphics::Mesh* m_mesh = NULL;
			eae6320::Graphics::Effect* m_effect = NULL;
			eae6320::Math::cVector cVector;
		};
	}
}

#endif	// EAE6320_MESH_H
