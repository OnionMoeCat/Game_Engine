/*
This file contains the function declarations for mesh
*/

#ifndef EAE6320_ENTITY_H
#define EAE6320_ENTITY_H

// Header Files
//=============
#include "../Windows/Includes.h"

// Interface
//==========

namespace eae6320
{
	namespace Graphics
	{
		struct Entity
		{
			Mesh* m_mesh;
			Effect* m_effect;
			cVector cVector;
		};
	}
}

#endif	// EAE6320_MESH_H
