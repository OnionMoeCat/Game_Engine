/*
This file contains the function declarations for mesh
*/

#ifndef EAE6320_ENTITY_MANAGER_H
#define EAE6320_ENTITY_MANAGER_H

// Header Files
//=============
#include "Entity.h"
#include "EntityHandle.h"

#include <vector>

// Interface
//==========

namespace eae6320
{
	namespace Core
	{
		struct EntityManager
		{
		public:
			static EntityManager& Get();
			void CleanUp();
			void Release(const EntityHandle& i_handle);
			bool CreateEntityFromFile(const char* const i_materialPath, const char* const i_meshPath, EntityHandle& o_handle);
			EntityHandle GetHandleAtIndex(size_t i_index);
			size_t GetEntitySize();
			friend struct EntityHandle;
		private:
			static uint32_t g_UniqueID;
			typedef std::vector<Entity*> EntityList;
			EntityList m_list;
			static EntityManager m_Instance;
			EntityManager();
			EntityHandle InsertAndGetIndex(Entity* i_entity);
		};
	}
}

#endif	// EAE6320_MESH_H
