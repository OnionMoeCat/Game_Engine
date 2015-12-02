/*
This file contains the function declarations for mesh
*/

#ifndef EAE6320_ENTITY_MANAGER_H
#define EAE6320_ENTITY_MANAGER_H

// Header Files
//=============
#include "Entity.h"

#include <vector>

// Interface
//==========

namespace eae6320
{
	namespace Core
	{
		struct EntityManager
		{
		private:
			typedef std::vector<Entity> EntityList;
			EntityList m_list;
			static EntityManager m_Instance;
			EntityManager();
		public:
			static EntityManager& Get();
			void CleanUp();
			bool CreateEntityFromFile(const char* const i_materialPath, const char* const i_meshPath);
			typedef EntityList::iterator iterator;
			typedef EntityList::const_iterator const_iterator;
			iterator begin() { return m_list.begin(); }
			iterator end() { return m_list.end(); }
		};
	}
}

#endif	// EAE6320_MESH_H
