/*
This file contains the function declarations for mesh
*/

#ifndef EAE6320_RENDERABLE_MANAGER_H
#define EAE6320_RENDERABLE_MANAGER_H

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
			static RenderableManager& Get();
			void CleanUp();
			void Submit(const Renderable& i_renderable);
			size_t GetSize();
			Renderable* GetRenderableAtIndex(size_t i_index);
		private:
			static RenderableManager m_Instance;
			std::vector<Renderable> m_transparent_list;
			std::vector<Renderable> m_opaque_list;
		};
	}
}

#endif	// EAE6320_MESH_H
