/*
This file contains the function declarations for mesh
*/

#ifndef EAE6320_ENTITY_MANAGER_H
#define EAE6320_ENTITY_MANAGER_H

// Header Files
//=============
#include "Renderable.h"

#include <vector>

// Interface
//==========

namespace eae6320
{
	namespace Graphics
	{
		struct RenderableManager
		{
			std::vector<Renderable> m_list;
			static RenderableManager& Get();
			void CleanUp();
			void Submit(const Renderable& i_renderable);
		private:
			static RenderableManager m_Instance;
		};
	}
}

#endif	// EAE6320_MESH_H
